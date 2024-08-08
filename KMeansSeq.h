//
// Created by Marco on 29/07/2024.
//
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include "Cluster.h"

#ifndef PROGETTOESAME_KMEANSCLUSTERING_H
#define PROGETTOESAME_KMEANSCLUSTERING_H

class KMeansSeq{

public:
    KMeansSeq(std::vector<Point> &points, int K, int iterations);

    void run(const std::string& output_dir, const std::string& original_filename);

private:
    int K, iters, dimensions;
    std::vector<Cluster> clusters;
    std::vector<Point> points;
    void clearClusters();

    int getNearestClusterId(const Point& point);

};


#endif //PROGETTOESAME_KMEANSCLUSTERING_H
