//
// Created by Matthieu Rudelle on 07/06/16.
//

#include "QueryAggregator.h"

QueryAggregator::QueryAggregator(db_feature* feature) {
    repoRank = feature->reporank;
    fileId = feature->file_id;

    startLine = feature->line_nb;
    endLine = feature->line_nb;
}

void QueryAggregator::addFeature(db_feature* feature) {
    featureSet.emplace(feature->feature_id);
    lines.emplace_back(feature->line_nb);

    if (startLine > feature->line_nb) {
        startLine = feature->line_nb;
    }

    if (endLine < feature->line_nb) {
        endLine = feature->line_nb;
    }
}

void QueryAggregator::finalize(int qLength, agg_result* out) {

    double ratioOfMatches = featureSet.size()/qLength;
    double sizeScore = lines.size();

    double densityScore = sizeScore / (endLine-startLine+1);
    densityScore = (densityScore > 5.0 ? 5.0 : densityScore) / 5.0;

    sizeScore = (sizeScore > 100.0? 100.0 : sizeScore) / 100.0;

    out->score = .6 * densityScore + .4 * sizeScore /*+ .3 * rarityScore */+ .1 * ratioOfMatches + .4 * repoRank;

    out->scoreBreakdown->emplace("repoRank", .4 * repoRank);
    out->scoreBreakdown->emplace("ratioOfMatches", .1 * ratioOfMatches);
    out->scoreBreakdown->emplace("rarity", 0.0);
    out->scoreBreakdown->emplace("density", 0.0);
    out->scoreBreakdown->emplace("size", .4 * sizeScore);
    out->scoreBreakdown->emplace("final", out->score);
}





