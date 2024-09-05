//
// Created by Marco on 30/07/2024.
//

#include "Cluster.h"

#include <utility>

Point::Point(int id, std::string line){
    pointId = id;
    coordinates = lineToVec(line);
    dimensions = (int)coordinates.size();
    clusterId = -1; // Initially not assigned to any cluster
}

std::vector<double> Point::lineToVec(std::string &line){
    std::vector<double> coordinates;
    std::string tmp;
    // 48-57 are 0-9 in ASCII
    for (int i = 0; i < (int)line.length(); i++)
    {
        if ((48 <= int(line[i]) && int(line[i])  <= 57) || line[i] == '.' || line[i]== '-')
        {
            tmp += line[i];
        }
        else if (!tmp.empty())
        {   // converts string to double
            coordinates.push_back(stod(tmp));
            tmp = "";
        }
    }
    if (!tmp.empty())
    {
        coordinates.push_back(stod(tmp));
        tmp = "";
    }

    return coordinates;
}

Cluster::Cluster(int clusterId, std::vector<double> centroid)
{
    this->clusterId = clusterId;
    this->centroid = std::move(centroid);
}

