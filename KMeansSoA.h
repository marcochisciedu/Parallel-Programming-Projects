

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <omp.h>
#include "PointsSoA.h"

#ifndef PROGETTOESAME_KMEANSCLUSTERING_H
#define PROGETTOESAME_KMEANSCLUSTERING_H

class KMeansSoA{

public:
    KMeansSoA(int K, int iterations,  MultiDimensionalPointArray& points) :
            K(K), iters(iterations), points(points) {
    }

    double runSeq(const std::string& output_dir, const std::string& original_filename);

    double runPar(const std::string& output_dir, const std::string& original_filename, int threads);

    double runParPrivate(const std::string& output_dir, const std::string& original_filename, int threads);

    void resetPointsClusters() ;

private:
    int K, iters;
    std::vector<Cluster> clusters;
    MultiDimensionalPointArray points;

    [[nodiscard]] std::vector<double> indexToCoordinates(int index) const;
    int getNearestClusterId(int index);
    void createOutputFile(const std::string& output_dir, const std::string& original_filename, const std::string& context) const;

};

#endif //PROGETTOESAME_KMEANSCLUSTERING_H
