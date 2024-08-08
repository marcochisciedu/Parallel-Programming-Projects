//
// Created by Marco on 29/07/2024.
//
#include "KMeansSeq.h"

#include <utility>
#include "Cluster.h"

KMeansSeq::KMeansSeq(std::vector<Point> &all_points, int K, int iterations)
{
    this->K = K;
    this->iters = iterations;
    this->points = all_points;
    this->dimensions = points[0].getDimensions();
}

void KMeansSeq::clearClusters()
{
    for (int i = 0; i < K; i++)
    {
        clusters[i].removeAllPoints();
    }
}

int KMeansSeq::getNearestClusterId(const Point& point){
    // max number
    double min_dist= std::numeric_limits<double>::max();
    int NearestClusterId;
    // search the closest cluster to the point
    for (int i = 0; i < K; i++)
    {
        double dist, sum = 0.0;

        for (int j = 0; j < dimensions; j++)
        {
            sum += pow(clusters[i].getCentroidByPos(j) - point.getCoord(j), 2.0);
        }

        dist = sqrt(sum);

        if (dist < min_dist)
        {
            min_dist = dist;
            NearestClusterId = clusters[i].getId();
        }
    }
    return NearestClusterId;
}

void KMeansSeq::run( const std::string& output_dir, const std::string& original_filename){   // Initializing Clusters

    // get random number from uniform distribution with a seed, always the same results
    std::default_random_engine eng{static_cast<long unsigned int>(time(0))};
    std::uniform_int_distribution<> dis(0, (int)points.size()-1);
    // select the first cluster
    int index = dis(eng);
    // cluster numbered from 0 to k-1
    points[index].setCluster(0);
    Cluster cluster(0, points[index]);
    clusters.push_back(cluster);

    std::vector<double>distances;
    // kmeans++ initialization for the other clusters
    for (int i = 1; i < K; i++){
        distances.clear();
        // find the closest existing cluster to each point, select the furthest
        for (auto & point : points){
            double dist = 0.0;
            for (int j = 0; j < i; j++) {
                double sum = 0.0;
                for (int dim = 0; dim < dimensions; dim++) {
                    sum += pow(clusters[j].getCentroidByPos(dim) - point.getCoord(dim), 2.0);
                }
                dist += sqrt(sum);
            }
            distances.push_back(dist);
        }
        // select the point that is the furthest from all the other clusters as a new centroid
        index = std::max_element(distances.begin(), distances.end())-distances.begin();
        points[index].setCluster(i);
        Cluster cluster(i, points[index]);
        clusters.push_back(cluster);
    }
    printf( "Initialized %d clusters \n ", (int)clusters.size() );

    printf("Running K-Means Clustering.. \n");

    for (int iter = 1; iter<=iters; iter++)
    {
        printf( "Iteration %d/%d \n", iter, iters);

        // removes all points from the clusters (the centroids coordinates are not deleted)
        clearClusters();

        // Add all points to their nearest cluster
        for (auto & point : points)
        {
            int currentClusterId = point.getCluster();
            int nearestClusterId = getNearestClusterId(point);

            if (currentClusterId != nearestClusterId)
            {
                point.setCluster(nearestClusterId);
            }
            clusters[nearestClusterId].addPoint(point);
        }


        // Recalculating the centroid of each cluster
        for (int i = 0; i < K; i++)
        {
            int clusterSize = clusters[i].getSize();

            for (int j = 0; j < dimensions; j++)
            {
                double sum = 0.0;
                if (clusterSize > 0)
                {
                    for (int p = 0; p < clusterSize; p++)
                    {
                        sum += clusters[i].getPoint(p).getCoord(j);
                    }
                    clusters[i].setCentroidByPos(j, sum / clusterSize);
                }
            }
        }

    }
    printf("Clustering completed \n");

    // creat output file with coordinates and clusterId of each Point
    std::ofstream outfile;
    outfile.open(output_dir + "/"+ original_filename+ "_" + std::to_string(K) + "-clusters.csv");
    if (outfile.is_open()) {
        for (int i = 0; i < dimensions; i++) {
            outfile << i+1 <<"° coordinate,";
        }
        outfile <<"clusterId" << std::endl;
        for (auto &point: points) {
            for (int j = 0; j < dimensions; j++) {
                outfile << point.getCoord(j) << ",";
            }
            outfile << point.getCluster() << std::endl;
        }
        outfile.close();
    }
    else
    {
       printf("Error: Unable to write to clusters.csv \n");
    }
}


