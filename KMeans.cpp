//
// Created by Marco on 29/07/2024.
//
#include "KMeans.h"

#include "Cluster.h"

KMeans::KMeans(std::vector<Point> &all_points, int K, int iterations){
    this->K = K;
    this->iters = iterations;
    this->points = all_points;
    this->dimensions = points[0].getDimensions();
}

//remove all points from each cluster
void KMeans::clearClusters(){
    for (int i = 0; i < K; i++)
    {
        clusters[i].removeAllPoints();
    }
}

//reset all the clusters and the points' assigned cluster id
void KMeans::resetPointsClusters() {
    // reset clusters
    clearClusters();
    clusters.clear();
    // reset points
    for(auto & point: points){
        point.setCluster(-1);
    }
}

// given a point select the nearest cluster id
int KMeans::getNearestClusterId(const Point& point){
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

// create cvs output file with all the points and their cluster id
void KMeans::createOutputFile(const std::string& output_dir, const std::string& original_filename, const std::string& context){
    std::ofstream outfile;
    outfile.open(output_dir + "/"+ original_filename+ "_"+ context+"_" + std::to_string(K) + "-clusters.csv");
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

// get random number from uniform distribution given a seed and the points' size
int getRandomIndex(int size, int seed){
    std::random_device rd;
    std::default_random_engine eng(rd());
    eng.seed(seed);
    std::uniform_int_distribution<> dis(0, size-1);

    return dis(eng);
}


double KMeans::runSeq( const std::string& output_dir, const std::string& original_filename){
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    // Initializing Clusters, select the first cluster
    int index = getRandomIndex((int)points.size(), 111);
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
    printf( "Initialized %d clusters \n", (int)clusters.size() );

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
    auto t2 = Clock::now();
    std::chrono::duration<double, std::milli> time = t2 - t1;
    printf("Clustering completed \n");

    createOutputFile(output_dir, original_filename, "seq");

    return time.count();
}

// structure to append points given a cluster id
struct fakeCluster{
    int clusterId;
    std::vector<Point> points;
};

double KMeans::runPar( const std::string& output_dir, const std::string& original_filename){
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    // Initializing Clusters, select the first cluster
    int index = getRandomIndex((int)points.size(), 111);
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
    printf( "Initialized %d clusters \n", (int)clusters.size() );

    printf("Running K-Means Clustering.. \n");

    for (int iter = 1; iter<=iters; iter++) {
        printf("Iteration %d/%d \n", iter, iters);

        // removes all points from the clusters (the centroids coordinates are not deleted)
        clearClusters();

#pragma omp parallel default(none) shared(points, clusters,dimensions)
        {
            // Add all points to their nearest cluster
            // each thread can handle its points and has its local clusters (fake, we don't need all their functions)
            int currentClusterId, nearestClusterId;
            std::vector<fakeCluster> localClusters;
            for (int i = 0; i < K; i++){
                fakeCluster local_cluster= {i, };
                localClusters.push_back(local_cluster);
            }
            // parallelize for cycle,
            #pragma omp for nowait
            for (auto &point: points) {
                currentClusterId = point.getCluster();
                nearestClusterId = getNearestClusterId(point);

                if (currentClusterId != nearestClusterId) {
                    point.setCluster(nearestClusterId);
                }

                localClusters[nearestClusterId].points.push_back(point);
            }
            // synchronize the local clusters, add the points to the real clusters
            #pragma omp critical
            {       for (int i = 0; i < K; i++){
                        clusters[i].addPoints(localClusters[i].points);
                }
            }

            #pragma omp barrier

            // Recalculating the centroid of each cluster
            int clusterSize;
            // parallelize for, each thread takes a cluster
            #pragma omp for
            for (int i = 0; i < K; i++) {
                clusterSize = clusters[i].getSize();

                for (int j = 0; j < dimensions; j++) {
                    double sum = 0.0;
                    if (clusterSize > 0) {
                        for (int p = 0; p < clusterSize; p++) {
                            sum += clusters[i].getPoint(p).getCoord(j);
                        }
                        clusters[i].setCentroidByPos(j, sum / clusterSize);
                    }
                }
            }

        }
    }
    auto t2 = Clock::now();
    std::chrono::duration<double, std::milli> time = t2 - t1;

    printf("Clustering completed \n");

    createOutputFile(output_dir, original_filename, "par");

    return time.count();
}

double KMeans::runParReduction( const std::string& output_dir, const std::string& original_filename){
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    // Initializing Clusters, select the first cluster
    int index = getRandomIndex((int)points.size(), 111);
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
    printf( "Initialized %d clusters \n", (int)clusters.size() );

    printf("Running K-Means Clustering.. \n");

    for (int iter = 1; iter<=iters; iter++) {
        printf("Iteration %d/%d \n", iter, iters);

        // removes all points from the clusters (the centroids coordinates are not deleted)
        clearClusters();

#pragma omp parallel default(none) shared(points, clusters,dimensions)
        {   // Add all points to their nearest cluster
            // each thread can handle its points and has its local clusters (fake, we don't need all their functions)
            int currentClusterId, nearestClusterId;
            std::vector<fakeCluster> localClusters;
            for (int i = 0; i < K; i++) {
                fakeCluster local_cluster = {i,};
                localClusters.push_back(local_cluster);
            }
            // parallelize for cycle,
            #pragma omp for nowait
            for (auto &point: points) {
                currentClusterId = point.getCluster();
                nearestClusterId = getNearestClusterId(point);

                if (currentClusterId != nearestClusterId) {
                    point.setCluster(nearestClusterId);
                }

                localClusters[nearestClusterId].points.push_back(point);
            }
            // synchronize the local clusters, add the points to the real clusters
            #pragma omp critical
            {
                for (int i = 0; i < K; i++) {
                    clusters[i].addPoints(localClusters[i].points);
                }
            }
        }

            // Recalculating the centroid of each cluster
            int clusterSize;
            for (int i = 0; i < K; i++) {
                clusterSize = clusters[i].getSize();

                for (int j = 0; j < dimensions; j++) {
                    double sum = 0.0;
                    if (clusterSize > 0) {
                        // use reduction to calculate the sum using all the threads
                        #pragma omp parallel for default(none) shared(clusterSize) firstprivate(i,j) reduction(+: sum)
                        for (int p = 0; p < clusterSize; p++) {
                            sum += clusters[i].getPoint(p).getCoord(j);
                        }
                        clusters[i].setCentroidByPos(j, sum / clusterSize);
                    }
                }
            }

        }

    auto t2 = Clock::now();
    std::chrono::duration<double, std::milli> time = t2 - t1;

    printf("Clustering completed \n");

    createOutputFile(output_dir, original_filename, "par_red");

    return time.count();
}

