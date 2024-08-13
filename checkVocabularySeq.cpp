
#include "checkVocabularySeq.h"

void checkVocabularySeq(const std::string& string, const std::vector<std::string>& words){
    int string_distance;
    int min_distance = std::numeric_limits<int>::max();
    std::vector<std::string> closest_strings;
    // get all the levenshteinDistances from the string to the vocabulary and find the closest strings
    for(const auto & word : words){
        string_distance = levenshteinDistance(string,(int)string.length(), word,(int)word.length());
        // find the closest string
        if(string_distance<min_distance){
            min_distance = string_distance;
            closest_strings.clear();
            closest_strings.push_back(word);
        }
        // check if more than one string is as close to the target string
        else if (string_distance == min_distance) {
            closest_strings.push_back(word);
        }
    }

    // print the closest strings
    for (auto & s : closest_strings ){
        printf("%s is distant %d operations from %s  \n", s.c_str(), min_distance, string.c_str());
    }

}