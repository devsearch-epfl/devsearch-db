#include <iostream>
#include <fstream>
#include <string>
#include "StringDict.h"
#include "JsonImporter.h"
#include "FeatureStore.h"
#include "FeatureIndex.h"
#include "QueryProcessor.h"
#include <regex>
#include <ctime>

using namespace std;

#define MAX_FEATURE_READ 27000000

int main() {

    JsonImporter ji("/Users/matt/git/devsearch-lookup/bucket1.json");
    StringDict* file_sd = new StringDict();
    StringDict* feature_sd = new StringDict();
    FeatureStore* db = new FeatureStore(MAX_FEATURE_READ);


    string feature, file;
    int lineNb = 0;
    float reporank = 0.0;

    const clock_t begin_import_time = clock();

    int count = 0;
    while(ji.readFeature(&feature, &file, &reporank, &lineNb) && count < MAX_FEATURE_READ) {

        int file_id = file_sd->add(file);
        int feature_id = feature_sd->add(feature);

        count++;

        db->add(file_id, feature_id, reporank, lineNb);

        if (count%100000 == 0) {
            printf("processed %dK\n", count/1000);
            fflush(stdout);
        }
    }

    printf("DB imported in %f sec\n", float( clock () - begin_import_time ) /  CLOCKS_PER_SEC);

    printf("Feature processed: %d\n", count);
    printf("File Count: %d\n", file_sd->size);
    printf("File Node Count: %d\n", file_sd->nodeCount());
    printf("Feature Count: %d\n", feature_sd->size);
    printf("Feature Node Count: %d\n", feature_sd->nodeCount());


    //not actually needed, build index will do it
//    db->sort();

    FeatureIndex* db_index = new FeatureIndex(feature_sd->size, count);
    db->buildFeatureIndex(db_index);

    const clock_t begin_sort_time = clock();
    db->sort();
    printf("DB sorted in %f sec\n", float( clock () - begin_sort_time ) /  CLOCKS_PER_SEC);

//    db->print();

    for (int i = 0; i < db->size-1; ++i) {
        if(db->at(i)->feature_id > db->at(i+1)->feature_id) {

            printf("db has problem\n");
            exit(1);
        }
    }

    int qLength = 3;

    string query[] = {
        "controlStatement=if",
        "import=java.util.List",
        "controlStatement=for"
    };

    int keys[qLength];

    feature_sd->getBatchKeys(qLength, query, keys);

    QueryProcessor* q = new QueryProcessor(db, db_index);

    const clock_t begin_query_time = clock();
    q->process(qLength, keys, 10, 0);
    printf("DB queried in %f sec\n", float( clock () - begin_query_time ) /  CLOCKS_PER_SEC);


    return 0;
}
