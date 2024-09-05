#include "checkVocabularySeq.h"
#include "checkVocabularyPar.h"
#include "generateData.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

// measure the time to run given a checkVocabulary function
double time_checkVocab(const std::vector<std::string>& words, const std::string& string,
                            const std::function<void(const std::string&, const std::vector<std::string>&, int threads,  bool display)>& checkVocabulary,
                            int threads, bool display){

    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    checkVocabulary(string, words, threads, display);
    auto t2 = Clock::now();
    std::chrono::duration<double, std::milli> time = t2 - t1;

    return time.count();
}

// function to test Levenshtein Distance's sequential and parallel implementations
void testLevenshteinDistance(const std::string& filename, const std::string& string, const int& iterations, int threads, bool display){
    // get the test file's number of lines
    std::ifstream Infile(filename);
    int fileLength= (int)std::count(std::istreambuf_iterator<char>(Infile),
                                    std::istreambuf_iterator<char>(), '\n');
    std::cout<< "number of lines "<< fileLength <<std::endl;

    // create a vector containing all the file's words
    std::vector<std::string> words(fileLength);
    std::ifstream file(filename);
    if (!file.is_open())
    {
        printf("Error: Failed to open file. \n");
        return ;
    }
    for ( int i = 0; i < fileLength; ++i )
        getline(file, words[i]);
    file.close();

    // outputfile to store the results
    std::ofstream outfile;
    outfile.open("results/Levenshtein_Distance_results.txt", std::ios_base::app);
    if (!outfile.is_open()){
        printf("Error: Unable to write to Levenshtein_Distance_results.txt \n");
        return ;
    }

    // string to search on the vocabulary
    std::cout<< "the string we're searching for is "<< string <<" in "<< filename << " with " << threads<< " threads " <<std::endl;
    outfile <<"Selected string: " << string<< " searched in file: " << filename << " with " << threads<< " threads " << std::endl;

    double mean_time_seq = 0, mean_time_par_crit = 0, mean_speedup_crit = 0, mean_time_par_private = 0, mean_speedup_private = 0;

    for(int i=0; i< iterations; i++ ) {
        printf("Iteration: %d \n", i+1);
        // test the sequential check vocabulary and measure the time it takes to run
        double time_seq = time_checkVocab(words, string, checkVocabularySeq, threads, display);
        mean_time_seq += time_seq;
        printf("The sequential check vocabulary completed in %f milliseconds \n", time_seq);

        // test the parallel check vocabulary with more critical sections and measure the time it takes to run
        double time_par_crit = time_checkVocab(words, string, checkVocabularyParCrit, threads, display);
        mean_time_par_crit += time_par_crit;
        printf("The parallel check vocabulary with more critical sections completed in %f milliseconds \n",
               time_par_crit);
        //calculate the speedup
        double speedup_crit = time_seq / time_par_crit;
        mean_speedup_crit += speedup_crit;
        printf("The speedup with the parallel check vocabulary with more critical sections is: %f \n", speedup_crit);

        // test the parallel check vocabulary with one final critical section and measure the time it takes to run
        double time_par_private = time_checkVocab(words, string, checkVocabularyParPrivate, threads, display);
        mean_time_par_private += time_par_private;
        printf("The parallel check vocabulary with one final critical section completed in %f milliseconds \n",
               time_par_private);
        //calculate the speedup
        double speedup_private = time_seq / time_par_private;
        mean_speedup_private += speedup_private;
        printf("The speedup with the parallel check vocabulary with one final critical section is: %f \n",
               speedup_private);
    }
    mean_time_seq /= iterations, mean_time_par_crit /= iterations, mean_speedup_crit /= iterations,
    mean_time_par_private /= iterations, mean_speedup_private /= iterations ;
    outfile << "Mean milliseconds sequential: " << mean_time_seq << std::endl;
    outfile << "Mean milliseconds parallel with more critical sections: "
            << mean_time_par_crit << " mean speedup: " << mean_speedup_crit << std::endl;
    outfile << "Mean milliseconds parallel with one final critical section: "
            << mean_time_par_private << " mean speedup: " << mean_speedup_private << std::endl;
    outfile << std::endl;
    outfile.close();
}

int main(){
    // code to generate txt file with random strings
    //generate_random_txt("10milRand60.txt", 10000000, 60, 11);
    // code to generate txt file with a given percentage of similar words
    //generate_similar_txt("10mil50Short", 100000000, 50, 11 );

    //testLevenshteinDistance("vocabularies/10mil80Short.txt", "dd", 1, 10, false);
    testLevenshteinDistance("vocabularies/German.txt",
                            "Korrespondenzbankenabteilungen", 1, 10, true);

    /*testLevenshteinDistance("vocabularies/all_Languages.txt",
                             "Korrespondenzbankenabteilungen", 5);

    testLevenshteinDistance("vocabularies/all_Languages_2.txt",
                            "Korrespondenzbankenabteilungen", 5);

    testLevenshteinDistance("vocabularies/all_Languages_10.txt",
                            "Korrespondenzbankenabteilungen", 5);

    testLevenshteinDistance("vocabularies/1milRand30.txt",
                            "asfsafsafafwasxfsafsasafsafsafsfsaw", 5);

    testLevenshteinDistance("vocabularies/1milRand60.txt",
                            "asfsafsafafwasxfsafsasafsafsafsfsaw", 5);

    testLevenshteinDistance("vocabularies/10milRand30.txt",
                            "asfsafsafafwasxfsafsasafsafsafsfsaw", 5);

    testLevenshteinDistance("vocabularies/10milRand60.txt",
                            "asfsafsafafwasxfsafsasafsafsafsfsaw", 5); */
    return 0;
}
