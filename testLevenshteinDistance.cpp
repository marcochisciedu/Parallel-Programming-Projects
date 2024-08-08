#include "checkVocabularySeq.h"
#include "generateData.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

int main(){
    // code to generate txt file with random strings
    //generate_random_txt("10milRand60.txt", 10000000, 60, 11);

    // get the test file's number of lines
    std::string filename = "vocabularies/Portuguese.txt";
    std::ifstream Infile(filename);
    int fileLength= (int)std::count(std::istreambuf_iterator<char>(Infile),
               std::istreambuf_iterator<char>(), '\n');
    std::cout<< "number of lines "<< fileLength <<std::endl;

    // select string to search on the vocabulary
    std::string string = "crianca";

    // test the sequential check vocabulary and measure the time it takes
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    checkVocabularySeq(string, filename, fileLength);

    auto t2 = Clock::now();

    std::chrono::duration<double, std::milli> time = t2 - t1;

    printf("The sequential check vocabulary completed in %f milliseconds \n", time.count());


    return 0;
}
