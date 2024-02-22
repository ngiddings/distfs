/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _FS454_H_RPCGEN
#define _FS454_H_RPCGEN

#include <rpc/rpc.h>

#ifdef __cplusplus
extern "C" {
#endif


struct file_attributes {
	long id;
	long link_count;
	long filesize;
	long owner;
	long mode;
	long last_access;
	long last_modify;
	long last_change;
};
typedef struct file_attributes file_attributes;

struct dir_entry {
	struct {
		u_int name_len;
		char *name_val;
	} name;
	struct file_attributes attributes;
	struct dir_entry *next;
};
typedef struct dir_entry dir_entry;

enum error_code {
	FS454_ENONE = 0,
	FS454_ENOINODE = 0 + 1,
	FS454_EINVAL = 0 + 2,
	FS454_EIO = 0 + 3,
	FS454_EBADF = 0 + 4,
	FS454_EACCESS = 0 + 5,
	FS454_ENOTDIR = 0 + 6,
	FS454_ENOTEMPTY = 0 + 7,
};
typedef enum error_code error_code;

typedef dir_entry *entry_list;

typedef struct {
	u_int byte_buffer_len;
	u_char *byte_buffer_val;
} byte_buffer;


struct attribute_res {
	error_code err;
	union {
		file_attributes attributes;
	} attribute_res_u;
};
typedef struct attribute_res attribute_res;

struct string_res {
	error_code err;
	union {
		char *str;
	} string_res_u;
};
typedef struct string_res string_res;

struct readdir_res {
	error_code err;
	union {
		entry_list entries;
	} readdir_res_u;
};
typedef struct readdir_res readdir_res;

struct buffer_res {
	error_code err;
	union {
		byte_buffer buffer;
	} buffer_res_u;
};
typedef struct buffer_res buffer_res;

struct allocate_1_argument {
	long arg1;
	long arg2;
};
typedef struct allocate_1_argument allocate_1_argument;

struct link_1_argument {
	long arg1;
	char *arg2;
	long arg3;
};
typedef struct link_1_argument link_1_argument;

struct lookup_1_argument {
	char *arg1;
	long arg2;
};
typedef struct lookup_1_argument lookup_1_argument;

struct mkdir_1_argument {
	char *arg1;
	long arg2;
	long arg3;
};
typedef struct mkdir_1_argument mkdir_1_argument;

struct mknod_1_argument {
	char *arg1;
	long arg2;
	long arg3;
};
typedef struct mknod_1_argument mknod_1_argument;

struct read_1_argument {
	long arg1;
	long arg2;
	long arg3;
};
typedef struct read_1_argument read_1_argument;

struct setattr_1_argument {
	long arg1;
	long arg2;
	struct file_attributes arg3;
};
typedef struct setattr_1_argument setattr_1_argument;

struct symlink_1_argument {
	char *arg1;
	long arg2;
	char *arg3;
};
typedef struct symlink_1_argument symlink_1_argument;

struct unlink_1_argument {
	long arg1;
	char *arg2;
};
typedef struct unlink_1_argument unlink_1_argument;

struct write_1_argument {
	long arg1;
	byte_buffer arg2;
	long arg3;
	long arg4;
};
typedef struct write_1_argument write_1_argument;

struct copy_1_argument {
	long arg1;
	long arg2;
	long arg3;
	long arg4;
	long arg5;
};
typedef struct copy_1_argument copy_1_argument;

#define FS454 0x33764824
#define MESSAGEVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define ALLOCATE 1
extern  error_code * allocate_1(long , long , CLIENT *);
extern  error_code * allocate_1_svc(long , long , struct svc_req *);
#define GETATTR 2
extern  attribute_res * getattr_1(long , CLIENT *);
extern  attribute_res * getattr_1_svc(long , struct svc_req *);
#define LINK 3
extern  attribute_res * link_1(long , char *, long , CLIENT *);
extern  attribute_res * link_1_svc(long , char *, long , struct svc_req *);
#define LOOKUP 4
extern  attribute_res * lookup_1(char *, long , CLIENT *);
extern  attribute_res * lookup_1_svc(char *, long , struct svc_req *);
#define MKDIR 5
extern  attribute_res * mkdir_1(char *, long , long , CLIENT *);
extern  attribute_res * mkdir_1_svc(char *, long , long , struct svc_req *);
#define MKNOD 6
extern  attribute_res * mknod_1(char *, long , long , CLIENT *);
extern  attribute_res * mknod_1_svc(char *, long , long , struct svc_req *);
#define READ 7
extern  buffer_res * read_1(long , long , long , CLIENT *);
extern  buffer_res * read_1_svc(long , long , long , struct svc_req *);
#define READDIR 8
extern  readdir_res * readdir_1(long , CLIENT *);
extern  readdir_res * readdir_1_svc(long , struct svc_req *);
#define READLINK 9
extern  string_res * readlink_1(long , CLIENT *);
extern  string_res * readlink_1_svc(long , struct svc_req *);
#define SETATTR 10
extern  attribute_res * setattr_1(long , long , struct file_attributes , CLIENT *);
extern  attribute_res * setattr_1_svc(long , long , struct file_attributes , struct svc_req *);
#define SYMLINK 11
extern  attribute_res * symlink_1(char *, long , char *, CLIENT *);
extern  attribute_res * symlink_1_svc(char *, long , char *, struct svc_req *);
#define UNLINK 12
extern  error_code * unlink_1(long , char *, CLIENT *);
extern  error_code * unlink_1_svc(long , char *, struct svc_req *);
#define WRITE 13
extern  error_code * write_1(long , byte_buffer , long , long , CLIENT *);
extern  error_code * write_1_svc(long , byte_buffer , long , long , struct svc_req *);
#define COPY 14
extern  error_code * copy_1(long , long , long , long , long , CLIENT *);
extern  error_code * copy_1_svc(long , long , long , long , long , struct svc_req *);
extern int fs454_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define ALLOCATE 1
extern  error_code * allocate_1();
extern  error_code * allocate_1_svc();
#define GETATTR 2
extern  attribute_res * getattr_1();
extern  attribute_res * getattr_1_svc();
#define LINK 3
extern  attribute_res * link_1();
extern  attribute_res * link_1_svc();
#define LOOKUP 4
extern  attribute_res * lookup_1();
extern  attribute_res * lookup_1_svc();
#define MKDIR 5
extern  attribute_res * mkdir_1();
extern  attribute_res * mkdir_1_svc();
#define MKNOD 6
extern  attribute_res * mknod_1();
extern  attribute_res * mknod_1_svc();
#define READ 7
extern  buffer_res * read_1();
extern  buffer_res * read_1_svc();
#define READDIR 8
extern  readdir_res * readdir_1();
extern  readdir_res * readdir_1_svc();
#define READLINK 9
extern  string_res * readlink_1();
extern  string_res * readlink_1_svc();
#define SETATTR 10
extern  attribute_res * setattr_1();
extern  attribute_res * setattr_1_svc();
#define SYMLINK 11
extern  attribute_res * symlink_1();
extern  attribute_res * symlink_1_svc();
#define UNLINK 12
extern  error_code * unlink_1();
extern  error_code * unlink_1_svc();
#define WRITE 13
extern  error_code * write_1();
extern  error_code * write_1_svc();
#define COPY 14
extern  error_code * copy_1();
extern  error_code * copy_1_svc();
extern int fs454_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_file_attributes (XDR *, file_attributes*);
extern  bool_t xdr_dir_entry (XDR *, dir_entry*);
extern  bool_t xdr_error_code (XDR *, error_code*);
extern  bool_t xdr_entry_list (XDR *, entry_list*);
extern  bool_t xdr_byte_buffer (XDR *, byte_buffer*);
extern  bool_t xdr_file_attributes (XDR *, file_attributes*);
extern  bool_t xdr_attribute_res (XDR *, attribute_res*);
extern  bool_t xdr_string_res (XDR *, string_res*);
extern  bool_t xdr_readdir_res (XDR *, readdir_res*);
extern  bool_t xdr_buffer_res (XDR *, buffer_res*);
extern  bool_t xdr_allocate_1_argument (XDR *, allocate_1_argument*);
extern  bool_t xdr_link_1_argument (XDR *, link_1_argument*);
extern  bool_t xdr_lookup_1_argument (XDR *, lookup_1_argument*);
extern  bool_t xdr_mkdir_1_argument (XDR *, mkdir_1_argument*);
extern  bool_t xdr_mknod_1_argument (XDR *, mknod_1_argument*);
extern  bool_t xdr_read_1_argument (XDR *, read_1_argument*);
extern  bool_t xdr_setattr_1_argument (XDR *, setattr_1_argument*);
extern  bool_t xdr_symlink_1_argument (XDR *, symlink_1_argument*);
extern  bool_t xdr_unlink_1_argument (XDR *, unlink_1_argument*);
extern  bool_t xdr_write_1_argument (XDR *, write_1_argument*);
extern  bool_t xdr_copy_1_argument (XDR *, copy_1_argument*);

#else /* K&R C */
extern bool_t xdr_file_attributes ();
extern bool_t xdr_dir_entry ();
extern bool_t xdr_error_code ();
extern bool_t xdr_entry_list ();
extern bool_t xdr_byte_buffer ();
extern bool_t xdr_file_attributes ();
extern bool_t xdr_attribute_res ();
extern bool_t xdr_string_res ();
extern bool_t xdr_readdir_res ();
extern bool_t xdr_buffer_res ();
extern bool_t xdr_allocate_1_argument ();
extern bool_t xdr_link_1_argument ();
extern bool_t xdr_lookup_1_argument ();
extern bool_t xdr_mkdir_1_argument ();
extern bool_t xdr_mknod_1_argument ();
extern bool_t xdr_read_1_argument ();
extern bool_t xdr_setattr_1_argument ();
extern bool_t xdr_symlink_1_argument ();
extern bool_t xdr_unlink_1_argument ();
extern bool_t xdr_write_1_argument ();
extern bool_t xdr_copy_1_argument ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_FS454_H_RPCGEN */
