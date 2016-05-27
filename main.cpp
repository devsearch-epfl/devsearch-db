#include <iostream>
#include <fstream>
#include <string>
#include "StringDict.h"
#include "JsonImporter.h"
#include "FeatureStore.h"
#include <regex>

using namespace std;

#define MAX_FEATURE_READ 1000

int main() {

//    ifstream jsonFile;
//
//    jsonFile.open("/Users/matt/git/devsearch-lookup/bucket1.json");
//
//    string line;
//
//    cout << " " << int('a')  << " " << int('z')  << " " << int('-')  << " " << int('A')  << " " << int('-') << endl;
//
//    getline(jsonFile, line);
//
//    cout << line;
//
//    StringDict* dict = new StringDict();
//
//    int a = dict->add("a");
//    int aa = dict->add("aa");
//    int aaa = dict->add("aaa");
//
//    int b = dict->add("b");
//    int ba = dict->add("ba");
//    int b_bis = dict->add("b");
//
//    int aaa_bis = dict->add("aaa");
//
//    printf("\n%d %d %d %d %d %d %d\n", a, aa, aaa, aaa_bis, b, ba, b_bis);
//
////    regex reg1("{\"feature\":\"([^\"]+)\",\"file\":\"([^\"]+)\",\"line\":{\"\\$numberInt\":\"(\\d+)\"},\"repoRank\":([\\d\\.]+)}");
//    regex reg1("\\{\"feature\":\"([^\"]+)\",\"file\":\"([^\"]+)\",\"line\":\\{\"\\$numberInt\":\"(\\d+)\"\\},\"repoRank\":([\\d\\.]+)\\}");
//    smatch match;
//
//    regex_search(line, match, reg1);
//
//    cout << " " << match.str(1)  << " " << match.str(2)  << " " << match.str(3)  << " " << match.str(4)  << " " << match.str(5) << endl;
//
//    cout << endl << endl << endl;

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

    file_sd->print();

    cout << endl;

//    No need to sort actually
//    int translation[file_sd->size];
//
//    file_sd->sort(translation);
//
//    file_sd->print();

    cout << endl;

    db->print();
    db->sort();
    cout << endl;
    db->print();


//    file_sd->print();
//    cout << endl << endl << endl;
//    feature_sd->print();


    return 0;
}
