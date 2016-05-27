//
// Created by Matthieu Rudelle on 26/05/16.
//

#include "StringDict.h"
#include <string>
#include <iostream>

using namespace std;

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
        char ordinal = text.at((unsigned long) strPos);

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
        char ordinal = text.at((unsigned long) strPos);

        if (!current->children[ordinal]) {
            return -1;
        }

        current = current->children[ordinal];
        strPos++;
    }

    return current->key;
}

void StringDict::getBatchKeys(int size, string* texts, int* keysOut) {
    for (int i = 0; i < size; ++i) {
        keysOut[i] = getKey(texts[i]);
    }
}

int StringDict::generateKey() {
    size++;
    return tempId++;
}

void StringDict::print() {
    print("", root);
}

void StringDict::print(string prefix, trie_node* node) {
    if (node->key >= 0) {
        printf("%s[%d]\n", prefix.c_str(), node->key);
    }

    for (child_map::iterator it=node->children.begin(); it!=node->children.end(); ++it) {

        string newPrefix = prefix;
        newPrefix += it->first;

        print(newPrefix, it->second);
    }
}

int StringDict::nodeCount() {
    return nodeCount(root);
}

int StringDict::nodeCount(trie_node* node) {
    int count = 0;

    for (child_map::iterator it=node->children.begin(); it!=node->children.end(); ++it) {

        count += nodeCount(it->second);
    }

    count+=node->children.size();

    return count;
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

    for (child_map::iterator it=node->children.begin(); it!=node->children.end(); ++it) {
        idStart = sort(translation, it->second, idStart);
    }

    return idStart;
}


