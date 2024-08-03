#include "checkVocabularySeq.h"

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

int main(){
    //
    std::string filename = "vocabularies/all_english_words.txt";
    std::ifstream Infile(filename);
    int fileLength= (int)std::count(std::istreambuf_iterator<char>(Infile),
               std::istreambuf_iterator<char>(), '\n');

    // select string to search on the vocabulary
    std::string string = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    checkVocabularySeq(string, filename, fileLength);

    auto t2 = Clock::now();

    std::chrono::duration<double, std::milli> time = t2 - t1;

    printf("It took %f milliseconds \n", time.count());


    return 0;
}
