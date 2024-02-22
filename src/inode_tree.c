#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include "inode_tree.h"

inode_tree *inode_tree_new() {
    inode_tree *tree = malloc(sizeof(inode_tree));
    if(tree != NULL) {
        tree->first = NULL;
        tree->last = NULL;
        tree->count = 0;
        tree->next_inode = 2;
    }
    return tree;
}

file_descriptor *inode_tree_create(inode_tree *tree) {
    inode_tree_node *newnode = malloc(sizeof(inode_tree_node));
    newnode->descriptor = malloc(sizeof(file_descriptor));
    newnode->next = NULL;
    newnode->descriptor->location = 0;
    newnode->descriptor->attributes.id = tree->next_inode;
    newnode->descriptor->attributes.link_count = 1;
    newnode->descriptor->attributes.owner = 0;
    newnode->descriptor->attributes.mode = 0;
    newnode->descriptor->attributes.last_access = time(0);
    newnode->descriptor->attributes.last_modify = time(0);
    newnode->descriptor->attributes.last_change = time(0);
    if(tree->last != NULL) {
        tree->last->next = newnode;
        tree->last = newnode;
    } else {
        tree->first = newnode;
        tree->last = newnode;
    }
    tree->count++;
    tree->next_inode++;
    return newnode->descriptor;
}

file_descriptor *inode_tree_find(inode_tree *tree, long inode) {
    inode_tree_node *node = tree->first;
    while(node != NULL) {
        if(node->descriptor->attributes.id == inode) {
            break;
        } else {
            node = node->next;
        }
    }
    if(node != NULL) {
        return node->descriptor;
    } else {
        return NULL;
    }
}

int inode_tree_insert_root(inode_tree *tree) {
    file_descriptor *root = malloc(sizeof(file_descriptor));
    root->location = 0;
    root->attributes.id = 1;
    root->attributes.filesize = 0;
    root->attributes.last_access = time(0);
    root->attributes.last_change = time(0);
    root->attributes.last_modify = time(0);
    root->attributes.owner = 0;
    root->attributes.mode = __S_IFDIR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    return inode_tree_insert(tree, root);
}

int inode_tree_insert(inode_tree *tree, file_descriptor *descriptor) {
    inode_tree_node *node = malloc(sizeof(inode_tree_node));
    if(node != NULL) {
        node->next = NULL;
        node->descriptor = descriptor;
        if(tree->last != NULL) {
            tree->last->next = node;
            tree->last = node;
        } else {
            tree->first = node;
            tree->last = node;
        }
        tree->count++;
        return 0;
    } else {
        return -1;
    }
}

int inode_tree_remove(inode_tree *tree, long inode) {
    inode_tree_node *prev = NULL;
    inode_tree_node *node = tree->first;
    while(node != NULL) {
        if(node->descriptor->attributes.id == inode) {
            break;
        } else {
            prev = node;
            node = node->next;
        }
    }
    if(node != NULL) {
        if(prev != NULL) {
            prev->next = node->next;
            if(prev->next == NULL) {
                tree->last = prev;
            }
        } else {
            if(tree->first == tree->last) {
                tree->first = tree->last = NULL;
            } else {
                tree->first = node->next;
            }
        }
        free(node->descriptor);
        free(node);
        tree->count--;
        return 0;
    } else {
        return -1;
    }
}