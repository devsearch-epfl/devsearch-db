//
// Created by Matthieu Rudelle on 27/05/16.
//
// NOTE: Requires feature indices to be continuous and to start at 0!
//

#include "FeatureIndex.h"

FeatureIndex::FeatureIndex(int distinctFeatureCount, int dbSize) {
    dbSize = dbSize;
    size = distinctFeatureCount;
    index = new int[distinctFeatureCount];
}

void FeatureIndex::build_addStart(int feature_id, int start) {
    index[feature_id] = start;
}

void FeatureIndex::getRange(int feature_id, int *start, int *end) {
    if (feature_id < size) {
        *start = index[feature_id];

//        if this is the last index the end position will be the bottom of the db
        *end = (feature_id == size-1)? dbSize: index[feature_id+1];
    }
}





