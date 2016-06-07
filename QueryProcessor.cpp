//
// Created by Matthieu Rudelle on 27/05/16.
//

#include "QueryProcessor.h"
#include "QueryAggregator.h"
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

void QueryProcessor::process(int qLength, int q[], int take, int from, vector<agg_result>* out, int* matchedFeatures, int* matchedFiles) {

    std::priority_queue<pointer, std::vector<pointer>, LessThanByFileId> pointers;
    max_heap_t bestResults;

    int totFeatureCount = 0;
    int totFileCount = 0;

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

    QueryAggregator* aggregator = nullptr;
     
    int readHead;
    int currFileID = -1;

    while (!pointers.top().exhausted) {

        currPointer = pointers.top();
        readHead = currPointer.currPos;
        currFeature = store->at(readHead);

//        When we reach a new file we start a new aggregator
        if (currFileID != currPointer.currFileId && aggregator) {

            // TODO: finalize and append current aggregator
            agg_result* agg = new agg_result();
            aggregator->finalize(qLength, agg);

            insertMaxHeap(agg, take+from, &bestResults);

            delete aggregator;
            aggregator = nullptr;

        }

        currFileID = currPointer.currFileId;

        while(currFeature->file_id == currFileID && currFeature->feature_id == currPointer.featureID) {

            totFeatureCount+=1;

            // do something with store->at(readHead)
            if (!aggregator) {
                aggregator = new QueryAggregator(currFeature);

                // means that we found a new file matching query
                totFileCount ++;
            } else {
                aggregator->addFeature(currFeature);
            }

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

    // add the final aggregation result
    if (aggregator) {

        agg_result* agg = new agg_result();
        aggregator->finalize(qLength, agg);

        insertMaxHeap(agg, take+from, &bestResults);

        delete aggregator;
        aggregator = nullptr;
    }


    // fill the output array with the list of best results
    for (int j = 0; j < bestResults.size() - take; ++j) {
        bestResults.pop();
    }

    while (!bestResults.empty()) {
        out->emplace_back(*bestResults.top());
        bestResults.pop();
    }

    // return the number of results
    *matchedFeatures = totFeatureCount;
    *matchedFiles = totFileCount;
}

void QueryProcessor::insertMaxHeap(agg_result *in, int maxSize, max_heap_t *maxHeap) {
    if (maxHeap->size() < maxSize) {

        maxHeap->push(in);

    } else if (maxHeap->top()->score < in->score) {

        // unallocate this aggregated result
        delete maxHeap->top();
        maxHeap->pop();

        maxHeap->push(in);
    }
}







