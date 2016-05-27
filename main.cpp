#include <iostream>
#include <fstream>
#include <string>
#include "StringDict.h"
#include "JsonImporter.h"
#include "FeatureStore.h"
#include "FeatureIndex.h"
#include <regex>

using namespace std;

#define MAX_FEATURE_READ 20000

int main() {

    JsonImporter ji("/Users/matt/git/devsearch-lookup/bucket1.json");
    StringDict* file_sd = new StringDict();
    StringDict* feature_sd = new StringDict();
    FeatureStore* db = new FeatureStore(MAX_FEATURE_READ);


    string feature, file;
    int lineNb = 0;
    float reporank = 0.0;

    int count = 0;
    while(ji.readFeature(&feature, &file, &reporank, &lineNb) && count < MAX_FEATURE_READ) {

        int file_id = file_sd->add(file);
        int feature_id = feature_sd->add(feature);

        count++;

        db->add(file_id, feature_id, reporank, lineNb);

        if (count%1000 == 0) {
            printf("processed %dK\n", count/1000);
            fflush(stdout);
        }
    }

    printf("Feature processed: %d\n", count);
    printf("File Count: %d\n", file_sd->size);
    printf("File Node Count: %d\n", file_sd->nodeCount());
    printf("Feature Count: %d\n", feature_sd->size);
    printf("Feature Node Count: %d\n", feature_sd->nodeCount());

//    file_sd->print();

    cout << endl;

//    No need to sort actually
//    int translation[file_sd->size];
//
//    file_sd->sort(translation);
//
//    file_sd->print();

    cout << endl;

//    db->print();
    db->sort();
//    cout << endl;
    db->print();


    FeatureIndex* db_index = new FeatureIndex(feature_sd->size, count);
    db->buildFeatureIndex(db_index);

    string query[] = {
        "controlStatement=if",
        "import=java.util.List",
        "controlStatement=for"
    };



//    file_sd->print();
//    cout << endl << endl << endl;
//    feature_sd->print();


    return 0;
}
