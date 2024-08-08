#include <chrono>

#include "KMeansSeq.h"
#include "generateData.h"

int main(int argc, char **argv){
    // code to generate random csv files with coordinates to clusters
    //generate_random_csv("10mil8d.csv", 10000000, 8, 11);

    std::string line;
    std::string filename= "1mil2d";
    std::ifstream file("clusters/"+filename+".csv");
    if (!file.is_open())
    {
        printf("Error: Failed to open file. \n");
        return 1;
    }

    // Fetching points from file
    int pointId = 0;
    std::vector<Point> points;
    while (getline(file, line))
    {   if(pointId>0){ // skip the first line
            Point point(pointId, line);
            points.push_back(point);
        }
        pointId++;
    }

    file.close();
    printf("Data fetched successfully! \n");

    int iters = 1, k = 2;

    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    KMeansSeq kmeansSeq(points,k,iters);
    kmeansSeq.run("clusters", filename);

    auto t2 = Clock::now();

    std::chrono::duration<double, std::milli> time = t2 - t1;

    printf("It took %f milliseconds \n", time.count());
    return 0;
}