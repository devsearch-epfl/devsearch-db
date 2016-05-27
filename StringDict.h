//
// Created by Matthieu Rudelle on 26/05/16.
//

#ifndef DEVSEARCH_DB_STRINGDICT_H
#define DEVSEARCH_DB_STRINGDICT_H

#include <string>
#include <map>
using namespace std;

struct trie_node;

typedef std::map<char, trie_node*> child_map;

struct trie_node {
    int key;
    child_map children;
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

    int getKey(string text);

    void print();

    void print(string prefix, trie_node* node);

    int nodeCount();

    int nodeCount(trie_node *node);

    void sort(int *translation);

    int sort(int *translation, trie_node *node, int idStart);
};


#endif //DEVSEARCH_DB_STRINGDICT_H
