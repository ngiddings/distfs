#include "inode_tree.h"
#include "file_tree.h"
#include "fsio.h"
#include "fs454.h"
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#define SET_MODE (1 << 0)
#define SET_OWNER (1 << 1)
#define SET_SIZE (1 << 3)
#define SET_ACCESS (1 << 4)
#define SET_MODIFY (1 << 5)
#define SET_ACCESS_NOW (1 << 7)
#define SET_MODIFY_NOW (1 << 8)
#define SET_CHANGE (1 << 10)

extern inode_tree *inodes;
extern directory_list *directories;

/**
 * @brief Manually check supported bits and copy them to a new mode
 * value. This ensures only valid file modes can be given to the filesystem.
 * If the input mode indicates the use of unsupported features, this function
 * shall return 0 indicating the input mode is illegal.
 * 
 * @param mode file mode to sanitize
 * @return long a sanitized file mode
 */
long sanitize_mode(long mode) {
	long newmode = 0;

	/* Test file type. Only files, directories, and links allowed. */
	if(S_ISBLK(mode)) {
		return 0;
	} else if(S_ISCHR(mode)) {
		return 0;
	} else if(S_ISDIR(mode)) {
		newmode |= __S_IFDIR;
	} else if(S_ISFIFO(mode)) {
		return 0;
	} else if(S_ISREG(mode)) {
		newmode |= __S_IFREG;
	} else if(S_ISLNK(mode)) {
		newmode |= __S_IFLNK;
	}

	/* Build a bitmask for supported permissions. Execute permissions are disabled. */
	long permission_mask = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	newmode |= mode & permission_mask;

	return newmode;
}

error_code *allocate_1_svc(long inode, long length, struct svc_req *rqstp)
{
	static error_code result;
	file_descriptor *desc = inode_tree_find(inodes, inode);
	if(desc == NULL) {
		result = FS454_ENOINODE;
	} else if(length < 0 || length > (1 << 30)) {
		result = FS454_EINVAL;
	} else if(desc->attributes.filesize >= length) {
		result = FS454_ENONE;
	} else {
		if(do_allocate(desc->location, length) != 0) {
			result = FS454_EIO;
		} else {
			desc->attributes.filesize = length;
			desc->attributes.last_change = time(0);
			result = FS454_ENONE;
		}
	}
	return &result;
}

attribute_res *getattr_1_svc(long inode, struct svc_req *rqstp)
{
	static attribute_res result;
	file_descriptor *desc = inode_tree_find(inodes, inode);
	if(desc == NULL) {
		result.err = FS454_ENOINODE;
	} else {
		result.attribute_res_u.attributes = desc->attributes;
		result.err = FS454_ENONE;
	}
	return &result;
}

attribute_res *link_1_svc(long inode, char *newname, long newparent, struct svc_req *rqstp)
{
	static attribute_res result;
	if(inode <= 0 || newparent <= 0 || newname == NULL) {
		result.err = FS454_EINVAL;
	}
	file_descriptor *descriptor = NULL;
	file_descriptor *parent_descriptor = NULL;
	if((descriptor = inode_tree_find(inodes, inode)) == NULL) {
		result.err = FS454_ENOINODE;
	} else if((descriptor = inode_tree_find(inodes, newparent)) == NULL) {
		result.err = FS454_ENOINODE;
	} else if(!S_ISDIR(parent_descriptor->attributes.mode)) {
		result.err = FS454_ENOTDIR;
	} else if(!S_ISREG(descriptor->attributes.mode)) {
		result.err = FS454_EBADF;
	} else if(file_tree_insert(directories, newparent, newname, descriptor->attributes.id) != 0) {
		result.err = FS454_EIO;
	} else {
		result.err = FS454_ENONE;
		descriptor->attributes.link_count++;
		result.attribute_res_u.attributes = descriptor->attributes;
	}
	return &result;
}

attribute_res *lookup_1_svc(char *name, long parent, struct svc_req *rqstp)
{
	static attribute_res result;
	if(name == NULL || parent <= 0) {
		result.err = FS454_EINVAL;
		return &result;
	}
	directory_entry *entry = file_tree_lookup(directories, parent, name);
	if(entry == NULL) {
		result.err = FS454_ENOINODE;
	} else {
		file_descriptor *descriptor = inode_tree_find(inodes, entry->inode);
		if(descriptor == NULL) {
			/* There's a serious problem; this lookup should not fail. */
			fprintf(stderr, "WARNING: Broken link %ld->%ld stored in directory table.\n", parent, entry->inode);
			result.err = FS454_EIO;
		} else {
			result.attribute_res_u.attributes = descriptor->attributes;
			result.err = FS454_ENONE;
		}
	}
	return &result;
}

attribute_res *mkdir_1_svc(char *name, long parent, long mode, struct svc_req *rqstp)
{
	static attribute_res result;
	if(name == NULL) {
		result.err = FS454_EINVAL;
	} else if((mode = sanitize_mode(mode)) == 0 || S_ISREG(mode) || S_ISLNK(mode)) {
		result.err = FS454_EINVAL;
	} else if(parent <= 0) {
		result.err = FS454_EINVAL;
	} else {
		file_descriptor *parent_descriptor = inode_tree_find(inodes, parent);
		file_descriptor *descriptor = NULL;
		if(parent_descriptor == NULL) {
			result.err = FS454_ENOINODE;
		} else if(!S_ISDIR(parent_descriptor->attributes.mode)) {
			result.err = FS454_ENOTDIR;
		} else if((descriptor = inode_tree_create(inodes)) == NULL) {
			result.err = FS454_EIO;
		} else if(file_tree_insert_dir(directories, parent, name, descriptor->attributes.id) != 0) {
			/* There's a serious problem; if a directory is stored in the inode table, inserting a new node should not fail. */
			fprintf(stderr, "WARNING: Failed to insert a new node %s under %ld into the file tree.\n", name, parent);
			if(inode_tree_remove(inodes, descriptor->attributes.id) != 0) {
				/* There's a really serious problem. This inode is known to exist; why can't we remove it? */
				fprintf(stderr, "WARNING: Failed to clean up orphaned inode %ld\n", descriptor->attributes.id);
				/* No obvious way to recover... */
			}
			result.err = FS454_EIO;
		} else {
			descriptor->location = 0;
			descriptor->attributes.mode = mode | __S_IFDIR;
			result.err = FS454_ENONE;
			result.attribute_res_u.attributes = descriptor->attributes;
		}
	}
	return &result;
}

attribute_res *mknod_1_svc(char *name, long parent, long mode, struct svc_req *rqstp)
{
	static attribute_res result;
	if(name == NULL) {
		result.err = FS454_EINVAL;
	} else if((mode = sanitize_mode(mode)) == 0 || S_ISDIR(mode) || S_ISLNK(mode)) {
		result.err = FS454_EINVAL;
	} else if(parent <= 0) {
		result.err = FS454_EINVAL;
	} else {
		file_descriptor *parent_descriptor = inode_tree_find(inodes, parent);
		file_descriptor *descriptor = NULL;
		if(parent_descriptor == NULL) {
			result.err = FS454_ENOINODE;
		} else if(!S_ISDIR(parent_descriptor->attributes.mode)) {
			result.err = FS454_ENOTDIR;
		} else if((descriptor = inode_tree_create(inodes)) == NULL) {
			result.err = FS454_EIO;
		} else if(file_tree_insert(directories, parent, name, descriptor->attributes.id) != 0) {
			/* There's a serious problem; if a directory is stored in the inode table, inserting a new node should not fail. */
			fprintf(stderr, "WARNING: Failed to insert a new node %s under %ld into the file tree.\n", name, parent);
			if(inode_tree_remove(inodes, descriptor->attributes.id) != 0) {
				/* There's a really serious problem. This inode is known to exist; why can't we remove it? */
				fprintf(stderr, "WARNING: Failed to clean up orphaned inode %ld\n", descriptor->attributes.id);
				/* No obvious way to recover... */
			}
			result.err = FS454_EIO;
		} else {
			descriptor->location = do_mknod();
			descriptor->attributes.mode = mode | __S_IFREG;
			result.err = FS454_ENONE;
			result.attribute_res_u.attributes = descriptor->attributes;
			fprintf(stderr, "INFO: Made new inode %ld at location %ld.\n", descriptor->attributes.id, descriptor->location);
		}
	}
	return &result;
}

buffer_res *read_1_svc(long inode, long offset, long len, struct svc_req *rqstp)
{
	static buffer_res result;
	file_descriptor *desc = inode_tree_find(inodes, inode);
	if(desc == NULL) {
		result.err = FS454_ENOINODE;
	} else if(desc->location == 0) {
		result.err = FS454_EBADF;
	} else if(len < 0 || offset < 0 || offset >= desc->attributes.filesize) {
		result.err = FS454_EINVAL;
	} else {
		if(offset + len > desc->attributes.filesize) {
			len = desc->attributes.filesize - offset;
		}
		result.buffer_res_u.buffer.byte_buffer_val = malloc(len);
		result.buffer_res_u.buffer.byte_buffer_len = len;
		if(do_read(desc->location, result.buffer_res_u.buffer.byte_buffer_val, offset, len) != 0) {
			result.err = FS454_EIO;
			free(result.buffer_res_u.buffer.byte_buffer_val);
			result.buffer_res_u.buffer.byte_buffer_val = NULL;
			result.buffer_res_u.buffer.byte_buffer_len = 0;
		} else {
			desc->attributes.last_access = time(0);
			result.err = FS454_ENONE;
		}
	}
	return &result;
}

readdir_res *readdir_1_svc(long inode, struct svc_req *rqstp)
{
	static readdir_res result;
	directory_list_node *entry_list = file_tree_list_dir(directories, inode);
	if(entry_list == NULL) {
		result.err = FS454_ENOINODE;
	} else {
		xdr_free(xdr_readdir_res, &result);
		file_descriptor *dir_desc = inode_tree_find(inodes, entry_list->self.inode);
		file_descriptor *parent_desc = NULL;
		if(entry_list->self.parent > 0) {
			parent_desc = inode_tree_find(inodes, entry_list->self.parent);
		} else {
			parent_desc = dir_desc;
		}
		result.readdir_res_u.entries = malloc(sizeof(dir_entry));
		dir_entry *current_entry = result.readdir_res_u.entries;
		current_entry->name.name_val = malloc(2);
		strcpy(current_entry->name.name_val, ".");
		current_entry->name.name_len = strlen(".") + 1;
		current_entry->attributes = dir_desc->attributes;
		current_entry->next = malloc(sizeof(dir_entry));
		current_entry = current_entry->next;
		current_entry->name.name_val = malloc(3);
		strcpy(current_entry->name.name_val, "..");
		current_entry->name.name_len = strlen("..") + 1;
		current_entry->attributes = parent_desc->attributes;
		dirent_list_node *node = entry_list->directory.first;
		while(node != NULL) {
			current_entry->next = malloc(sizeof(dir_entry));
			current_entry = current_entry->next;
			u_int namelen = strlen(node->entry.name) + 1;
			current_entry->name.name_val = malloc(namelen);
			strcpy(current_entry->name.name_val, node->entry.name);
			current_entry->name.name_len = namelen;
			current_entry->attributes = inode_tree_find(inodes, node->entry.inode)->attributes;
			current_entry->next = NULL;
			node = node->next;
		}
		result.err = FS454_ENONE;
	}
	return &result;
}

string_res *readlink_1_svc(long arg1, struct svc_req *rqstp)
{
	static string_res result;
	result.err = FS454_EINVAL;
	return &result;
}

attribute_res *setattr_1_svc(long inode, long to_set, struct file_attributes attributes, struct svc_req *rqstp)
{
	static attribute_res result;
	file_descriptor *descriptor = NULL;
	if(to_set & SET_SIZE) {
		fprintf(stderr, "INFO: Received request to change file size\n");
		result.err = FS454_EINVAL;
	} else if((attributes.mode = sanitize_mode(attributes.mode)) == 0 && (to_set & SET_MODE)) {
		fprintf(stderr, "INFO: Received invalid file mode\n");
		result.err = FS454_EINVAL;
	} else if(/*(S_ISREG(attributes.mode) || S_ISDIR(attributes.mode) || S_ISLNK(attributes.mode)) && (to_set & SET_MODE)*/ 0) {
		fprintf(stderr, "INFO: Received request to change file type\n");
		result.err = FS454_EINVAL;
	} else if((descriptor = inode_tree_find(inodes, inode)) == NULL) {
		result.err = FS454_ENOINODE;
	} else {
		if(to_set & SET_MODE)
			descriptor->attributes.mode = attributes.mode;
		if(to_set & SET_OWNER)
			descriptor->attributes.owner = attributes.owner;
		if(to_set & SET_CHANGE)
			descriptor->attributes.last_change = attributes.last_change;
		if(to_set & SET_ACCESS)
			descriptor->attributes.last_access = attributes.last_access;
		if(to_set & SET_MODIFY)
			descriptor->attributes.last_modify = attributes.last_modify;
		if(to_set & SET_ACCESS_NOW)
			descriptor->attributes.last_access = time(0);
		if(to_set & SET_MODIFY_NOW)
			descriptor->attributes.last_modify = time(0);
		result.attribute_res_u.attributes = descriptor->attributes;
		result.err = FS454_ENONE;
	}
	return &result;
}

attribute_res *symlink_1_svc(char *arg1, long arg2, char *arg3, struct svc_req *rqstp)
{
	static attribute_res result;
	result.err = FS454_EINVAL;
	return &result;
}

error_code *unlink_1_svc(long parent, char *name, struct svc_req *rqstp)
{
	static error_code result;
	directory_entry *entry = NULL;
	file_descriptor *descriptor = NULL;
	if(parent <= 0 || name == NULL) {
		result = FS454_EINVAL;
	} else if((entry = file_tree_lookup(directories, parent, name)) == NULL) {
		result = FS454_ENOINODE;
	} else if((descriptor = inode_tree_find(inodes, entry->inode)) == NULL) {
		/* There's a serious problem; this lookup should not fail. */
		fprintf(stderr, "WARNING: Broken link %ld->%ld stored in directory table.\n", parent, entry->inode);
		result = FS454_EIO;
	} else if(S_ISDIR(descriptor->attributes.mode)) {
		/* We should only delete empty directories */
		directory_list_node *entry_list = file_tree_list_dir(directories, descriptor->attributes.id);
		if(entry_list == NULL) {
			/* This is a problem. There exists a directory with no entry list stored in the filesystem. */
			fprintf(stderr, "WARNING: Count not find entry list for directory %ld.\n", descriptor->attributes.id);
			result = FS454_EIO;
		} else if(entry_list->directory.count > 0) {
			result = FS454_ENOTEMPTY;
		} else if(file_tree_remove(directories, parent, name) != 0) {
			/* There's a serious problem; this node was found in a lookup, and has no children; why can't we remove it? */
			fprintf(stderr, "WARNING: Failed to remove empty directory node %ld->%s.\n", parent, name);
			result = FS454_EIO;
		} else if(inode_tree_remove(inodes, descriptor->attributes.id) != 0) {
			/* This is a more serious problem; we've removed the link, but can't remove the inode. */
			fprintf(stderr, "WARNING: Failed to remove inode %ld from filesystem.\n", descriptor->attributes.id);
			result = FS454_ENONE;
		} else {
			result = FS454_ENONE;
		}
	} else {
		if(file_tree_remove(directories, parent, name) != 0) {
			/* There's a serious problem; this node was found in a lookup; why can't we remove it? */
			fprintf(stderr, "WARNING: Failed to remove file node %ld->%s.\n", parent, name);
			result = FS454_EIO;
		} else if((descriptor->attributes.link_count--) == 0) {
			/* Neither of the two following functions should ever fail. */
			if(do_rmnod(descriptor->location) != 0) {
				fprintf(stderr, "WARNING: Failed to delete location %ld from disk.\n", descriptor->location);
			} 
			if(inode_tree_remove(inodes, descriptor->attributes.id) != 0) {
				fprintf(stderr, "WARNING: Failed to remove inode %ld from filesystem.\n", descriptor->attributes.id);
			}
			result = FS454_ENONE;
		} else {
			result = FS454_ENONE;
		}
	}
	
	return &result;
}

error_code *write_1_svc(long inode, byte_buffer buffer, long offset, long length, struct svc_req *rqstp)
{
	static error_code result;
	file_descriptor *desc = inode_tree_find(inodes, inode);
	if(desc == NULL) {
		result = FS454_ENOINODE;
	} else if(desc->location == 0) {
		result = FS454_EBADF;
	} else if(offset < 0 || length < 0 || length > (1 << 30) || offset > (1 << 30)) {
		result = FS454_EINVAL;
	} else if(length == 0) {
		result = FS454_ENONE;
	} else if(length > buffer.byte_buffer_len) {
		result = FS454_EINVAL;
	} else if(!(desc->attributes.mode & S_IWUSR)) {
		result = FS454_EACCESS;
	} else if(do_write(desc->location, buffer.byte_buffer_val, offset, length) != 0) {
		result = FS454_EIO;
	} else {
		if(offset + length > desc->attributes.filesize) {
			desc->attributes.filesize = offset + length;
			desc->attributes.last_access = time(0);
			desc->attributes.last_modify = time(0);
			desc->attributes.last_change = time(0);
		}
		result = FS454_ENONE;
	}
	return &result;
}

error_code *copy_1_svc(long arg1, long arg2, long arg3, long arg4, long arg5, struct svc_req *rqstp)
{
	static error_code result;
	result = FS454_EINVAL;
	return &result;
}
