//
// Created by Marco on 30/07/2024.
//

#ifndef PARALLEL_PROGRAMMING_PROJECTS_CLUSTER_H
#define PARALLEL_PROGRAMMING_PROJECTS_CLUSTER_H
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class Point
{
public:
    Point(int id, std::string line);

    [[nodiscard]] int getDimensions() const { return dimensions; }

    [[nodiscard]] int getCluster() const { return clusterId; }

    [[nodiscard]] int getID() const { return pointId; }

    void setCluster(int val) { clusterId = val; }

    [[nodiscard]] double getCoord(int pos) const{ return coordinates.at(pos); }


private:
    int pointId, clusterId;
    int dimensions;
    std::vector<double> coordinates;

    static std::vector<double> lineToVec(std::string &line);
};



class Cluster {

public:
    Cluster(int clusterId, const Point& centroid);

    void addPoint(Point p);

    void removeAllPoints() { points.clear(); }

    [[nodiscard]] int getId() const { return clusterId; }

    Point getPoint(int pos) { return points[pos]; }

    [[nodiscard]] int getSize() const{ return (int)points.size(); }

    [[nodiscard]] double getCentroidByPos(int pos) const { return centroid[pos]; }

    void setCentroidByPos(int pos, double val) { this->centroid[pos] = val; }

private:
    int clusterId;
    std::vector<double> centroid;
    std::vector<Point> points;
};


#endif //PARALLEL_PROGRAMMING_PROJECTS_CLUSTER_H
