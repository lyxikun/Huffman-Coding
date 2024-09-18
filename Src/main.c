//
// Created by lench on 24-9-13.
//

#include <stdio.h>
#include <math.h>

#include "huffmantable.h"

#define STRINGLENGTH 100

/*
 * test list, you can ignore it.
 */
// static probabilitylist probabilitytable[HUFFMANLISTSIZE] = {
//     {'a', 0.36},    {'b', 0.18},    {'c', 0.18},
//     {'d', 0.09},    {'e', 0.06}, {'f', 0.06},
//     {'g', 0.03},   {'h', 0.03}, {'i', 0.01}
// };

// test string.
//char stringbuffer[STRINGLENGTH] = "Across the Great Wall we can reach every corner of the world";
char stringbuffer[STRINGLENGTH] ;

void shownum(struct huffmantree_node* node);

int main() {
    // entropy of the source.
    float hx = 0.0f;
    float hufflength = 0.0f;
    struct huffmancode* huffmantable[HUFFMANLISTSIZE];
    // set up nodes
    struct huffmantree_node* tempnode = addnode();
    memset(tempnode, 0, sizeof(struct huffmantree_node));
    for(int i = 0; i < HUFFMANLISTSIZE; i++) {
        tempnode->next = addnode();
        tempnode->probability = probabilitytable[i].probability;
        tempnode->character = probabilitytable[i].character;
        tempnode->next->prev = tempnode;
        tempnode = tempnode->next;
    }
    struct huffmantree_node* root = tempnode;

    //set root node.
    root = createhuffmantree(root);

    createhuffmantable(huffmantable, probabilitytable, root, HUFFMANLISTSIZE);

    for(int i = 0; i < HUFFMANLISTSIZE; i++) {
        // print hugffmantable.
        showtable(huffmantable[i]);
        int length = 0;
        length = showlength(huffmantable[i], length);
        hufflength += length * probabilitytable[i].probability;
        // calculate entropy.
        hx += (-1.0f)*probabilitytable[i].probability*log2(probabilitytable[i].probability);
    }

    printf("Entropy of the source: %f\n", hx);
    printf("Average length of the huffmancode: %f\n", hufflength);
    printf("Huffmancode efficiency: %%%.2f\n", hx/hufflength*100.0f);

    printf("Please input string to encode\n");
    fgets(stringbuffer, sizeof(stringbuffer), stdin);

    // encode ascii to huffmancode.
    huffencode(stringbuffer, huffmantable, STRINGLENGTH);
    // decode huffmancode to ascii.
    huffdecode(root);

    // free huffmantree.
    free_huffmantree_node(root);
    // free root node.
    free(root);
    // free temp node.
    free(tempnode);
    // free code table.
    for(int i = 0; i < HUFFMANLISTSIZE; i++) {
        free_huffmancode(huffmantable[i]);
        free(huffmantable[i]);
    }

    return 0;
}

// test func.
void shownum(struct huffmantree_node* node) {
    if(node->right != NULL) {
        shownum(node->right);
        if(node->left != NULL) {
            shownum(node->left);
        }
    }
    if(node->character != '\0') {
        printf("character:%s, %f \n",&node->character, node->probability);
    }
}






