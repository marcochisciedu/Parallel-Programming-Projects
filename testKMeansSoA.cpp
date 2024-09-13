#include <chrono>
#include <cstring>

#include "KMeansSoA.h"
#include "generateData.h"

// function that compares if two csv files contain the same points
void compare_csv(const std::string& firstFileName,const std::string& secondFileName, std::ofstream& outfile ){
    std::ifstream Infile(firstFileName);
    int fileLength= (int)std::count(std::istreambuf_iterator<char>(Infile),
                                    std::istreambuf_iterator<char>(), '\n');

    bool isSame = true;
    std::ifstream firstFile(firstFileName);
    std::ifstream secondFile(secondFileName);
    if (!firstFile.is_open() || !secondFile.is_open())
    {
        printf("Error: Failed to open file. \n");
    }
    std::string firstFileLine, secondFileLine;
    for ( int i = 0; i < fileLength; ++i ) {
        getline(firstFile, firstFileLine);
        getline(secondFile, secondFileLine);
        if(strcmp(firstFileLine.c_str(), secondFileLine.c_str()) != 0){
            isSame= false;
            printf("They differ at line %d \n", i+1);
            outfile<< "They differ at line " << i+1 << std::endl;
            break;
        }

    }
    if(isSame){
        printf("The files are the same \n");
        outfile<< "The files are the same" << std::endl;
    }
}

void testKMeans(const std::string& filename, const int& k, const int& iters_kmeans, const int& out_iters ,int threads){
    MultiDimensionalPointArray points(filename);

    // outputfile to store the results
    std::ofstream outfile;
    outfile.open("results/KmeansSoA_results.txt", std::ios_base::app);
    if (!outfile.is_open()){
        printf("Error: Unable to write to KmeansSoA_results.txt \n");
        return;
    }

    std::cout<< "Searching for "<< k <<" clusters in "<< filename <<" with " << iters_kmeans<< " iterations and " <<
    threads << " threads" << std::endl;
    outfile <<"Searching for " << k<< " clusters in " << filename <<" with " << iters_kmeans<< " iterations and " <<
    threads << " threads" << std::endl;
    double mean_time_seq = 0, mean_time_par = 0, mean_speedup = 0, mean_time_par_private = 0, mean_speedup_private =0;
    for(int i = 0; i< out_iters; i++) {
        std::cout << "Total iteration: "<< i+1 << "/" << out_iters << std::endl;
        KMeansSoA kmeans( k, iters_kmeans, points);

        double time_seq = kmeans.runSeq("output_clusters", filename);
        printf("The sequential version of kmeans completed in %f milliseconds \n", time_seq);
        mean_time_seq += time_seq;

        kmeans.resetPointsClusters();

        double time_par = kmeans.runPar("output_clusters", filename, threads);
        printf("The parallel version of kmeans completed in %f milliseconds \n", time_par);
        double speedup = time_seq / time_par;
        printf("The speedup is %f \n", speedup);
        mean_time_par += time_par;
        mean_speedup += speedup;

        compare_csv("output_clusters/" + filename + "_parSOA_" + std::to_string(k) + "-clusters.csv",
                    "output_clusters/" + filename + "_seqSOA_" + std::to_string(k) + "-clusters.csv", outfile);

        kmeans.resetPointsClusters();

        double time_par_private = kmeans.runParPrivate("output_clusters", filename, threads);
        printf("The private parallel version of kmeans completed in %f milliseconds \n", time_par_private);
        double speedup_private = time_seq / time_par_private;
        printf("The speedup with the private version is %f \n", speedup_private);
        mean_time_par_private += time_par_private;
        mean_speedup_private += speedup_private;

        compare_csv("output_clusters/" + filename + "_parPrivSOA_" + std::to_string(k) + "-clusters.csv",
                    "output_clusters/" + filename + "_seqSOA_" + std::to_string(k) + "-clusters.csv", outfile);
    }

    mean_time_seq /= out_iters, mean_time_par /= out_iters, mean_speedup /= out_iters,
            mean_time_par_private /= out_iters, mean_speedup_private /= out_iters;
    outfile << "On average, the sequential version of kmeans completed in " << mean_time_seq
    << " milliseconds" << std::endl;

    outfile << "On average, the parallel version of kmeans completed in " << mean_time_par << " milliseconds with a "
            << mean_speedup << " speedup" << std::endl;
   outfile << "On average, the private parallel version of kmeans completed in " << mean_time_par_private <<
           " milliseconds with a " << mean_speedup_private << " speedup" << std::endl;
    outfile << std::endl;
}


int main(int argc, char **argv){
    // code to generate random csv files with coordinates to clusters
    //generate_random_csv("1000002d.csv", 100000, 2, 11);

    // test different k
    testKMeans("1mil8d", 2 , 100, 5, 64);
    testKMeans("1mil8d", 4 , 100, 5, 64);
    testKMeans("1mil8d", 8 , 100, 5, 64);
    testKMeans("1mil8d", 16 , 100, 5, 64);
    testKMeans("1mil8d", 25 , 100, 5, 64);
    testKMeans("1mil8d", 32 , 100, 5, 64);

    // testing different dimensions and sizes
    testKMeans("1000002d", 8 , 100, 5, 64);
    testKMeans("1000004d", 8 , 100, 5, 64);
    testKMeans("1000008d", 8 , 100, 5, 64);

    testKMeans("1mil2d", 8 , 100, 5, 64);
    testKMeans("1mil4d", 8 , 100, 5, 64);
    testKMeans("1mil8d", 8 , 100, 5, 64);

    testKMeans("10mil2d", 8 , 100, 5, 64);
    testKMeans("10mil4d", 8 , 100, 5, 64);
    testKMeans("10mil8d", 8 , 100, 5, 64);

    // test different number of threads
    testKMeans("1mil8d", 8 , 100, 5, 2);
    testKMeans("1mil8d", 8 , 100, 5, 4);
    testKMeans("1mil8d", 8 , 100, 5, 8);
    testKMeans("1mil8d", 8 , 100, 5, 16);
    testKMeans("1mil8d", 8 , 100, 5, 32);
    testKMeans("1mil8d", 8 , 100, 5, 128);
    return 0;
}