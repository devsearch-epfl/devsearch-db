//
// Created by Matthieu Rudelle on 27/05/16.
//

#ifndef DEVSEARCH_DB_QUERYPROCESSOR_H
#define DEVSEARCH_DB_QUERYPROCESSOR_H

#include <string>
#include "FeatureStore.h"

using namespace std;

class QueryProcessor {
    FeatureIndex* index;
    FeatureStore* store;
public:
    QueryProcessor(FeatureStore* str, FeatureIndex* idx);
    void process(int qLength, int* q, int take, int from);
};


#endif //DEVSEARCH_DB_QUERYPROCESSOR_H
