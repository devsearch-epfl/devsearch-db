//
// Created by Matthieu Rudelle on 27/05/16.
//
#include <iostream>
#include "FeatureStore.h"
#include "FeatureIndex.h"

FeatureStore::FeatureStore(int initSize) {
    maxSize = initSize;
    size = 0;
    db = new db_feature[initSize];

    HS_end = 0;
    isSorted = false;
}

void FeatureStore::add(int feature, int file, float reporank, int line) {
    if (size < maxSize) {
        db[size] = {feature, file, line, reporank};
        size++;
        isSorted = false;
    }
}

void FeatureStore::sort() {
    HS_sort();
    isSorted = true;
}

void FeatureStore::print() {
    for (int i = 0; i < size; i++) {
        printf("%d\t%d\t%d\t%f\n", db[i].feature_id, db[i].file_id, db[i].line_nb, db[i].reporank);
    }
}


void FeatureStore::buildFeatureIndex(FeatureIndex *index) {

//    if data is not sorted yet, will have to do that first
    if (!isSorted) {
        sort();
    }

    int currentFID = -1;

    for (int i = 0; i < size; i++) {

        if (db[i].feature_id != currentFID) {

            currentFID = db[i].feature_id;
            index->build_addStart(currentFID, i);
        }
    }
}

db_feature *FeatureStore::at(int pos) {
    return &db[pos];
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
    for (int i = HS_end/2; i >= 0; i--) {
        HS_siftDown(i);
    }
}

void FeatureStore::HS_swap(int i, int j) {
    db_feature temp = db[i];
    db[i] = db[j];
    db[j] = temp;
}

// sort order is feature_id, file_id, line nb
bool FeatureStore::HS_isBefore(int i, int j) {
    if(db[i].feature_id < db[j].feature_id) {
        return true;
    } else if (db[i].feature_id == db[j].feature_id) {
        if(db[i].file_id < db[j].file_id) {
            return true;
        } else if (db[i].file_id == db[j].file_id) {
            return db[i].line_nb < db[j].line_nb;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void FeatureStore::HS_sort() {
    HS_end = size;

    HS_heapify();

    while (HS_end > 0) {
        HS_end--;

        if (db[0].feature_id < db[1].feature_id || db[0].feature_id < db[2].feature_id) {
            printf("big problem\n");
        }

        HS_swap(0, HS_end);

        HS_siftDown(0);
    }
}


