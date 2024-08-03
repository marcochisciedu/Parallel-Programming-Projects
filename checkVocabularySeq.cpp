
#include "checkVocabularySeq.h"

void checkVocabularySeq(const std::string& string,const std::string& filename, const int& fileLength){

    std::string line;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        printf("Error: Failed to open file. \n");
    }

    int string_distance;
    int min_distance = std::numeric_limits<int>::max();
    std::vector<std::string> closest_strings;
    // get all the levenshteinDistances from the string to the vocabulary and find the closest strings
    for(int i = 0; i < fileLength; i++){
        getline(file,line);
        string_distance = levenshteinDistanceSeq(string,(int)string.length(), line,(int)line.length());
        if(string_distance<min_distance){
            min_distance = string_distance;
            closest_strings.clear();
            closest_strings.push_back(line);
        }
        else if (string_distance == min_distance) {
            closest_strings.push_back(line);
        }
    }

    for (auto & s : closest_strings ){
        printf("%s is distant %d operations from %s \n", s.c_str(), min_distance, string.c_str());
    }

    file.close();
}