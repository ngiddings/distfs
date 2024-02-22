#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "file_tree.h"

directory_list *file_tree_new() {
    directory_list *list = malloc(sizeof(directory_list));
    if(list != NULL) {
        list->first = NULL;
        list->last = NULL;
        list->count = 0;
    }
    return list;
}

directory_entry *file_tree_lookup(directory_list *list, long parent, const char *name) {
    directory_list_node *node = file_tree_list_dir(list, parent);
    if(node == NULL) {
        return NULL;
    }
    dirent_list_node *dirent_node = node->directory.first;
    while(dirent_node != NULL) {
        if(strcmp(dirent_node->entry.name, name) == 0) {
            break;
        } else {
            dirent_node = dirent_node->next;
        }
    }
    if(dirent_node != NULL) {
        return &dirent_node->entry;
    } else {
        return NULL;
    }
}

directory_list_node *file_tree_list_dir(directory_list *list, long directory) {
    directory_list_node *node = list->first;
    while(node != NULL) {
        if(node->self.inode == directory) {
            break;
        } else {
            node = node->next;
        }
    }
    return node;
}

int file_tree_insert_root(directory_list *list) {
    directory_list_node *newnode = malloc(sizeof(directory_list_node));
    newnode->next = NULL;
    newnode->directory.count = 0;
    newnode->directory.first = NULL;
    newnode->directory.last = NULL;
    newnode->self.inode = 1;
    newnode->self.parent = 0;
    newnode->self.name = malloc(strlen("") + 1);
    newnode->self.name[0] = (char) 0;
    list->first = newnode;
    list->last = newnode;
    list->count = 1;
    return 0;
}

int file_tree_insert(directory_list *list, long parent, const char *name, long inode) {
    directory_list_node *node = file_tree_list_dir(list, parent);
    if(node == NULL) {
        /* Parent directory not found */
        return -1;
    }
    dirent_list_node *newnode = malloc(sizeof(dirent_list_node));
    newnode->next = NULL;
    unsigned int namelen = strlen(name) + 1;
    newnode->entry.name = malloc(namelen);
    strcpy(newnode->entry.name, name);
    newnode->entry.inode = inode;
    newnode->entry.parent = parent;
    if(node->directory.last != NULL) {
        node->directory.last->next = newnode;
        node->directory.last = newnode;
    } else {
        node->directory.first = newnode;
        node->directory.last = newnode;
    }
    node->directory.count++;
    return 0;
}

int file_tree_insert_dir(directory_list *list, long parent, const char *name, long inode) {
    if(file_tree_list_dir(list, inode) != NULL) {
        /* We can't have several hardlinks to the same directory */
        return -1;
    } else if(file_tree_insert(list, parent, name, inode) != 0) {
        /* Parent directory not found */
        return -1;
    }
    directory_list_node *newnode = malloc(sizeof(directory_list_node));
    newnode->next = NULL;
    newnode->self.inode = inode;
    unsigned int namelen = strlen(name) + 1;
    newnode->self.name = malloc(namelen);
    strcpy(newnode->self.name, name);
    newnode->self.parent = parent;
    newnode->directory.first = NULL;
    newnode->directory.last = NULL;
    newnode->directory.count = 0;
    list->last->next = newnode;
    list->last = newnode;
    list->count++;
    return 0;
}

int file_tree_remove(directory_list *list, long parent, const char *name) {
    directory_entry *entry = file_tree_lookup(list, parent, name);
    if(entry == NULL) {
        return -1;
    }
    directory_list_node *dir_list = file_tree_list_dir(list, entry->inode);
    if(dir_list != NULL && dir_list->directory.count > 0) {
        return -1;
    } else if(dir_list != NULL && dir_list->directory.count == 0) {
        directory_list_node *prev = NULL;
        directory_list_node *curr = list->first;
        while(curr != NULL) {
            if(curr->self.inode == entry->inode) {
                break;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
        if(curr != NULL) {
            if(prev != NULL) {
                prev->next = curr->next;
                if(prev->next == NULL) {
                    list->last = prev;
                }
            } else if(list->first == list->last) {
                list->first = list->last = NULL;
            } else {
                list->first = curr->next;
            }
            free(curr->self.name);
            free(curr);
            list->count--;
        } else {
            return -1;
        }
    }
    directory_list_node *parent_list = file_tree_list_dir(list, parent);
    dirent_list_node *prev = NULL;
    dirent_list_node *curr = parent_list->directory.first;
    while (curr != NULL) {
        if (curr->entry.inode == entry->inode){
            break;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    if (curr != NULL) {
        if (prev != NULL) {
            prev->next = curr->next;
            if (prev->next == NULL) {
                parent_list->directory.last = prev;
            }
        } else if (parent_list->directory.first == parent_list->directory.last) {
            parent_list->directory.first = parent_list->directory.last = NULL;
        } else {
            parent_list->directory.first = curr->next;
        }
        free(curr->entry.name);
        free(curr);
        parent_list->directory.count--;
    } else {
        return -1;
    }
    return 0;
}