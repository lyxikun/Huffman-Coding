//
// Created by lench on 24-9-13.
//

#include "huffmantable.h"


struct huffmantree_node* addnode() {
    struct huffmantree_node* node = (struct huffmantree_node *)malloc(sizeof(struct huffmantree_node));
    return node;
}

void delnode(struct huffmantree_node* pnode) {
    free(pnode);
}

// free mem.
void free_huffmantree_node(struct huffmantree_node* pnode) {
    if(pnode->right != NULL) {
        free_huffmantree_node(pnode->right);
        free(pnode->right);
    }
    if(pnode->left != NULL) {
        free_huffmantree_node(pnode->left);
        free(pnode->left);
    }
}

void free_huffmancode(struct huffmancode* pcode) {
    if(pcode->prev != NULL) {
        free_huffmancode(pcode->prev);
        free(pcode->prev);
    }
}

// find the minimum propability of nodes.
struct huffmantree_node* findmin(struct huffmantree_node* plist) {
    struct huffmantree_node* minnode = plist;
    struct huffmantree_node* position = plist;
    while(position != NULL) {
        // make sure the right node of the root node is a character instead of a child node.
        if(position->character == -1) {
            if(position->probability <= minnode->probability) {
                minnode = position;
            }
        }
        else {
            if(position->probability < minnode->probability) {
                minnode = position;
            }
        }
        position = position->prev;
    }
    if(minnode->prev != NULL) {
        minnode->prev->next = minnode->next;
    }
    if(minnode->next != NULL) {
        minnode->next->prev = minnode->prev;
    }
    minnode->next = NULL;
    minnode->prev = NULL;
    return minnode;
}

// find the pointer of the char.
struct huffmantree_node *findchar(struct huffmantree_node *pnode, char character, struct huffmantree_node *pcharnode) {
    if(pnode->character != character) {
        if(pnode->right != NULL) {
            pcharnode = findchar(pnode->right, character, pcharnode);
            if(pnode->left != NULL) {
                pcharnode = findchar(pnode->left, character, pcharnode);
            }
        }
    }
    if(pnode->character == character) {
        pcharnode = pnode;
    }
    return pcharnode;
}

// create huffmantree.
struct huffmantree_node* createhuffmantree(struct huffmantree_node* ptempnode) {
    // create root node.
    struct huffmantree_node* temproot = (struct huffmantree_node*)malloc(sizeof(struct huffmantree_node));
    memset(temproot, 0, sizeof(struct huffmantree_node));
    temproot->right = findmin(ptempnode->prev);
    temproot->right->parent = temproot;
    temproot->left = findmin(ptempnode->prev);
    temproot->left->parent = temproot;
    temproot->character = -1;

    //set position pointer.
    struct huffmantree_node* position = ptempnode;
    // goto the last element of the chain.
    while(position->prev != NULL) {
        position = position->prev;
    }
    // link the root node into bottom of the chain.
    temproot->next = position;
    position->prev = temproot;
    // set root node probability.
    temproot->probability = temproot->left->probability + temproot->right->probability;

    // goto next turn.
    if(temproot->probability - 1.0f < 0.0f) {
        return createhuffmantree(ptempnode);
    }
    return temproot;
}

//create huffmantable.
void createhuffmantable(struct huffmancode** phufftable, probabilitylist* pptable,
    struct huffmantree_node* root, int pptablesize) {
    for(int i = 0; i < pptablesize; i++) {
        struct huffmantree_node* pcharnode = NULL;
        pcharnode = findchar(root, pptable[i].character, pcharnode);
        phufftable[i] = (struct huffmancode*)malloc(sizeof(struct huffmancode));
        memset(phufftable[i], 0, sizeof(struct huffmancode));
        while(true) {
            struct huffmancode* tempcode = (struct huffmancode*)malloc(sizeof(struct huffmancode));
            memset(tempcode, 0, sizeof(struct huffmancode));
            // let left node is 0.
            if(pcharnode->parent->left == pcharnode) {
                phufftable[i]->code = '0';
            }
            // let right node is 1.
            if(pcharnode->parent->right == pcharnode) {
                phufftable[i]->code = '1';
            }
            phufftable[i]->next = tempcode;
            tempcode->prev = phufftable[i];
            phufftable[i] =  phufftable[i]->next;
            pcharnode = pcharnode->parent;
            if(pcharnode == root) {
                break;
            }
        }
        phufftable[i]->code = pptable[i].character;
    }
}

void huffencode(char* pchararcter, struct huffmancode** phuffmantable, int charlength) {
    FILE *fw = fopen("../huffmanencode.txt", "w");
    for(int i = 0; i < charlength; i++) {
        // case conversion.
        if(pchararcter[i] >= 65 && pchararcter[i] <=90) {
            pchararcter[i] = pchararcter[i] + 32;
        }
        // replace unknown characters.
        if((pchararcter[i] < 97 || pchararcter[i] > 122) && pchararcter[i] != '\0') {
            pchararcter[i] = ' ';
        }
        if(pchararcter[i] != '\0') {
            for(int j = 0; j < HUFFMANLISTSIZE; j++) {
                if(pchararcter[i] == phuffmantable[j]->code) {
                    struct huffmancode* position = phuffmantable[j];
                    int length = 0;
                    length = showlength(phuffmantable[j], length);
                    for(int k = 0; k < length; k++) {
                        if(position->prev != NULL) {
                            fprintf(fw, "%s", &position->prev->code);
                            position = position->prev;
                        }
                        else {
                            fprintf(fw, "%s", &position->code);
                        }
                    }
                }
            }
        }
    }
    fclose(fw);
}

void huffdecode(struct huffmantree_node* phufftree) {
    FILE *fw = fopen("../huffmandecode.txt", "w");
    FILE* fr = fopen("../huffmanencode.txt", "r");
    if(fr != NULL) {
        char character;
        while(feof(fr) == 0) {
            struct huffmantree_node* position = phufftree;
            while(position->left != NULL && position->right != NULL || character == '\n') {
                fscanf(fr, "%c", &character);
                if(character == '\n') {
                    break;
                }
                if(character == '0') {
                    position = position->left;
                }
                else if(character == '1') {
                    position = position->right;
                }
            }
            if(character != '\n') {
                fprintf(fw, "%s", &position->character);
            }
        }
    }
    fclose(fr);
    fclose(fw);
}

// get huffmancode length.
int showlength(struct huffmancode* pcode, int length) {
    if(pcode->prev != NULL) {
        length++;
        return showlength(pcode->prev, length);
    }
    return length;
}

// show huffmantable.
void showtable(struct huffmancode* pcode) {
    if(pcode->prev != NULL) {
        if(pcode->next == NULL) {
            printf("%s: ", &pcode->code);
        }
        else {
            printf("%s", &pcode->code);
        }
        showtable(pcode->prev);
    }
    if(pcode->prev == NULL) {
        printf("%s", &pcode->code);
        printf("\n");
    }
}