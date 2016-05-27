//
// Created by Matthieu Rudelle on 27/05/16.
//

#ifndef DEVSEARCH_DB_FEATUREINDEX_H
#define DEVSEARCH_DB_FEATUREINDEX_H


class FeatureIndex {
private:
    int dbSize;
    int size;
    int* index;
public:
    FeatureIndex(int length, int dbSize);
    void build_addStart(int feature_id, int start);
    void getRange(int feature_id, int* start, int* end);

    void buildFeatureIndex(FeatureIndex *index);
};


#endif //DEVSEARCH_DB_FEATUREINDEX_H
