#ifndef FS454_IO
#define FS454_IO

void fsio_init();

long do_mknod();

int do_rmnod(long location);

int do_read(long location, unsigned char *buffer, long offset, long length);

int do_write(long location, const unsigned char *buffer, long offset, long length);

int do_allocate(long location, long size);

#endif