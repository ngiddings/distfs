#include "fsio.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

long next_location;

void get_filename(char *dest, long location) {
    mkdir("data/", 0700);
    sprintf(dest, "data/%ld", location);
}

void fsio_init() {
    next_location = 1;
}

long do_mknod() {
    long location = next_location;
    next_location++;
    char filename[80];
    get_filename(filename, location);
    FILE *fd = fopen(filename, "wx");
    if(fd != NULL) {
        fclose(fd);
        return location;
    } else {
        return 0;
    }
}

int do_rmnod(long location) {
    char filename[80];
    get_filename(filename, location);
    remove(filename);
    return 0;
}

int do_read(long location, unsigned char *buffer, long offset, long length) {
    printf("do_read %i %i %i %p\n", location, length, offset, buffer);
    char filename[80];
    get_filename(filename, location);
    int fd = open(filename, O_RDONLY);
    if(fd > 0) {
        lseek(fd, offset, SEEK_SET);
        read(fd, buffer, length);
        close(fd);
        return 0;
    } else {
        return -1;
    }
}

int do_write(long location, const unsigned char *buffer, long offset, long length) {
    printf("do_write %i %i %i %p\n", location, length, offset, buffer);
    char filename[80];
    get_filename(filename, location);
    int fd = open(filename, O_WRONLY);
    if(fd > 0) {
        lseek(fd, offset, SEEK_SET);
        write(fd, buffer, length);
        close(fd);
        return 0;
    } else {
        return -1;
    }
}

int do_allocate(long location, long size) {
    char filename[80];
    get_filename(filename, location);
    int fd = open(filename, O_RDWR);
    posix_fallocate(fd, 0, size);
    close(fd);
    return 0;
}