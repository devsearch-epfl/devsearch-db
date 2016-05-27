//
// Created by Matthieu Rudelle on 26/05/16.
//

#ifndef DEVSEARCH_DB_JSONIMPORTER_H
#define DEVSEARCH_DB_JSONIMPORTER_H

#include <string>
#include <fstream>

using namespace std;

class JsonImporter {
private:
    ifstream input;
    char* buffer;
public:
    JsonImporter(string filename);
    bool readFeature(string* feature, string* file, float* reporank, int* line);
};


#endif //DEVSEARCH_DB_JSONIMPORTER_H
