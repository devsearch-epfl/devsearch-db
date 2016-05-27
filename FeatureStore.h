//
// Created by Matthieu Rudelle on 27/05/16.
//

#ifndef DEVSEARCH_DB_FEATURESTORE_H
#define DEVSEARCH_DB_FEATURESTORE_H


#include "FeatureIndex.h"

struct db_feature {
    int file_id;
    int feature_id;
    int line_nb;
    float reporank;
};


class FeatureStore {
private:
    db_feature* db;
    int maxSize;
    int HS_end;
    bool isSorted;
public:
    int size;
    FeatureStore(int InitSize);

    void add(int feature, int file, float reporank, int line);

    void sort();

    db_feature* at(int pos);

    int HS_parent(int i);

    int HS_firstChild(int i);

    int HS_secondChild(int i);

    void HS_siftUp(int i);

    void HS_siftDown(int i);

    void HS_heapify();

    void HS_swap(int i, int j);

    void HS_sort();

    bool HS_isBefore(int i, int j);

    void print();

    void buildFeatureIndex(FeatureIndex *index);
};


#endif //DEVSEARCH_DB_FEATURESTORE_H
