#ifndef FS454_INODE_TREE
#define FS454_INODE_TREE

#include "fs454.h"

typedef struct file_descriptor_t {
    struct file_attributes attributes;
    long location;
} file_descriptor;

typedef struct inode_tree_node_t {
    struct inode_tree_node_t *next;
    file_descriptor *descriptor;
} inode_tree_node;

typedef struct inode_tree_t {
    inode_tree_node *first, *last;
    long count;
    long next_inode;
} inode_tree;

inode_tree *inode_tree_new();

file_descriptor *inode_tree_create(inode_tree *tree);

file_descriptor *inode_tree_find(inode_tree *tree, long inode);

int inode_tree_insert_root(inode_tree *tree);

int inode_tree_insert(inode_tree *tree, file_descriptor *descriptor);

int inode_tree_remove(inode_tree *tree, long inode);

#endif