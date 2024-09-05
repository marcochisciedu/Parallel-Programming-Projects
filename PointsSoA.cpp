//
// Created by Marco on 30/07/2024.
//

#include <algorithm>
#include <utility>
#include "PointsSoA.h"

// extract a certain point's coordinate from its line
void MultiDimensionalPointArray::lineToCoord(std::string &line, int point_num){
    std::string tmp;
    size_t dim=0;
    // 48-57 are 0-9 in ASCII
    for (int j = 0; j < (int)line.length(); j++)
    {
        if ((48 <= int(line[j]) && int(line[j])  <= 57) || line[j] == '.' || line[j]== '-')
        {
            tmp += line[j];
        }
        else if (!tmp.empty())
        {   // converts string to double
            dimensionArrays[dim][point_num]=stod(tmp);
            dim++;
            tmp = "";
        }
    }
}

// extract the coordinates from the file
void MultiDimensionalPointArray::fileToPoints(const std::string& filename){
    // open the file
    std::string line;
    std::ifstream file("clusters/"+filename+".csv");
    if (!file.is_open())
    {
        printf("Error: Failed to open file. \n");
    }
    // Fetching points from file and set the null cluster for each point
    int i=-1;
    while (getline(file, line))
    {   if(i>=0){ // skip the first line
            //initialize all the points clusters as -1 (no cluster)
            clusterId.push_back(-1);
            //get the points' coordinates
            lineToCoord(line, i);
        }
        i++;
    }

    file.close();
    printf("Data fetched successfully! \n");
}

//given a file get the dimensions of its data
size_t MultiDimensionalPointArray::getDimFromFile(const std::string& filename) {
    size_t dim=0;
    //open the file
    std::string line;
    std::ifstream file("clusters/"+filename+".csv");
    if (!file.is_open())
    {
        printf("Error: Failed to open file. \n");
    }

    // skip the first line
    getline(file, line);
    getline(file, line);

    //count the numbers in the line
    for (int j = 0; j < (int)line.length(); j++)
    {
        if ((48 > int(line[j]) || int(line[j])  > 57) && line[j] != '.' && line[j]!= '-')
        {
            dim++;
        }

    }

    file.close();

    return dim;
}

// constructor, initialize the number of points, their dimension and their coordinates
MultiDimensionalPointArray::MultiDimensionalPointArray(const std::string& filename) {
    std::ifstream Infile("clusters/"+filename+".csv");
    if (!Infile.is_open())
    {
        printf("Error: Failed to open file. \n");
    }
    // count the file's lines except the first
    numPoints= (int)std::count(std::istreambuf_iterator<char>(Infile),
                               std::istreambuf_iterator<char>(), '\n') -1;
    std::cout<< "Number of points: "<< numPoints <<std::endl;
    Infile.close();

    dimensions = getDimFromFile(filename);
    std::cout<< "Number of dimensions: "<< dimensions <<std::endl;

    // Allocate memory for the arrays, one for each dimension
    for (size_t i = 0; i < dimensions; i++) {
        dimensionArrays.push_back(new double[numPoints]);
    }
    // get all the coordinates
    fileToPoints(filename);
}

Cluster::Cluster(int clusterId,  std::vector<double> centroid)
{
    this->clusterId = clusterId;
    this->centroid = std::move(centroid);
}


