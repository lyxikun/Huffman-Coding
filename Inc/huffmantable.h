//
// Created by lench on 24-9-13.
//

#ifndef HUFFMANTABLE_H
#define HUFFMANTABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define HUFFMANLISTSIZE 27

typedef struct {
    char character;
    float probability;
} probabilitylist;

struct huffmantree_node {
    float probability;
    char character;
    struct huffmantree_node* next;
    struct huffmantree_node* prev;
    struct huffmantree_node* parent;
    struct huffmantree_node* left;
    struct huffmantree_node* right;
};

struct huffmancode {
    char code;
    struct huffmancode* next;
    struct huffmancode* prev;
};

struct huffmantree_node* addnode();
void delnode(struct huffmantree_node* pnode);
void free_huffmantree_node(struct huffmantree_node* pnode);
void free_huffmancode(struct huffmancode* pcode);

struct huffmantree_node* findmin(struct huffmantree_node* plist);
struct huffmantree_node* findchar(struct huffmantree_node* pnode, char character, struct huffmantree_node* pcharnode);

struct huffmantree_node* createhuffmantree(struct huffmantree_node* ptempnode);
void createhuffmantable(struct huffmancode** phufftable, probabilitylist* pptable,
    struct huffmantree_node* proot, int pptablesize);

void huffencode(char* pchararcter, struct huffmancode** phuffmantable, int charlength);
void huffdecode(struct huffmantree_node* phufftree);

int showlength(struct huffmancode* pcode, int length);
void showtable(struct huffmancode* pcode);

/*
 * list of alphabet and space.
 */
static probabilitylist probabilitytable[HUFFMANLISTSIZE] = {
    {' ', 0.1918182},    {'a', 0.0651738},    {'b', 0.0124248},    {'c', 0.0217339},
    {'d', 0.0349835},    {'e', 0.1041442},    {'f', 0.0197881},    {'g', 0.0158610},
    {'h', 0.0492888},    {'i', 0.0558094},    {'j', 0.0009033},    {'k', 0.0050529},
    {'l', 0.0331490},    {'m', 0.0202124},    {'n', 0.0564513},    {'o', 0.0596302},
    {'p', 0.0137645},    {'q', 0.0008606},    {'r', 0.0497563},    {'s', 0.0515760},
    {'t', 0.0729357},    {'u', 0.0225134},    {'v', 0.0082903},    {'w', 0.0171272},
    {'x', 0.0013692},    {'y', 0.0145984},    {'z', 0.0007836},
};

#endif //HUFFMANTABLE_H


