//
// Created by Matthieu Rudelle on 26/05/16.
//

#include "JsonImporter.h"
#include <fstream>
#include <string>
#include <regex>
using namespace std;

#define MAX_BUFFER_LENGTH 1000


//TODO: allow for space between objects
const regex FEATURE_REGEX("\\{\"feature\":\"((?:[^\"]|\\\\\")+)\",\"file\":\"([^\"]+)\",\"line\":\\{\"\\$numberInt\":\"(\\d+)\"\\},\"repoRank\":([\\d\\.]+)\\}");

JsonImporter::JsonImporter(string filename) {
//    TODO: file does not exist + IOException

    // open file for reading
    input.open(filename);

    buffer = new char[MAX_BUFFER_LENGTH];
}

bool JsonImporter::readFeature(string *feature, string *file, float *reporank, int *line) {

    if (input.eof()) {
        input.close();
        return false;
    }

    input.getline(buffer, MAX_BUFFER_LENGTH);

    string featLine(buffer);

    smatch matches;
    bool match = regex_search(featLine, matches, FEATURE_REGEX);

    if (!match) {

        *feature = "dummy";
        *file = "dummy";
        *line = 0;
        *reporank = 0.0;

        printf("Line not parseable: %s\n", featLine.c_str());

        // even if we can't match it we return true as it's not a EOF
        return true;
    }

    *feature = matches.str(1);
    *file = matches.str(2);
    *line = stoi(matches.str(3));
    *reporank = stof(matches.str(4));

    return true;
}



