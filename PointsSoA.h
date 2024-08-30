//
// Created by Marco on 30/07/2024.
//

#ifndef PARALLEL_PROGRAMMING_PROJECTS_POINTSSOA_H
#define PARALLEL_PROGRAMMING_PROJECTS_POINTSSOA_H
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Points8D{
    int num_points;
    int* clusterId;
    double* firstCoord;
    double* secondCoord;
    double* thirdCoord;
    double* fourthCoord;
    double* fifthCoord;
    double* sixthCoord;
    double* seventhCoord;
    double* eighthsCoord;
};

Points8D linetoPoints8D(const std::string& filename);


class Cluster {

public:
    Cluster(int clusterId,  std::vector<double> centroid);

    [[nodiscard]] int getId() const { return clusterId; }

    [[nodiscard]] double getCentroidCoordByPos(int pos) const { return centroid[pos]; }

    void setCentroidByPos(int pos, double val) { this->centroid[pos] = val; }

private:
    int clusterId;
    std::vector<double> centroid;
};


#endif //PARALLEL_PROGRAMMING_PROJECTS_POINTSSOA_H
