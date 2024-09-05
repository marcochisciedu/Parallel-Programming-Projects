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

class MultiDimensionalPointArray {
public:
    explicit MultiDimensionalPointArray(const std::string& filename);

    // copy constructor
    MultiDimensionalPointArray(MultiDimensionalPointArray& dimArr){
        numPoints = dimArr.numPoints;
        dimensions = dimArr.dimensions;
        for (size_t i = 0; i < dimensions; i++) {
            dimensionArrays.push_back(new double[numPoints]);
            for(size_t p=0; p< numPoints; p++)
                dimensionArrays[i][p] = dimArr.dimensionArrays[i][p];
        }
        clusterId = dimArr.clusterId;
    }

    ~MultiDimensionalPointArray() {
        // Free the memory allocated for the dimension arrays
        for (auto arr : dimensionArrays) {
            delete[] arr;
        }
    }

    double& operator()(size_t pointIndex, size_t dimensionIndex) {
        return dimensionArrays[dimensionIndex][pointIndex];
    }

    const double& operator()(size_t pointIndex, size_t dimensionIndex) const {
        return dimensionArrays[dimensionIndex][pointIndex];
    }

    [[nodiscard]] size_t getNumPoints() const {
        return numPoints;
    }
    [[nodiscard]] size_t getNumDimensions() const {
        return dimensions;
    }

    [[nodiscard]] int getClusterIdByIndex(int index) const { return clusterId[index];}

    void setClusterIdByIndex(int index, int value){ clusterId[index]= value;}

    void fileToPoints(const std::string& filename);

private:
    size_t numPoints;
    size_t dimensions;
    std::vector<double*> dimensionArrays;
    std::vector<int> clusterId;

    void lineToCoord(std::string &line, int point_num);
    static size_t getDimFromFile(const std::string& filename);
};




class Cluster {

public:
    Cluster(int clusterId,  std::vector<double> centroid);

    [[nodiscard]] int getId() const { return clusterId; }

    [[nodiscard]] double getCentroidCoordByPos(int pos) const { return centroid[pos]; }

    void setCentroidByPos(int pos, double val) { this->centroid[pos] = val; }

private:
    int clusterId;
    // cordinates
    std::vector<double> centroid;
};


#endif //PARALLEL_PROGRAMMING_PROJECTS_POINTSSOA_H
