
#include "checkVocabularyPar.h"

void checkVocabularyParCrit(const std::string& string, const std::vector<std::string>& words){
    int string_distance;
    int min_distance = std::numeric_limits<int>::max();
    std::vector<std::string> closest_strings;

    // get all the levenshteinDistances from the string to the vocabulary and find the closest strings
    // create the threads, start parallel section
#pragma omp parallel default(none) shared(min_distance, closest_strings, words) private(string_distance) firstprivate(string)
{   //parallelize the for loop
    #pragma omp for
    for(const auto & word : words){
        string_distance = levenshteinDistance(string,(int)string.length(), word,(int)word.length());
        // find the closest string
        // synchronize min_distance and closest_strings in all the threads
        if(string_distance<min_distance){
            #pragma omp flush(min_distance)
            #pragma omp atomic write
            min_distance = string_distance;
            #pragma omp flush(min_distance)
            #pragma omp critical
            {
            closest_strings.clear();
            closest_strings.push_back(word);
            }
        }
            // check if more than one string is as close to the target string
            // synchronize closest_strings in all the threads
        else if (string_distance == min_distance) {
            #pragma omp critical
            closest_strings.push_back(word);
        }
    }
    // implicit barrier

    // print the closest strings
    //parallelize the for loop since the thread are already available
    #pragma omp for
    for (auto &s: closest_strings) {
        printf("%s is distant %d operations from %s  \n", s.c_str(), min_distance, string.c_str());
    }
}

}


void checkVocabularyParPrivate(const std::string& string, const std::vector<std::string>& words){
    int string_distance;
    int min_distance = std::numeric_limits<int>::max();
    std::vector<std::string> closest_strings;

    // get all the levenshteinDistances from the string to the vocabulary and find the closest strings
    // create the threads, start parallel section
#pragma omp parallel default(none) shared(words, closest_strings, min_distance) private(string_distance) firstprivate(string)
    {   // each thread has its own local variables to search the closest words to the original string
        std::vector<std::string> local_closest_strings;
        int local_min_distance = std::numeric_limits<int>::max();
        //parallelize the for loop, not necessary to wait all the threads, they can start entering the critical section
        #pragma omp for nowait
        for(const auto & word : words){
            string_distance = levenshteinDistance(string,(int)string.length(), word,(int)word.length());
            // find the closest string
            if(string_distance<local_min_distance){
                local_min_distance = string_distance;
                local_closest_strings.clear();
                local_closest_strings.push_back(word);

            }
                // check if more than one string is as close to the target string
            else if (string_distance == local_min_distance) {
                local_closest_strings.push_back(word);
            }
        }
        // synchronize all the threads, find the real closest strings
        #pragma omp critical
        {
            if(local_min_distance < min_distance){
                min_distance = local_min_distance;
                closest_strings.clear();
                closest_strings.insert(closest_strings.end(), local_closest_strings.begin(), local_closest_strings.end());
            }
            else if(local_min_distance == min_distance){
                closest_strings.insert(closest_strings.end(), local_closest_strings.begin(), local_closest_strings.end());
            }

        }

        // make sure that all the thread compared their local_closest_strings before final print
        #pragma omp barrier

        // print the closest strings
        //parallelize the for loop since the thread are already available
        #pragma omp for
        for (auto &s: closest_strings) {
            printf("%s is distant %d operations from %s  \n", s.c_str(), min_distance, string.c_str());
        }
    }

}