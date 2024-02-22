/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "fs454.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

error_code *
allocate_1(long arg1, long arg2,  CLIENT *clnt)
{
	allocate_1_argument arg;
	static error_code clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.arg1 = arg1;
	arg.arg2 = arg2;
	if (clnt_call (clnt, ALLOCATE, (xdrproc_t) xdr_allocate_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_error_code, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

attribute_res *
getattr_1(long arg1,  CLIENT *clnt)
{
	static attribute_res clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, GETATTR,
		(xdrproc_t) xdr_long, (caddr_t) &arg1,
		(xdrproc_t) xdr_attribute_res, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

attribute_res *
link_1(long arg1, char *arg2, long arg3,  CLIENT *clnt)
{
	link_1_argument arg;
	static attribute_res clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.arg1 = arg1;
	arg.arg2 = arg2;
	arg.arg3 = arg3;
	if (clnt_call (clnt, LINK, (xdrproc_t) xdr_link_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_attribute_res, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

attribute_res *
lookup_1(char *arg1, long arg2,  CLIENT *clnt)
{
	lookup_1_argument arg;
	static attribute_res clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.arg1 = arg1;
	arg.arg2 = arg2;
	if (clnt_call (clnt, LOOKUP, (xdrproc_t) xdr_lookup_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_attribute_res, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

attribute_res *
mkdir_1(char *arg1, long arg2, long arg3,  CLIENT *clnt)
{
	mkdir_1_argument arg;
	static attribute_res clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.arg1 = arg1;
	arg.arg2 = arg2;
	arg.arg3 = arg3;
	if (clnt_call (clnt, MKDIR, (xdrproc_t) xdr_mkdir_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_attribute_res, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

attribute_res *
mknod_1(char *arg1, long arg2, long arg3,  CLIENT *clnt)
{
	mknod_1_argument arg;
	static attribute_res clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.arg1 = arg1;
	arg.arg2 = arg2;
	arg.arg3 = arg3;
	if (clnt_call (clnt, MKNOD, (xdrproc_t) xdr_mknod_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_attribute_res, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

buffer_res *
read_1(long arg1, long arg2, long arg3,  CLIENT *clnt)
{
	read_1_argument arg;
	static buffer_res clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.arg1 = arg1;
	arg.arg2 = arg2;
	arg.arg3 = arg3;
	if (clnt_call (clnt, READ, (xdrproc_t) xdr_read_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_buffer_res, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

readdir_res *
readdir_1(long arg1,  CLIENT *clnt)
{
	static readdir_res clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, READDIR,
		(xdrproc_t) xdr_long, (caddr_t) &arg1,
		(xdrproc_t) xdr_readdir_res, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

string_res *
readlink_1(long arg1,  CLIENT *clnt)
{
	static string_res clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, READLINK,
		(xdrproc_t) xdr_long, (caddr_t) &arg1,
		(xdrproc_t) xdr_string_res, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

attribute_res *
setattr_1(long arg1, long arg2, struct file_attributes arg3,  CLIENT *clnt)
{
	setattr_1_argument arg;
	static attribute_res clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.arg1 = arg1;
	arg.arg2 = arg2;
	arg.arg3 = arg3;
	if (clnt_call (clnt, SETATTR, (xdrproc_t) xdr_setattr_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_attribute_res, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

attribute_res *
symlink_1(char *arg1, long arg2, char *arg3,  CLIENT *clnt)
{
	symlink_1_argument arg;
	static attribute_res clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.arg1 = arg1;
	arg.arg2 = arg2;
	arg.arg3 = arg3;
	if (clnt_call (clnt, SYMLINK, (xdrproc_t) xdr_symlink_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_attribute_res, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

error_code *
unlink_1(long arg1, char *arg2,  CLIENT *clnt)
{
	unlink_1_argument arg;
	static error_code clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.arg1 = arg1;
	arg.arg2 = arg2;
	if (clnt_call (clnt, UNLINK, (xdrproc_t) xdr_unlink_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_error_code, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

error_code *
write_1(long arg1, byte_buffer arg2, long arg3, long arg4,  CLIENT *clnt)
{
	write_1_argument arg;
	static error_code clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.arg1 = arg1;
	arg.arg2 = arg2;
	arg.arg3 = arg3;
	arg.arg4 = arg4;
	if (clnt_call (clnt, WRITE, (xdrproc_t) xdr_write_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_error_code, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

error_code *
copy_1(long arg1, long arg2, long arg3, long arg4, long arg5,  CLIENT *clnt)
{
	copy_1_argument arg;
	static error_code clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	arg.arg1 = arg1;
	arg.arg2 = arg2;
	arg.arg3 = arg3;
	arg.arg4 = arg4;
	arg.arg5 = arg5;
	if (clnt_call (clnt, COPY, (xdrproc_t) xdr_copy_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_error_code, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}