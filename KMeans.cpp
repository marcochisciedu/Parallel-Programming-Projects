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


//reset all the clusters and the points' assigned cluster id
void KMeans::resetPointsClusters() {
    // reset clusters
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
    Cluster cluster(0, points[index].getCoordinates());
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
        Cluster cluster(i, points[index].getCoordinates());
        clusters.push_back(cluster);
    }
    printf( "Initialized %d clusters \n", (int)clusters.size() );

    printf("Running K-Means Clustering.. \n");


    // Initialize the sums of each cluster and their number of points
    std::vector<std::vector<double> > sums(K,std::vector<double>(points.size()));
    std::vector<int> nPoints(K);

    for (int iter = 1; iter<=iters; iter++)
    {
        printf( "Iteration %d/%d \n", iter, iters);

        // Add all points to their nearest cluster and calculate the sums and nPoints
        for (auto & point : points)
        {
            int currentClusterId = point.getCluster();
            int nearestClusterId = getNearestClusterId(point);

            if (currentClusterId != nearestClusterId)
            {
                point.setCluster(nearestClusterId);
            }
            for (int dim = 0; dim < dimensions; dim++) {
                sums[nearestClusterId][dim] += point.getCoord(dim);
            }
            nPoints[nearestClusterId]++;
        }

        // Recalculate the centroid of each cluster and reset sums and nPoints
        for(int c=0; c<K ; c++) {
            for (int dim = 0; dim < dimensions; dim++) {
                clusters[c].setCentroidByPos(dim, sums[c][dim]/nPoints[c]);
                sums[c][dim] = 0;
            }
            nPoints[c] = 0;
        }

    }
    auto t2 = Clock::now();
    std::chrono::duration<double, std::milli> time = t2 - t1;
    printf("Clustering completed \n");

    createOutputFile(output_dir, original_filename, "seq");

    return time.count();
}



double KMeans::runPar( const std::string& output_dir, const std::string& original_filename, int threads){
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    // Initializing Clusters, select the first cluster
    int index = getRandomIndex((int)points.size(), 111);
    // cluster numbered from 0 to k-1
    points[index].setCluster(0);
    Cluster cluster(0, points[index].getCoordinates());
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
        Cluster cluster(i, points[index].getCoordinates());
        clusters.push_back(cluster);
    }
    printf( "Initialized %d clusters \n", (int)clusters.size() );

    printf("Running K-Means Clustering.. \n");

    std::vector<std::vector<double> > sums(K,std::vector<double>(points.size()));
    std::vector<int> nPoints(K);

    for (int iter = 1; iter<=iters; iter++) {
        printf("Iteration %d/%d \n", iter, iters);

#pragma omp parallel default(none) shared(sums, nPoints, points, clusters,dimensions) num_threads(threads)
        {   // Add all points to their nearest cluster and accumulate the sums of each cluster's coordinates
            // parallelize for cycle
            #pragma omp for
            for (auto &point: points) {
               int currentClusterId = point.getCluster();
               int nearestClusterId = getNearestClusterId(point);

                if (currentClusterId != nearestClusterId) {
                    point.setCluster(nearestClusterId);
                }

                for (int dim = 0; dim < dimensions; dim++) {
                    // synchronize access to the shared sums matrix
                    #pragma omp flush
                    #pragma omp atomic
                    sums[nearestClusterId][dim] += point.getCoord(dim);
                    #pragma omp flush
                }
                // synchronize access to the shared nPoints vector
                #pragma omp flush
                #pragma omp atomic
                nPoints[nearestClusterId]++;
                #pragma omp flush
            }

            // Recalculate the centroid of each cluster and reset sums and nPoints
            #pragma omp for
            for (int c = 0; c < K; c++) {
                for (int dim = 0; dim < dimensions; dim++) {
                    clusters[c].setCentroidByPos(dim, sums[c][dim] / nPoints[c]);
                    sums[c][dim] = 0;
                }
                nPoints[c] = 0;
            }

        }
    }
    auto t2 = Clock::now();
    std::chrono::duration<double, std::milli> time = t2 - t1;

    printf("Clustering completed \n");

    createOutputFile(output_dir, original_filename, "par");

    return time.count();
}

double KMeans::runParPrivate( const std::string& output_dir, const std::string& original_filename, int threads){
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    // Initializing Clusters, select the first cluster
    int index = getRandomIndex((int)points.size(), 111);
    // cluster numbered from 0 to k-1
    points[index].setCluster(0);
    Cluster cluster(0, points[index].getCoordinates());
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
        Cluster cluster(i, points[index].getCoordinates());
        clusters.push_back(cluster);
    }

    printf( "Initialized %d clusters \n", (int)clusters.size() );

    printf("Running K-Means Clustering.. \n");

    // Initialize the sums of each cluster and their number of points
    std::vector<std::vector<double> > sums(K,std::vector<double>(dimensions));
    std::vector<int> nPoints(K);

    for (int iter = 1; iter<=iters; iter++)
    {
        printf( "Iteration %d/%d \n", iter, iters);
#pragma omp parallel default(none) shared(points, clusters, sums, nPoints) num_threads(threads)
        {   // Each thread has its on sums and nPoints
            std::vector<std::vector<double> > sumsPrivate(K,std::vector<double>(dimensions));
            std::vector<int> nPointsPrivate(K);

            // Calculate each point's new cluster ID and accumulate the private sums of each cluster's coordinates
            #pragma omp for nowait
            for (auto &point: points) {
                int currentClusterId = point.getCluster();
                int nearestClusterId = getNearestClusterId(point);

                if (currentClusterId != nearestClusterId) {
                    point.setCluster(nearestClusterId);
                }
                for (int dim = 0; dim < dimensions; dim++) {
                    sumsPrivate[nearestClusterId][dim] += point.getCoord(dim);
                }
                nPointsPrivate[nearestClusterId]++;
            }

            // Accumulate all the private sums and nPoints, synchronize after each threads finishes accumulating the private sums
            for (int c = 0; c < K; c++) {
                for (int dim = 0; dim < dimensions; dim++) {
                    #pragma omp flush
                    #pragma omp atomic
                    sums[c][dim] += sumsPrivate[c][dim];
                    #pragma omp flush
                }
                #pragma omp flush
                #pragma omp atomic
                nPoints[c] += nPointsPrivate[c];
                #pragma omp flush
            }

            // wait until all the sums and nPoints have been calculated
            #pragma omp barrier

            // Recalculate the centroid of each cluster and reset sums and nPoints
            #pragma omp for
            for (int c = 0; c < K; c++) {
                for (int dim = 0; dim < dimensions; dim++) {
                    clusters[c].setCentroidByPos(dim, sums[c][dim] / nPoints[c]);
                    sums[c][dim] = 0;
                }
                nPoints[c] = 0;
            }
        }
    }
    auto t2 = Clock::now();
    std::chrono::duration<double, std::milli> time = t2 - t1;
    printf("Clustering completed \n");

    createOutputFile(output_dir, original_filename, "par_priv");

    return time.count();
}

