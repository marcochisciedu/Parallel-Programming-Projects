//
// Created by Marco on 30/07/2024.
//

#include <algorithm>
#include <utility>
#include "PointsSoA.h"

std::vector<double> lineToVec(std::string &line){
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

Points8D linetoPoints8D(const std::string& filename){

    std::ifstream Infile("clusters/"+filename+".csv");
    int fileLength= (int)std::count(std::istreambuf_iterator<char>(Infile),
                                    std::istreambuf_iterator<char>(), '\n');
    std::cout<< "number of points "<< fileLength -1<<std::endl;

    std::string line;
    std::ifstream file("clusters/"+filename+".csv");
    if (!file.is_open())
    {
        printf("Error: Failed to open file. \n");
    }

    // Fetching points from file
    struct Points8D points{};
    points.num_points = fileLength -1;
    points.clusterId = (int *)malloc((fileLength-1)*sizeof(int));
    points.firstCoord = (double *)malloc((fileLength-1)*sizeof(double));
    points.secondCoord = (double *)malloc((fileLength-1)*sizeof(double));
    points.thirdCoord = (double *)malloc((fileLength-1)*sizeof(double));
    points.fourthCoord = (double *)malloc((fileLength-1)*sizeof(double));
    points.fifthCoord = (double *)malloc((fileLength-1)*sizeof(double));
    points.sixthCoord = (double *)malloc((fileLength-1)*sizeof(double));
    points.seventhCoord = (double *)malloc((fileLength-1)*sizeof(double));
    points.eighthsCoord = (double *)malloc((fileLength-1)*sizeof(double));

    int i=-1;
    while (getline(file, line))
    {   if(i>=0){ // skip the first line
            //initialize all the points clusters as -1 (no cluster)
            points.clusterId[i]= -1;
            //get the coordinates
            std::vector<double> tmpCoord = lineToVec(line);
            points.firstCoord[i] = tmpCoord[0];
            points.secondCoord[i]  = tmpCoord[1];
            points.thirdCoord[i]  = tmpCoord[2];
            points.fourthCoord[i]  = tmpCoord[3];
            points.fifthCoord[i]  = tmpCoord[4];
            points.sixthCoord[i]  = tmpCoord[5];
            points.seventhCoord[i]  = tmpCoord[6];
            points.eighthsCoord[i]  = tmpCoord[7];
        }
        i++;
    }

    file.close();
    printf("Data fetched successfully! \n");

    return  points;
}


Cluster::Cluster(int clusterId,  std::vector<double> centroid)
{
    this->clusterId = clusterId;
    this->centroid = std::move(centroid);
}


