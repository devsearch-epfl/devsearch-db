//
// Created by Matthieu Rudelle on 27/05/16.
//

#ifndef DEVSEARCH_DB_QUERYPROCESSOR_H
#define DEVSEARCH_DB_QUERYPROCESSOR_H

#include <string>
#include <queue>
#include "FeatureStore.h"
#include "QueryAggregator.h"

using namespace std;

// sort aggregated results per score (worst results first as they need to be replaced by better score)
struct LessThanByScore
{
    bool operator()(const agg_result* lhs, const agg_result* rhs) const
    {
        return lhs->score > rhs->score;
    }
};

typedef std::priority_queue<agg_result*, std::vector<agg_result*>, LessThanByScore> max_heap_t;

class QueryProcessor {
    FeatureIndex* index;
    FeatureStore* store;

    void insertMaxHeap(agg_result* in, int maxSize, max_heap_t* maxHeap);
public:
    QueryProcessor(FeatureStore* str, FeatureIndex* idx);
    void process(int qLength, int* q, int take, int from, vector<agg_result>* out, int* matchedFeatures, int* matchedFiles);
};


#endif //DEVSEARCH_DB_QUERYPROCESSOR_H
