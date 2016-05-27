//
// Created by Matthieu Rudelle on 26/05/16.
//

#include "StringDict.h"
#include <string>
#include <iostream>

using namespace std;

#define CHILDRENS_LENGTH 128

StringDict::StringDict() {
    tempId = 0;
    size = 0;

    // root element has no key
    root = new trie_node();
    root->key = -1;
}

int StringDict::add(string text) {
    
    int strPos = 0;
    
    trie_node *current = root;

    while(strPos < text.length()) {
        int ordinal = charToAddress(text.at((unsigned long) strPos));

        if (!current->children[ordinal]) {

            current->children[ordinal] = new trie_node();

//            non leaf nodes are not assigned to a key
            current->children[ordinal]->key = -1;
        }

        current = current->children[ordinal];
        strPos++;
    }

    if (current->key < 0) {
        current->key = generateKey();
    }

    return current->key;
}

// returns the key or -1 if there is no key for this string
int StringDict::getKey(string text) {

    int strPos = 0;

    trie_node* current = root;

    while(strPos < text.length()) {
        int ordinal = charToAddress(text.at((unsigned long) strPos));

        if (!current->children[ordinal]) {
            return -1;
        }

        current = current->children[ordinal];
        strPos++;
    }

    return current->key;
}

int StringDict::generateKey() {
    size++;
    return tempId++;
}

int StringDict::charToAddress(char &c) {
    int ord = int(c);

    if (ord>=CHILDRENS_LENGTH) {
        ord = 0;
    }

    return ord;
}

void StringDict::print() {
    print("", root);
}

void StringDict::print(string prefix, trie_node* node) {
    if (node->key >= 0) {
        printf("%s[%d]\n", prefix.c_str(), node->key);
    }

    for(int i = 0; i < CHILDRENS_LENGTH; i++) {


        if (node->children[i]) {

            string newPrefix = prefix;
            newPrefix += i;

            print(newPrefix, node->children[i]);
        }
    }
}

int StringDict::nodeCount() {
    return nodeCount(root);
}

int StringDict::nodeCount(trie_node* node) {
    int count = 0;

    for(int i = 0; i < CHILDRENS_LENGTH; i++) {

        if (node->children[i]) {

            count += nodeCount(node->children[i]);
        }
    }

    return count+1;
}

void StringDict::sort(int* translation) {
    sort(translation, root, 0);
}

int StringDict::sort(int* translation, trie_node* node, int idStart) {
    if (node->key >= 0) {

        translation[node->key] = idStart;
        node->key = idStart;

        idStart++;
    }

    for(int i = 0; i < CHILDRENS_LENGTH; i++) {

        if (node->children[i]) {

            idStart = sort(translation, node->children[i], idStart);
        }
    }

    return idStart;
}


