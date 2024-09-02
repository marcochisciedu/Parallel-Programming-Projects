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

void testKMeans(const std::string& filename, const int& k, const int& iters_kmeans, const int& out_iters ){
    Points8D points8D = linetoPoints8D("1mil8d");

    // outputfile to store the results
    std::ofstream outfile;
    outfile.open("results/KmeansSoA_results.txt", std::ios_base::app);
    if (!outfile.is_open()){
        printf("Error: Unable to write to KmeansSoA_results.txt \n");
        return;
    }

    std::cout<< "Searching for  "<< k <<" clusters in "<< filename <<" with " << iters_kmeans<< " iterations" <<std::endl;
    outfile <<"Searching for " << k<< " clusters in " << filename <<" with " << iters_kmeans<< " iterations" << std::endl;

    double mean_time_seq = 0, mean_time_par = 0, mean_speedup = 0, mean_time_par_reduction = 0, mean_speedup_reduction =0;
    for(int i = 0; i< out_iters; i++) {
        std::cout << "Total iteration: "<< i+1 << "/" << out_iters << std::endl;
        KMeansSoA kmeans(points8D, k, iters_kmeans);

        double time_seq = kmeans.runSeq("clusters", filename);
        printf("The sequential version of kmeans completed in %f milliseconds \n", time_seq);
        mean_time_seq += time_seq;

        kmeans.resetPointsClusters();

        double time_par = kmeans.runParPrivate("clusters", filename);
        printf("The private parallel version of kmeans completed in %f milliseconds \n", time_par);
        double speedup = time_seq / time_par;
        printf("The speedup is %f \n", speedup);
        mean_time_par += time_par;
        mean_speedup += speedup;

        compare_csv("clusters/" + filename + "_par_" + std::to_string(k) + "-clusters.csv",
                    "clusters/" + filename + "_seq_" + std::to_string(k) + "-clusters.csv", outfile);

        /*kmeans.resetPointsClusters();

        double time_par_reduction = kmeans.runParReduction("clusters", filename);
        printf("The parallel version of kmeans with reduction completed in %f milliseconds \n", time_par_reduction);
        double speedup_reduction = time_seq / time_par_reduction;
        printf("The speedup with reduction is %f \n", speedup_reduction);
        mean_time_par_reduction += time_par_reduction;
        mean_speedup_reduction += speedup_reduction;

        compare_csv("clusters/" + filename + "_par_red_" + std::to_string(k) + "-clusters.csv",
                    "clusters/" + filename + "_seq_" + std::to_string(k) + "-clusters.csv", outfile); */
    }

    mean_time_seq /= out_iters, mean_time_par /= out_iters, mean_speedup /= out_iters;
           // mean_time_par_reduction /= out_iters, mean_speedup_reduction /= out_iters;
    outfile << "On average, the sequential version of kmeans completed in " << mean_time_seq
    << " milliseconds" << std::endl;

    outfile << "On average, the parallel version of kmeans completed in " << mean_time_par << " milliseconds with a "
            << mean_speedup << " speedup" << std::endl;
   // outfile << "On average, the parallel version of kmeans with reduction completed in " << mean_time_par_reduction <<
     //       " milliseconds with a " << mean_speedup_reduction << " speedup" << std::endl;
    outfile << std::endl;
}


int main(int argc, char **argv){
    // code to generate random csv files with coordinates to clusters
    //generate_random_csv("10mil8d.csv", 10000000, 8, 11);

    testKMeans("1mil8d", 2 ,5,1);

    // serve free(coordinate di struct) ?? guardare codice
    return 0;
}