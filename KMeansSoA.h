

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include "PointsSoA.h"

#ifndef PROGETTOESAME_KMEANSCLUSTERING_H
#define PROGETTOESAME_KMEANSCLUSTERING_H

class KMeansSoA{

public:
    KMeansSoA(struct Points8D all_points, int K, int iterations);

    double runSeq(const std::string& output_dir, const std::string& original_filename);

    void resetPointsClustersID() const;

private:
    int K, iters;
    std::vector<Cluster> clusters;
    struct Points8D points{};

    [[nodiscard]] std::vector<double> indexToCoordinates(int index) const;
    int getNearestClusterId(int index);
    void createOutputFile(const std::string& output_dir, const std::string& original_filename, const std::string& context) const;

};

#endif //PROGETTOESAME_KMEANSCLUSTERING_H
