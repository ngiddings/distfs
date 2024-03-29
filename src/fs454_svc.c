/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "fs454.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "inode_tree.h"
#include "file_tree.h"
#include "fsio.h"

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

inode_tree *inodes;
directory_list *directories;

static error_code *
_allocate_1 (allocate_1_argument *argp, struct svc_req *rqstp)
{
	return (allocate_1_svc(argp->arg1, argp->arg2, rqstp));
}

static attribute_res *
_getattr_1 (long  *argp, struct svc_req *rqstp)
{
	return (getattr_1_svc(*argp, rqstp));
}

static attribute_res *
_link_1 (link_1_argument *argp, struct svc_req *rqstp)
{
	return (link_1_svc(argp->arg1, argp->arg2, argp->arg3, rqstp));
}

static attribute_res *
_lookup_1 (lookup_1_argument *argp, struct svc_req *rqstp)
{
	return (lookup_1_svc(argp->arg1, argp->arg2, rqstp));
}

static attribute_res *
_mkdir_1 (mkdir_1_argument *argp, struct svc_req *rqstp)
{
	return (mkdir_1_svc(argp->arg1, argp->arg2, argp->arg3, rqstp));
}

static attribute_res *
_mknod_1 (mknod_1_argument *argp, struct svc_req *rqstp)
{
	return (mknod_1_svc(argp->arg1, argp->arg2, argp->arg3, rqstp));
}

static buffer_res *
_read_1 (read_1_argument *argp, struct svc_req *rqstp)
{
	return (read_1_svc(argp->arg1, argp->arg2, argp->arg3, rqstp));
}

static readdir_res *
_readdir_1 (long  *argp, struct svc_req *rqstp)
{
	return (readdir_1_svc(*argp, rqstp));
}

static string_res *
_readlink_1 (long  *argp, struct svc_req *rqstp)
{
	return (readlink_1_svc(*argp, rqstp));
}

static attribute_res *
_setattr_1 (setattr_1_argument *argp, struct svc_req *rqstp)
{
	return (setattr_1_svc(argp->arg1, argp->arg2, argp->arg3, rqstp));
}

static attribute_res *
_symlink_1 (symlink_1_argument *argp, struct svc_req *rqstp)
{
	return (symlink_1_svc(argp->arg1, argp->arg2, argp->arg3, rqstp));
}

static error_code *
_unlink_1 (unlink_1_argument *argp, struct svc_req *rqstp)
{
	return (unlink_1_svc(argp->arg1, argp->arg2, rqstp));
}

static error_code *
_write_1 (write_1_argument *argp, struct svc_req *rqstp)
{
	return (write_1_svc(argp->arg1, argp->arg2, argp->arg3, argp->arg4, rqstp));
}

static error_code *
_copy_1 (copy_1_argument *argp, struct svc_req *rqstp)
{
	return (copy_1_svc(argp->arg1, argp->arg2, argp->arg3, argp->arg4, argp->arg5, rqstp));
}

static void
fs454_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		allocate_1_argument allocate_1_arg;
		long getattr_1_arg;
		link_1_argument link_1_arg;
		lookup_1_argument lookup_1_arg;
		mkdir_1_argument mkdir_1_arg;
		mknod_1_argument mknod_1_arg;
		read_1_argument read_1_arg;
		long readdir_1_arg;
		long readlink_1_arg;
		setattr_1_argument setattr_1_arg;
		symlink_1_argument symlink_1_arg;
		unlink_1_argument unlink_1_arg;
		write_1_argument write_1_arg;
		copy_1_argument copy_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case ALLOCATE:
		_xdr_argument = (xdrproc_t) xdr_allocate_1_argument;
		_xdr_result = (xdrproc_t) xdr_error_code;
		local = (char *(*)(char *, struct svc_req *)) _allocate_1;
		break;

	case GETATTR:
		_xdr_argument = (xdrproc_t) xdr_long;
		_xdr_result = (xdrproc_t) xdr_attribute_res;
		local = (char *(*)(char *, struct svc_req *)) _getattr_1;
		break;

	case LINK:
		_xdr_argument = (xdrproc_t) xdr_link_1_argument;
		_xdr_result = (xdrproc_t) xdr_attribute_res;
		local = (char *(*)(char *, struct svc_req *)) _link_1;
		break;

	case LOOKUP:
		_xdr_argument = (xdrproc_t) xdr_lookup_1_argument;
		_xdr_result = (xdrproc_t) xdr_attribute_res;
		local = (char *(*)(char *, struct svc_req *)) _lookup_1;
		break;

	case MKDIR:
		_xdr_argument = (xdrproc_t) xdr_mkdir_1_argument;
		_xdr_result = (xdrproc_t) xdr_attribute_res;
		local = (char *(*)(char *, struct svc_req *)) _mkdir_1;
		break;

	case MKNOD:
		_xdr_argument = (xdrproc_t) xdr_mknod_1_argument;
		_xdr_result = (xdrproc_t) xdr_attribute_res;
		local = (char *(*)(char *, struct svc_req *)) _mknod_1;
		break;

	case READ:
		_xdr_argument = (xdrproc_t) xdr_read_1_argument;
		_xdr_result = (xdrproc_t) xdr_buffer_res;
		local = (char *(*)(char *, struct svc_req *)) _read_1;
		break;

	case READDIR:
		_xdr_argument = (xdrproc_t) xdr_long;
		_xdr_result = (xdrproc_t) xdr_readdir_res;
		local = (char *(*)(char *, struct svc_req *)) _readdir_1;
		break;

	case READLINK:
		_xdr_argument = (xdrproc_t) xdr_long;
		_xdr_result = (xdrproc_t) xdr_string_res;
		local = (char *(*)(char *, struct svc_req *)) _readlink_1;
		break;

	case SETATTR:
		_xdr_argument = (xdrproc_t) xdr_setattr_1_argument;
		_xdr_result = (xdrproc_t) xdr_attribute_res;
		local = (char *(*)(char *, struct svc_req *)) _setattr_1;
		break;

	case SYMLINK:
		_xdr_argument = (xdrproc_t) xdr_symlink_1_argument;
		_xdr_result = (xdrproc_t) xdr_attribute_res;
		local = (char *(*)(char *, struct svc_req *)) _symlink_1;
		break;

	case UNLINK:
		_xdr_argument = (xdrproc_t) xdr_unlink_1_argument;
		_xdr_result = (xdrproc_t) xdr_error_code;
		local = (char *(*)(char *, struct svc_req *)) _unlink_1;
		break;

	case WRITE:
		_xdr_argument = (xdrproc_t) xdr_write_1_argument;
		_xdr_result = (xdrproc_t) xdr_error_code;
		local = (char *(*)(char *, struct svc_req *)) _write_1;
		break;

	case COPY:
		_xdr_argument = (xdrproc_t) xdr_copy_1_argument;
		_xdr_result = (xdrproc_t) xdr_error_code;
		local = (char *(*)(char *, struct svc_req *)) _copy_1;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	inodes = inode_tree_new();
	directories = file_tree_new();

	inode_tree_insert_root(inodes);
	file_tree_insert_root(directories);
	fsio_init();

	pmap_unset (FS454, MESSAGEVERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, FS454, MESSAGEVERS, fs454_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (FS454, MESSAGEVERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, FS454, MESSAGEVERS, fs454_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (FS454, MESSAGEVERS, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
