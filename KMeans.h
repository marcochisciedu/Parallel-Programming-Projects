

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include "Cluster.h"

#ifndef PROGETTOESAME_KMEANSCLUSTERING_H
#define PROGETTOESAME_KMEANSCLUSTERING_H

class KMeans{

public:
    KMeans(std::vector<Point> &points, int K, int iterations);

    double runSeq(const std::string& output_dir, const std::string& original_filename);

    double runPar(const std::string& output_dir, const std::string& original_filename);

    double runParReduction(const std::string& output_dir, const std::string& original_filename);

    void resetPointsClusters();

private:
    int K, iters, dimensions;
    std::vector<Cluster> clusters;
    std::vector<Point> points;

    int getNearestClusterId(const Point& point);
    void clearClusters();
    void createOutputFile(const std::string& output_dir, const std::string& original_filename, const std::string& context);

};


#endif //PROGETTOESAME_KMEANSCLUSTERING_H
