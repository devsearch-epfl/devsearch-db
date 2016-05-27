//
// Created by Matthieu Rudelle on 27/05/16.
//
#include <iostream>
#include "FeatureStore.h"

FeatureStore::FeatureStore(int initSize) {
    maxSize = initSize;
    size = 0;
    db = new db_feature[initSize];

    HS_end = 0;
}

void FeatureStore::add(int feature, int file, float reporank, int line) {
    if (size < maxSize) {
        db[size] = {feature, file, line, reporank};
        size++;
    }
}

void FeatureStore::sort() {
    HS_sort();
}

void FeatureStore::print() {
    for (int i = 0; i < size; i++) {
        printf("%d\t%d\t%d\t%f\n", db[i].feature_id, db[i].file_id, db[i].line_nb, db[i].reporank);
    }
}


/*
 * Parts relative to heap sorting
 */

int FeatureStore::HS_parent(int i) {
    return (i-1)/2;
}

int FeatureStore::HS_firstChild(int i) {
    return 2*i +1;
}

int FeatureStore::HS_secondChild(int i) {
    return 2*i +2;
}

void FeatureStore::HS_siftUp(int i) {
    if (i <= 0) {
        return;
    }

    int p = HS_parent(i);

    if (HS_isBefore(p, i)) {
        HS_swap(p, i);
        HS_siftUp(p);
    }
}

void FeatureStore::HS_siftDown(int i) {
    int fc = HS_firstChild(i);
    int sc = HS_secondChild(i);

    if (fc >= HS_end) {
        return;
    }

//    Only the first child needs to be compared
    if (sc >= HS_end) {

        if (HS_isBefore(i, fc)) {
            HS_swap(i, fc);
            HS_siftDown(fc);
        }

        return;
    }

//    First and second children have to be compared now

//    children are in order
    if (HS_isBefore(fc, i) && HS_isBefore(sc, i)) {
        return;
    }

//    one child needs to step up
    if (HS_isBefore(sc, fc)) {
        HS_swap(fc, i);
        HS_siftDown(fc);
    } else {
        HS_swap(sc, i);
        HS_siftDown(sc);
    }
}

void FeatureStore::HS_heapify() {
    for (int i = HS_end/2; i < HS_end; i++) {
        HS_siftUp(i);
    }
}

void FeatureStore::HS_swap(int i, int j) {
    db_feature temp = db[i];
    db[i] = db[j];
    db[j] = temp;
}

bool FeatureStore::HS_isBefore(int i, int j) {
    if(db[i].feature_id < db[j].feature_id) {
        return true;
    } else if (db[i].feature_id == db[j].feature_id) {
        return db[i].file_id < db[j].file_id;
    } else {
        return false;
    }
}

void FeatureStore::HS_sort() {
    HS_end = size;

    HS_heapify();

    while (HS_end > 0) {
        HS_end--;

        HS_swap(0, HS_end);

        HS_siftDown(0);
    }
}
