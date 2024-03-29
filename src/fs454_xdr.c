/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "fs454.h"

bool_t
xdr_file_attributes (XDR *xdrs, file_attributes *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, 8 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_long (xdrs, &objp->id))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->link_count))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->filesize))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->owner))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->mode))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->last_access))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->last_modify))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->last_change))
				 return FALSE;
		} else {
			IXDR_PUT_LONG(buf, objp->id);
			IXDR_PUT_LONG(buf, objp->link_count);
			IXDR_PUT_LONG(buf, objp->filesize);
			IXDR_PUT_LONG(buf, objp->owner);
			IXDR_PUT_LONG(buf, objp->mode);
			IXDR_PUT_LONG(buf, objp->last_access);
			IXDR_PUT_LONG(buf, objp->last_modify);
			IXDR_PUT_LONG(buf, objp->last_change);
		}
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, 8 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_long (xdrs, &objp->id))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->link_count))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->filesize))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->owner))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->mode))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->last_access))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->last_modify))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->last_change))
				 return FALSE;
		} else {
			objp->id = IXDR_GET_LONG(buf);
			objp->link_count = IXDR_GET_LONG(buf);
			objp->filesize = IXDR_GET_LONG(buf);
			objp->owner = IXDR_GET_LONG(buf);
			objp->mode = IXDR_GET_LONG(buf);
			objp->last_access = IXDR_GET_LONG(buf);
			objp->last_modify = IXDR_GET_LONG(buf);
			objp->last_change = IXDR_GET_LONG(buf);
		}
	 return TRUE;
	}

	 if (!xdr_long (xdrs, &objp->id))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->link_count))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->filesize))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->owner))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->mode))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->last_access))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->last_modify))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->last_change))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_dir_entry (XDR *xdrs, dir_entry *objp)
{
	register int32_t *buf;

	 if (!xdr_array (xdrs, (char **)&objp->name.name_val, (u_int *) &objp->name.name_len, ~0,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_file_attributes (xdrs, &objp->attributes))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->next, sizeof (dir_entry), (xdrproc_t) xdr_dir_entry))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_error_code (XDR *xdrs, error_code *objp)
{
	register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_entry_list (XDR *xdrs, entry_list *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (dir_entry), (xdrproc_t) xdr_dir_entry))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_byte_buffer (XDR *xdrs, byte_buffer *objp)
{
	register int32_t *buf;

	 if (!xdr_array (xdrs, (char **)&objp->byte_buffer_val, (u_int *) &objp->byte_buffer_len, ~0,
		sizeof (u_char), (xdrproc_t) xdr_u_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_attribute_res (XDR *xdrs, attribute_res *objp)
{
	register int32_t *buf;

	 if (!xdr_error_code (xdrs, &objp->err))
		 return FALSE;
	switch (objp->err) {
	case FS454_ENONE:
		 if (!xdr_file_attributes (xdrs, &objp->attribute_res_u.attributes))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_string_res (XDR *xdrs, string_res *objp)
{
	register int32_t *buf;

	 if (!xdr_error_code (xdrs, &objp->err))
		 return FALSE;
	switch (objp->err) {
	case FS454_ENONE:
		 if (!xdr_string (xdrs, &objp->string_res_u.str, ~0))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_readdir_res (XDR *xdrs, readdir_res *objp)
{
	register int32_t *buf;

	 if (!xdr_error_code (xdrs, &objp->err))
		 return FALSE;
	switch (objp->err) {
	case FS454_ENONE:
		 if (!xdr_entry_list (xdrs, &objp->readdir_res_u.entries))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_buffer_res (XDR *xdrs, buffer_res *objp)
{
	register int32_t *buf;

	 if (!xdr_error_code (xdrs, &objp->err))
		 return FALSE;
	switch (objp->err) {
	case FS454_ENONE:
		 if (!xdr_byte_buffer (xdrs, &objp->buffer_res_u.buffer))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_allocate_1_argument (XDR *xdrs, allocate_1_argument *objp)
{
	 if (!xdr_long (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_link_1_argument (XDR *xdrs, link_1_argument *objp)
{
	 if (!xdr_long (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->arg2, ~0))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg3))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_lookup_1_argument (XDR *xdrs, lookup_1_argument *objp)
{
	 if (!xdr_string (xdrs, &objp->arg1, ~0))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mkdir_1_argument (XDR *xdrs, mkdir_1_argument *objp)
{
	 if (!xdr_string (xdrs, &objp->arg1, ~0))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg2))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg3))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mknod_1_argument (XDR *xdrs, mknod_1_argument *objp)
{
	 if (!xdr_string (xdrs, &objp->arg1, ~0))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg2))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg3))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_read_1_argument (XDR *xdrs, read_1_argument *objp)
{
	 if (!xdr_long (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg2))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg3))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_setattr_1_argument (XDR *xdrs, setattr_1_argument *objp)
{
	 if (!xdr_long (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg2))
		 return FALSE;
	 if (!xdr_file_attributes (xdrs, &objp->arg3))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_symlink_1_argument (XDR *xdrs, symlink_1_argument *objp)
{
	 if (!xdr_string (xdrs, &objp->arg1, ~0))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg2))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->arg3, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_unlink_1_argument (XDR *xdrs, unlink_1_argument *objp)
{
	 if (!xdr_long (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->arg2, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_write_1_argument (XDR *xdrs, write_1_argument *objp)
{
	 if (!xdr_long (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_byte_buffer (xdrs, &objp->arg2))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg3))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg4))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_copy_1_argument (XDR *xdrs, copy_1_argument *objp)
{
	 if (!xdr_long (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg2))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg3))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg4))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->arg5))
		 return FALSE;
	return TRUE;
}
