#define FUSE_USE_VERSION 34

#include "fuse3/fuse_lowlevel.h"
#include "fs454.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CLIENT *rpc_client;

struct buffer_t
{
    char *ptr;
    size_t size;
};

void fill_stat(struct stat *dest, file_attributes *src) {
    memset(dest, 0, sizeof(struct stat));
    dest->st_ino = src->id;
    dest->st_size = src->filesize;
    dest->st_uid = src->owner;
    dest->st_gid = 0;
    dest->st_mode = src->mode;
    dest->st_nlink = src->link_count;
    dest->st_atime = src->last_access;
    dest->st_ctime = src->last_change;
    dest->st_mtime = src->last_modify;
}

int buffer_resize(struct buffer_t *buffer, size_t size) {
    if(buffer != NULL) {
        buffer->ptr = realloc(buffer->ptr, size);
        if(buffer->ptr == NULL) {
            buffer->size = 0;
            return 1;
        } else {
            buffer->size = size;
            return 0;
        }
    } else {
        return 1;
    }
}

void buffer_free(struct buffer_t *buffer) {
    if(buffer != NULL) {
        if(buffer->ptr != NULL) {
            free(buffer->ptr);
            buffer->ptr = NULL;
        }
        buffer->size = 0;
    }
}

int append_dirent(fuse_req_t request, struct buffer_t *buffer, const char *name, struct stat *attr) {
    size_t prev_size = buffer->size;
    if(buffer_resize(buffer, prev_size + fuse_add_direntry(request, NULL, 0, name, NULL, 0))) {
        return 1;
    }
    fuse_add_direntry(request, buffer->ptr + prev_size, buffer->size - prev_size, name, attr, buffer->size);
    return 0;
}

void fs454_access(fuse_req_t req, fuse_ino_t ino, int mask) {
    fuse_reply_err(req, ENOSYS);
}

void fs454_copy_file_range(fuse_req_t req, fuse_ino_t ino_in, off_t off_in, struct fuse_file_info *fi_in, fuse_ino_t ino_out, off_t off_out, struct fuse_file_info *fi_out, size_t len, int flags) {
    fuse_reply_err(req, ENOSYS);
}

void fs454_fallocate(fuse_req_t request, fuse_ino_t inode, int mode, off_t offset, off_t length, struct fuse_file_info *fi) {
    if(mode == 0) {
        error_code *result = allocate_1(inode, offset + length, rpc_client);
        if(result == NULL) {
            fuse_reply_err(request, EIO);
        } else if(*result == FS454_ENONE) {
            fuse_reply_err(request, 0);
        } else {
            fuse_reply_err(request, ENOENT);
        }
    } else {
        fuse_reply_err(request, EOPNOTSUPP);
    }
}

void fs454_getattr(fuse_req_t request, ino_t inode, struct fuse_file_info *info) {
    attribute_res *result = getattr_1(inode, rpc_client);
    if(result == NULL) {
        fuse_reply_err(request, EIO);
    } else if(result->err == FS454_ENONE) {
        struct stat s;
        fill_stat(&s, &result->attribute_res_u.attributes);
        fuse_reply_attr(request, &s, 0);
    } else {
        printf("\terror %i in getattr\n", result->err);
        fuse_reply_err(request, ENOENT);
    }
}

void fs454_link(fuse_req_t request, fuse_ino_t inode, fuse_ino_t newparent, const char *newname) {
    attribute_res *result = link_1(inode, newname, newparent, rpc_client);
    if(result == NULL) {
        fuse_reply_err(request, EIO);
    } else if(result->err != FS454_ENONE) {
        printf("link %i/%s -> %i failed with error %i\n", newparent, newname, inode, result->err);
        fuse_reply_err(request, ENOENT);
    } else {
        struct fuse_entry_param entry = {
            .attr_timeout = 0,
            .entry_timeout = 0,
            .generation = 0,
            .ino = result->attribute_res_u.attributes.id
        };
        fill_stat(&entry.attr, &result->attribute_res_u.attributes);
        fuse_reply_entry(request, &entry);
    }
}

void fs454_lookup(fuse_req_t request, fuse_ino_t parent, const char *name) {
    attribute_res *result = lookup_1(name, parent, rpc_client);
    if(result == NULL) {
        fuse_reply_err(request, EIO);
    } else if(result->err == FS454_ENONE) {
        struct fuse_entry_param entry = {
            .attr_timeout = 0,
            .entry_timeout = 0,
            .generation = 0,
            .ino = result->attribute_res_u.attributes.id
        };
        fill_stat(&entry.attr, &result->attribute_res_u.attributes);
        fuse_reply_entry(request, &entry);
    } else {
        fuse_reply_err(request, ENOENT);
    }
}

void fs454_mknod(fuse_req_t request, fuse_ino_t parent, const char *name, mode_t mode, dev_t rdev) {
    attribute_res *result = mknod_1(name, parent, mode, rpc_client);
    if(result == NULL) {
        fuse_reply_err(request, EIO);
    } else if(result->err == FS454_ENONE) {
        struct fuse_entry_param entry = {
            .attr_timeout = 0,
            .entry_timeout = 0,
            .generation = 0,
            .ino = result->attribute_res_u.attributes.id
        };
        fill_stat(&entry.attr, &result->attribute_res_u.attributes);
        fuse_reply_entry(request, &entry);
    } else {
        fuse_reply_err(request, EINVAL);
    }
}

void fs454_mkdir(fuse_req_t request, fuse_ino_t parent, const char *name, mode_t mode) {
    attribute_res *result = mkdir_1(name, parent, mode, rpc_client);
    if(result == NULL) {
        fuse_reply_err(request, EIO);
    } else if(result->err == FS454_ENONE) {
        struct fuse_entry_param entry = {
            .attr_timeout = 0,
            .entry_timeout = 0,
            .generation = 0,
            .ino = result->attribute_res_u.attributes.id
        };
        fill_stat(&entry.attr, &result->attribute_res_u.attributes);
        fuse_reply_entry(request, &entry);
    } else {
        printf("mkdir failed error %i\n", result->err);
        fuse_reply_err(request, EINVAL);
    }
}

void fs454_open(fuse_req_t request, fuse_ino_t inode, struct fuse_file_info *finfo) {
    finfo->keep_cache = 0;
    fuse_reply_open(request, finfo);
}

void fs454_read(fuse_req_t request, fuse_ino_t inode, size_t size, off_t off, struct fuse_file_info *info) {
    buffer_res *result = read_1(inode, off, size, rpc_client);
    if(result == NULL) {
        fuse_reply_err(request, EIO);
    } else if(result->err == FS454_ENONE) {
        fuse_reply_buf(request, result->buffer_res_u.buffer.byte_buffer_val, result->buffer_res_u.buffer.byte_buffer_len);
    } else {
        printf("read failed error %i\n", result->err);
        fuse_reply_err(request, ENOENT);
    }
}

void fs454_readdir(fuse_req_t request, fuse_ino_t inode, size_t size, off_t off, struct fuse_file_info *info) {
    readdir_res *result = readdir_1(inode, rpc_client);
    if(result == NULL) {
        fuse_reply_err(request, EIO);
    } else if(result->err == FS454_ENONE) {
        struct buffer_t buffer = {0};
        struct dir_entry *entry = result->readdir_res_u.entries;
        while(entry != NULL) {
            struct stat attr;
            fill_stat(&attr, &entry->attributes);
            append_dirent(request, &buffer, entry->name.name_val, &attr);
            entry = entry->next;
        }
        if(off < buffer.size) {
            if(buffer.size - off < size) {
                fuse_reply_buf(request, buffer.ptr + off, buffer.size - off);
            } else {
                fuse_reply_buf(request, buffer.ptr + off, size);
            }
        } else {
            fuse_reply_buf(request, NULL, 0);
        }
    } else {
        printf("readdir failed error %i\n", result->err);
        fuse_reply_err(request, EINVAL);
    }
}

void fs454_readlink(fuse_req_t request, fuse_ino_t inode) {
    string_res *link = readlink_1(inode, rpc_client);
    if(link == NULL) {
        fuse_reply_err(request, EIO);
    } else {
        fuse_reply_readlink(request, link->string_res_u.str);
    }
}

void fs454_rename(fuse_req_t request, fuse_ino_t parent, const char *name, fuse_ino_t newparent, const char *newname, unsigned int flags) {
    if(parent == newparent && strcmp(name, newname) == 0) {
        fuse_reply_err(request, 0);
        return;
    }
    attribute_res *oldattr = lookup_1(name, parent, rpc_client);
    if(oldattr == NULL) {
        fuse_reply_err(request, EIO);
        return;
    } else if(oldattr->err != FS454_ENONE) {
        fuse_reply_err(request, ENOENT);
    }
    attribute_res *link_result = link_1(oldattr->attribute_res_u.attributes.id, newname, newparent, rpc_client);
    if(link_result == NULL) {
        fuse_reply_err(request, EIO);
    } else if(link_result->err != FS454_ENONE) {
        printf("rename failed to link with error %i\n", link_result->err);
        fuse_reply_err(request, EIO);
    } else {
        error_code *unlink_result = unlink_1(parent, name, rpc_client);
        if(*unlink_result != FS454_ENONE) {
            printf("rename failed to unlink with error %i\n", link_result->err);
            fuse_reply_err(request, EIO);
        } else {
            fuse_reply_err(request, 0);
        }
    }
}

void fs454_rmdir(fuse_req_t request, fuse_ino_t parent, const char *name) {
    error_code *result = unlink_1(parent, name, rpc_client);
    if(result == NULL) {
        fuse_reply_err(request, EIO);
    } else {
        fuse_reply_err(request, *result == FS454_ENONE ? 0 : EINVAL);
    }
}

void fs454_setattr(fuse_req_t request, fuse_ino_t inode, struct stat *attr, int to_set, struct fuse_file_info *fi) {
    file_attributes attributes = {
        .id = inode,
        .filesize = attr->st_size,
        .owner = attr->st_uid,
        .mode = attr->st_mode,
        .last_access = attr->st_atime,
        .last_change = attr->st_ctime,
        .last_modify = attr->st_mtime
    };
    attribute_res *result = setattr_1(inode, to_set, attributes, rpc_client);
    if(result == NULL) {
        fuse_reply_err(request, EIO);
    } else if(result->err == FS454_ENONE) {
        struct stat s;
        fill_stat(&s, &result->attribute_res_u.attributes);
        fuse_reply_attr(request, &s, 0);
    } else {
        printf("setattr error %i\n", result->err);
        fuse_reply_err(request, EINVAL);
    }
}

void fs454_symlink(fuse_req_t request, const char *link, fuse_ino_t parent, const char *name) {
    attribute_res *result = symlink_1(link, parent, name, rpc_client);
    if(result == NULL) {
        fuse_reply_err(request, EIO);
    } else {
        file_attributes *attr = &result->attribute_res_u.attributes;
        struct stat n;
        struct fuse_entry_param entry = {
            .attr_timeout = 0,
            .entry_timeout = 0,
            .generation = 0,
            .ino = attr->id
        };
        fill_stat(&entry.attr, attr);
        fuse_reply_entry(request, &entry);
    }
}

void fs454_unlink(fuse_req_t request, fuse_ino_t parent, const char *name) {
    error_code *result = unlink_1(parent, name, rpc_client);
    if(result == NULL) {
        fuse_reply_err(request, EIO);
    } else {
        fuse_reply_err(request, (int) (*result == FS454_ENONE ? 0 : ENOENT));
    }
}

void fs454_write(fuse_req_t request, fuse_ino_t inode, const char *buffer, size_t size, off_t offset, struct fuse_file_info *fi) {
    char *copy = malloc(size);
    memcpy(copy, buffer, size);
    byte_buffer arg_buffer = {
        .byte_buffer_len = size,
        .byte_buffer_val = copy
    };
    error_code *result = write_1(inode, arg_buffer, offset, size, rpc_client);
    if(result == NULL) {
        fuse_reply_err(request, EIO);
    } else if(*result == FS454_ENONE) {
        fuse_reply_write(request, size);
    } else {
        printf("write failed error %i\n", *result);
        fuse_reply_write(request, ENOENT);
    }
}

int main(int argc, char** argv) {
    const struct fuse_lowlevel_ops fs454_ops = {
        .access = fs454_access,
        .copy_file_range = fs454_copy_file_range,
        .fallocate = fs454_fallocate,
        .getattr = fs454_getattr,
        .link = fs454_link,
        .lookup = fs454_lookup,
        .mknod = fs454_mknod,
        .mkdir = fs454_mkdir,
        .open = fs454_open,
        .read = fs454_read,
        .readdir = fs454_readdir,
        .readlink = fs454_readlink,
        .rename = fs454_rename,
        .rmdir = fs454_rmdir,
        .setattr = fs454_setattr,
        .symlink = fs454_symlink,
        .unlink = fs454_unlink,
        .write = fs454_write
    };
    
    const char *host = NULL;
    if(argc > 2) {
        host = argv[argc - 1];
        argc--;
    } else {
        printf("Please specify a host. Usage: %s [fuse options] [host]\n", argv[0]);
        return 0;
    }

    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    struct fuse_cmdline_opts opts;
    struct fuse_loop_config cfg;
    struct fuse_session *session;
    
    if(fuse_parse_cmdline(&args, &opts) != 0) {
        return 1;
    }

    if(opts.show_help) {
        fuse_cmdline_help();
        fuse_lowlevel_help();
    }

    if(opts.mountpoint == NULL) {
        fuse_opt_free_args(&args);
        return 1;
    }

    printf("Connecting to '%s'...\n", host);

    rpc_client = clnt_create(host, FS454, MESSAGEVERS, "tcp");
    if(rpc_client == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    printf("Connected.\n");

    session = fuse_session_new(&args, &fs454_ops, sizeof(fs454_ops), NULL);

    if(fuse_set_signal_handlers(session) != 0) {
        fuse_session_destroy(session);
        fuse_opt_free_args(&args);
        return 1;
    }

    if(fuse_session_mount(session, opts.mountpoint) != 0) {
        fuse_remove_signal_handlers(session);
        fuse_session_destroy(session);
        fuse_opt_free_args(&args);
        return 1;
    }

    int status = 0;
    fuse_daemonize(opts.foreground);
    if(opts.singlethread) {
        status = fuse_session_loop(session);
    } else {
        cfg.clone_fd = opts.clone_fd;
        cfg.max_idle_threads = opts.max_idle_threads;
        status = fuse_session_loop_mt(session, &cfg);
    }

    fuse_session_unmount(session);
    fuse_remove_signal_handlers(session);
    fuse_session_destroy(session);
    fuse_opt_free_args(&args);
    return status;
}