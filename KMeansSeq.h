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
    KMeansSeq(std::vector<Point> &points, int K, int iterations, std::string output_dir);

    void run();

private:
    int K, iters, dimensions;
    std::vector<Cluster> clusters;
    std::string output_dir;
    std::vector<Point> points;
    void clearClusters();

    int getNearestClusterId(const Point& point);

};


#endif //PROGETTOESAME_KMEANSCLUSTERING_H
