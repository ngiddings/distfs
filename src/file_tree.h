#ifndef FS454_FILE_TREE
#define FS454_FILE_TREE

typedef struct directory_entry_t {
    long inode;
    long parent;
    char *name;
} directory_entry;

typedef struct dirent_list_node_t {
    struct dirent_list_node_t *next;
    directory_entry entry;
} dirent_list_node;

typedef struct dirent_list_t {
    dirent_list_node *first, *last;
    long count;
} dirent_list;

typedef struct directory_list_node_t {
    struct directory_list_node_t *next;
    directory_entry self;
    dirent_list directory;
} directory_list_node;

typedef struct directory_list_t {
    directory_list_node *first, *last;
    long count;
} directory_list;

directory_list *file_tree_new();

directory_entry *file_tree_lookup(directory_list *list, long parent, const char *name);

directory_list_node *file_tree_list_dir(directory_list *list, long directory);

int file_tree_insert_root(directory_list *list);

int file_tree_insert(directory_list *list, long parent, const char *name, long inode);

int file_tree_insert_dir(directory_list *list, long parent, const char *name, long inode);

int file_tree_remove(directory_list *list, long parent, const char *name);

#endif