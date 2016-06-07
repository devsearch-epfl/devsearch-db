//
// Created by Matthieu Rudelle on 07/06/16.
//

#ifndef DEVSEARCH_DB_QUERYAGGREGATOR_H
#define DEVSEARCH_DB_QUERYAGGREGATOR_H

#include <string>
#include <map>
#include <vector>
#include <unordered_set>
#include "FeatureStore.h"

using namespace std;

struct agg_result {
    double score;
    map<string, double>* scoreBreakdown;
    vector<int> lines;
};


class QueryAggregator {
private:
    vector<int> lines;
    unordered_set<int> featureSet;
    int fileId;
    double repoRank;

    int startLine;
    int endLine;

public:
    QueryAggregator(db_feature* feature);
    void addFeature(db_feature* feature);
    void finalize(int qLength, agg_result* out);
};


#endif //DEVSEARCH_DB_QUERYAGGREGATOR_H
