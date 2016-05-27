//
// Created by Matthieu Rudelle on 26/05/16.
//

#ifndef DEVSEARCH_DB_STRINGDICT_H
#define DEVSEARCH_DB_STRINGDICT_H

#include <string>
using namespace std;

#define CHILDRENS_LENGTH 128

struct trie_node {
    int key;
    trie_node *children[CHILDRENS_LENGTH];
};

class StringDict {
private:
    trie_node *root;

    int tempId;

    int generateKey();

public:
    StringDict();

    int add(string text);

    int size;

    int charToAddress(char &at);

    int getKey(string text);

    void print();

    void print(string prefix, trie_node* node);

    int nodeCount();

    int nodeCount(trie_node *node);

    void sort(int *translation);

    int sort(int *translation, trie_node *node, int idStart);
};


#endif //DEVSEARCH_DB_STRINGDICT_H
