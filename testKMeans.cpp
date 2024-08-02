#include <chrono>

#include "KMeansSeq.h"

int main(int argc, char **argv){
    std::string line;
    std::ifstream file("clusters/basic5_train.csv");
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

    int iters = 10000, k = 3;

    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    KMeansSeq kmeansSeq(points,k,iters,"clusters");
    kmeansSeq.run();

    auto t2 = Clock::now();

    std::chrono::duration<double, std::milli> time = t2 - t1;

    printf("It took %f milliseconds \n", time.count());
    return 0;
}