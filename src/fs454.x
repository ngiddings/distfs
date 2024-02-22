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

struct dir_entry {
    char name<>;
    struct file_attributes attributes;
    struct dir_entry *next;
};

enum error_code {
    FS454_ENONE = 0,
    FS454_ENOINODE,
    FS454_EINVAL,
    FS454_EIO,
    FS454_EBADF,
    FS454_EACCESS,
    FS454_ENOTDIR,
    FS454_ENOTEMPTY
};

typedef struct dir_entry *entry_list;

typedef unsigned char byte_buffer<>;

typedef struct file_attributes file_attributes;

union attribute_res switch(error_code err) {
case FS454_ENONE:
    file_attributes attributes;
default:
    void;
};

union string_res switch(error_code err) {
case FS454_ENONE:
    string str<>;
default:
    void;
};

union readdir_res switch(error_code err) {
case FS454_ENONE:
    entry_list entries;
default:
    void;
};

union buffer_res switch(error_code err) {
case FS454_ENONE:
    byte_buffer buffer;
default:
    void;
};

program FS454 {
    version MESSAGEVERS {
            error_code ALLOCATE(long, long) = 1;
            attribute_res GETATTR(long) = 2;
            attribute_res LINK(long, string, long) = 3;
            attribute_res LOOKUP(string, long) = 4;
            attribute_res MKDIR(string, long, long) = 5;
            attribute_res MKNOD(string, long, long) = 6;
            buffer_res READ(long, long, long) = 7;
            readdir_res READDIR(long) = 8;
            string_res READLINK(long) = 9;
            attribute_res SETATTR(long, long, struct file_attributes) = 10;
            attribute_res SYMLINK(string, long, string) = 11;
            error_code UNLINK(long, string) = 12;
            error_code WRITE(long, byte_buffer, long, long) = 13;
            error_code COPY(long, long, long, long, long) = 14;
    } = 1;
} = 0x33764824;