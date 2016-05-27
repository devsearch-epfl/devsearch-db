//
// Created by Matthieu Rudelle on 27/05/16.
//

#include "QueryProcessor.h"
#include <queue>
using namespace std;

struct pointer {
    int featureID;
    int currPos;
    int currFileId;
    int end;
    bool exhausted;
};

struct LessThanByFileId
{
    bool operator()(const pointer& lhs, const pointer& rhs) const
    {
        if (lhs.exhausted) {
            return true;
        }

        if (rhs.exhausted) {
            return false;
        }

        return lhs.currFileId > rhs.currFileId;
    }
};

QueryProcessor::QueryProcessor(FeatureStore *str, FeatureIndex *idx) {
    store = str;
    index = idx;
}

void QueryProcessor::process(int qLength, int q[], int take, int from) {

    std::priority_queue<pointer, std::vector<pointer>, LessThanByFileId> pointers;

    int count = 0;

    // build pointers that will perform the query
    for (int i = 0; i < qLength; i++) {
        int start;
        int end;

        index->getRange(q[i], &start, &end);

        printf("feature %d has range %d to %d ... %d elements\n", q[i], start, end, end-start);

        pointers.push({
                q[i],
                start,
                store->at(start)->file_id,
                end,
                (start >= end) // pointer migth be exhausted from start
        });
    }

    pointer currPointer;
    db_feature* currFeature;
     
    int readHead, currFileID;

    while (!pointers.top().exhausted) {

        currPointer = pointers.top();

        currFileID = currPointer.currFileId;
        readHead = currPointer.currPos;

        currFeature = store->at(readHead);

        while(currFeature->file_id == currFileID && currFeature->feature_id == currPointer.featureID) {
            currFeature = store->at(readHead);

            // do something with store->at(readHead)
            count+=1;

//            printf("found feature: %d\t%d\t%d\t%f\n", currFeature->feature_id, currFeature->file_id, currFeature->line_nb, currFeature->reporank);

            currFeature = store->at(++readHead);
        }

        pointers.pop();
        pointers.push({
            currPointer.featureID,
            readHead,
            store->at(readHead)->file_id,
            currPointer.end,
            (readHead >= currPointer.end)
         });
    }

    // do something with the returned result
    printf("found %d element matching the query\n", count);
}





