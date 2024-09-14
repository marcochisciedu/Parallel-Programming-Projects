//
// Created by Marco on 29/07/2024.
//
#include "KMeansSoA.h"

#include "PointsSoA.h"

//reset all the points' assigned cluster id
void KMeansSoA::resetPointsClusters() {
    // reset clusters
    clusters.clear();

    //reset points
    for(int i =0; i<points.getNumPoints(); i++ ){
        points.setClusterIdByIndex(i, -1);
    }
}

// given an index select the nearest cluster id
int KMeansSoA::getNearestClusterId(int index){
    // max number
    double min_dist= std::numeric_limits<double>::max();
    int NearestClusterId;
    // find the point closest cluster
    for (int i = 0; i < K; i++)
    {double dist, sum = 0.0;
        for (int j = 0; j < points.getNumDimensions(); j++)
        {
            sum += pow(clusters[i].getCentroidCoordByPos(j) - points(index,j), 2.0);
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

// create csv output file with all the points and their cluster id
void KMeansSoA::createOutputFile(const std::string& output_dir, const std::string& original_filename, const std::string& context) const{
    std::ofstream outfile;
    outfile.open(output_dir + "/"+ original_filename+ "_"+ context+"_" + std::to_string(K) + "-clusters.csv");
    if (outfile.is_open()) {
        // print name of the columns
        for (int i = 0; i < points.getNumDimensions(); i++) {
            outfile << i+1 <<"° coordinate,";
        }
        outfile <<"clusterId" << std::endl;

        for (int p=0; p<points.getNumPoints(); p++) {
            for (int j = 0; j < points.getNumDimensions(); j++) {
                outfile << points(p,j) << ",";
            }
            outfile << points.getClusterIdByIndex(p) << std::endl;
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

// given an index (a point) push its coordinate in a vector
std::vector<double> KMeansSoA::indexToCoordinates(int index) const{
    std::vector<double> coordinates;

    for (int j = 0; j < points.getNumDimensions(); j++) {
        coordinates.push_back(points(index,j));
    }

    return  coordinates;
}

double KMeansSoA::runSeq( const std::string& output_dir, const std::string& original_filename){
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    // Initializing Clusters, select the first cluster
    int index = getRandomIndex((int)points.getNumPoints(), 111);
    // cluster numbered from 0 to k-1
    points.setClusterIdByIndex(index, 0);
    Cluster cluster(0, indexToCoordinates(index));
    clusters.push_back(cluster);

    std::vector<double>distances;
    // kmeans++ initialization for the other clusters
    for (int i = 1; i < K; i++){
        distances.clear();
        // find the closest existing cluster to each point, select the furthest
        for (int p =0; p<points.getNumPoints(); p++){
            double dist = 0.0;
            // iterate through all the previously found centroids
            for (int j = 0; j < i; j++) {
                double  sum = 0.0;
                for (int dim = 0; dim < points.getNumDimensions(); dim++) {
                    sum += pow(clusters[j].getCentroidCoordByPos(dim) - points(p,dim), 2.0);
                }
                dist += sqrt(sum);
            }
            distances.push_back(dist);
        }
        // select the point that is the furthest from all the other clusters as a new centroid
        index = std::max_element(distances.begin(), distances.end())-distances.begin();
        points.setClusterIdByIndex(index,i);
        Cluster cluster(i, indexToCoordinates(index));
        clusters.push_back(cluster);
    }
    printf( "Initialized %d clusters \n", (int)clusters.size() );

    // Initialize the sums of each cluster's coordinates and their number of points
    std::vector<std::vector<double> > sums(K,std::vector<double>(points.getNumDimensions()));
    std::vector<int> nPoints(K);

    for (int iter = 1; iter<=iters; iter++)
    {
        //printf( "Iteration %d/%d \n", iter, iters);

        // Calculate each point's new cluster ID and accumulate the sums of each cluster's coordinates
        for (int p =0; p<points.getNumPoints(); p++)
        {
            int currentClusterId = points.getClusterIdByIndex(p);
            int nearestClusterId = getNearestClusterId(p);

            if (currentClusterId != nearestClusterId)
            {
                points.setClusterIdByIndex(p, nearestClusterId);
            }
            for (int dim = 0; dim < points.getNumDimensions(); dim++) {
                sums[nearestClusterId][dim] += points(p,dim);
            }
            nPoints[nearestClusterId]++;
        }

        // Recalculate the centroid of each cluster, and reset sums and nPoints
        for(int c=0; c<K ; c++) {
            for (int dim = 0; dim < points.getNumDimensions(); dim++) {
                clusters[c].setCentroidByPos(dim, sums[c][dim]/nPoints[c]);
                sums[c][dim]=0;
            }
            nPoints[c]=0;
        }
    }
    auto t2 = Clock::now();
    std::chrono::duration<double, std::milli> time = t2 - t1;
    printf("Clustering completed \n");

    createOutputFile(output_dir, original_filename, "seqSOA");

    return time.count();
}

double KMeansSoA::runPar( const std::string& output_dir, const std::string& original_filename, int threads){
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    // Initializing Clusters, select the first cluster
    int index = getRandomIndex((int)points.getNumPoints(), 111);
    // cluster numbered from 0 to k-1
    points.setClusterIdByIndex(index, 0);
    Cluster cluster(0, indexToCoordinates(index));
    clusters.push_back(cluster);

    std::vector<double>distances;
    std::vector<int>indices;
    // kmeans++ initialization for the other clusters
    for (int i = 1; i < K; i++){
        distances.clear();
        indices.clear();
        // find the closest existing cluster to each point, select the furthest
#pragma omp parallel default(none) shared(distances,i, points, clusters, indices)  num_threads(threads)
        {   std::vector<double>privateDistances;
            std::vector<int>privateIndices;
            #pragma omp for
            for (int p = 0; p < points.getNumPoints(); p++) {
                double dist = 0.0;
                // iterate through all the previously found centroids
                for (int j = 0; j < i; j++) {
                    double sum = 0.0;
                    for (int dim = 0; dim < points.getNumDimensions(); dim++) {
                        sum += pow(clusters[j].getCentroidCoordByPos(dim) - points(p, dim), 2.0);
                    }
                    dist += sqrt(sum);
                }
                privateDistances.push_back(dist);
                // collect the local indices to know the original index of each distance
                privateIndices.push_back(p);
            }
            // synchronize all the threads, collect all the distances and their original indices
            #pragma omp critical
            {
                distances.insert(distances.end(), privateDistances.begin(), privateDistances.end());
                indices.insert(indices.end(), privateIndices.begin(), privateIndices.end());
            }
        }
        // select the point that is the furthest from all the other clusters as a new centroid
        index = std::max_element(distances.begin(), distances.end())-distances.begin();
        points.setClusterIdByIndex(indices[index],i);
        Cluster cluster(i, indexToCoordinates(indices[index]));
        clusters.push_back(cluster);
    }
    printf( "Initialized %d clusters \n", (int)clusters.size() );

    // Initialize the sums of each cluster's coordinates and their number of points
    std::vector<std::vector<double> > sums(K,std::vector<double>(points.getNumDimensions()));
    std::vector<int> nPoints(K);

    for (int iter = 1; iter<=iters; iter++)
    {   //printf( "Iteration %d/%d \n", iter, iters);

#pragma omp parallel default(none) shared(points, clusters, sums, nPoints) num_threads(threads)
        {   // Calculate each point's new cluster ID and accumulate the sums of each cluster's coordinates
            // each threads takes a part of the points
            #pragma omp for
            for (int p = 0; p < points.getNumPoints(); p++) {
                int currentClusterId = points.getClusterIdByIndex(p);
                int nearestClusterId = getNearestClusterId(p);

                if (currentClusterId != nearestClusterId) {
                    points.setClusterIdByIndex(p, nearestClusterId);
                }
                for (int dim = 0; dim < points.getNumDimensions(); dim++) {
                // synchronize access to the shared sums matrix
                    #pragma omp atomic update
                    sums[nearestClusterId][dim] += points(p, dim);
                }
                // synchronize access to the shared nPoints vector
                #pragma omp atomic update
                nPoints[nearestClusterId]++;
            }

            // Recalculate the centroid of each cluster and reset sums and nPoints
            #pragma omp for
            for (int c = 0; c < K; c++) {
                for (int dim = 0; dim < points.getNumDimensions(); dim++) {
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

    createOutputFile(output_dir, original_filename, "parSOA");

    return time.count();
}


double KMeansSoA::runParPrivate( const std::string& output_dir, const std::string& original_filename, int threads){
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    // Initializing Clusters, select the first cluster
    int index = getRandomIndex((int)points.getNumPoints(), 111);
    // cluster numbered from 0 to k-1
    points.setClusterIdByIndex(index, 0);
    Cluster cluster(0, indexToCoordinates(index));
    clusters.push_back(cluster);

    std::vector<double>distances;
    std::vector<int>indices;
    // kmeans++ initialization for the other clusters
    for (int i = 1; i < K; i++){
        distances.clear();
        indices.clear();
        // find the closest existing cluster to each point, select the furthest
#pragma omp parallel default(none) shared(distances,i, points, clusters, indices)  num_threads(threads)
        {   std::vector<double>privateDistances;
            std::vector<int>privateIndices;
            #pragma omp for
            for (int p = 0; p < points.getNumPoints(); p++) {
                double dist = 0.0;
                // iterate through all the previously found centroids
                for (int j = 0; j < i; j++) {
                    double sum = 0.0;
                    for (int dim = 0; dim < points.getNumDimensions(); dim++) {
                        sum += pow(clusters[j].getCentroidCoordByPos(dim) - points(p, dim), 2.0);
                    }
                    dist += sqrt(sum);
                }
                privateDistances.push_back(dist);
                // collect the local indices to know the original index of each distance
                privateIndices.push_back(p);
            }
            // synchronize all the threads, collect all the distances and their original indices
            #pragma omp critical
            {
                distances.insert(distances.end(), privateDistances.begin(), privateDistances.end());
                indices.insert(indices.end(), privateIndices.begin(), privateIndices.end());
            }
        }
        // select the point that is the furthest from all the other clusters as a new centroid
        index = std::max_element(distances.begin(), distances.end())-distances.begin();
        points.setClusterIdByIndex(indices[index],i);
        Cluster cluster(i, indexToCoordinates(indices[index]));
        clusters.push_back(cluster);
    }

    printf( "Initialized %d clusters \n", (int)clusters.size() );

    // Initialize the sums of each cluster's coordinates and their number of points
    std::vector<std::vector<double> > sums(K,std::vector<double>(points.getNumDimensions()));
    std::vector<int> nPoints(K);

    for (int iter = 1; iter<=iters; iter++)
    {
        //printf( "Iteration %d/%d \n", iter, iters);
#pragma omp parallel default(none) shared(points, clusters, sums, nPoints) num_threads(threads)
        {   // Each thread has its own private sums and nPoints
            std::vector<std::vector<double> > sumsPrivate(K,std::vector<double>(points.getNumDimensions()));
            std::vector<int> nPointsPrivate(K);

            // Calculate each point's new cluster ID and accumulate the private sums of each cluster's coordinates
            // each threads has a part of all the points
            #pragma omp for nowait
            for (int p = 0; p < points.getNumPoints(); p++) {
                int currentClusterId = points.getClusterIdByIndex(p);
                int nearestClusterId = getNearestClusterId(p);

                if (currentClusterId != nearestClusterId) {
                    points.setClusterIdByIndex(p, nearestClusterId);
                }
                for (int dim = 0; dim < points.getNumDimensions(); dim++) {
                    sumsPrivate[nearestClusterId][dim] += points(p, dim);
                }
                nPointsPrivate[nearestClusterId]++;
            }

            // Accumulate all the private sums and nPoints, synchronize after each threads finishes accumulating the private sums
            for (int c = 0; c < K; c++) {
                    for (int dim = 0; dim < points.getNumDimensions(); dim++) {
                    #pragma omp atomic update
                        sums[c][dim] += sumsPrivate[c][dim];
                    }
            #pragma omp atomic update
                nPoints[c] += nPointsPrivate[c];
            }

            // wait until all the sums and nPoints have been calculated
            #pragma omp barrier

            // Recalculate the centroid of each cluster and reset sums and nPoints
            #pragma omp for
            for (int c = 0; c < K; c++) {
                for (int dim = 0; dim < points.getNumDimensions(); dim++) {
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

    createOutputFile(output_dir, original_filename, "parPrivSOA");

    return time.count();
}

