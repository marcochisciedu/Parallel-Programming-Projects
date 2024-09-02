//
// Created by Marco on 29/07/2024.
//
#include "KMeansSoA.h"

#include "PointsSoA.h"

KMeansSoA::KMeansSoA(struct Points8D all_points, int K, int iterations){
    this->K = K;
    this->iters = iterations;
    this->points = all_points;
}


//reset all the points' assigned cluster id
void KMeansSoA::resetPointsClusters() {
    // reset clusters
    clusters.clear();

    //reset points
    for(int i =0; i<points.num_points; i++ ){
        points.clusterId[i]=-1;
    }
}

// given an index select the nearest cluster id
int KMeansSoA::getNearestClusterId(int index){
    // max number
    double min_dist= std::numeric_limits<double>::max();
    int NearestClusterId;
    // search the closest cluster to the point
    for (int i = 0; i < K; i++)
    {
        double dist, sum = 0.0;

        sum += pow(clusters[i].getCentroidCoordByPos(0) - points.firstCoord[index], 2.0);
        sum += pow(clusters[i].getCentroidCoordByPos(1) - points.secondCoord[index], 2.0);
        sum += pow(clusters[i].getCentroidCoordByPos(2) - points.thirdCoord[index], 2.0);
        sum += pow(clusters[i].getCentroidCoordByPos(3) - points.fourthCoord[index], 2.0);
        sum += pow(clusters[i].getCentroidCoordByPos(4) - points.fifthCoord[index], 2.0);
        sum += pow(clusters[i].getCentroidCoordByPos(5) - points.sixthCoord[index], 2.0);
        sum += pow(clusters[i].getCentroidCoordByPos(6) - points.seventhCoord[index], 2.0);
        sum += pow(clusters[i].getCentroidCoordByPos(7) - points.eighthsCoord[index], 2.0);

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
        for (int i = 0; i < 8; i++) {
            outfile << i+1 <<"° coordinate,";
        }
        outfile <<"clusterId" << std::endl;
        for (int p=0; p<points.num_points; p++) {

            outfile << points.firstCoord[p] << ",";
            outfile << points.secondCoord[p] << ",";
            outfile << points.thirdCoord[p] << ",";
            outfile << points.fourthCoord[p] << ",";
            outfile << points.fifthCoord[p] << ",";
            outfile << points.sixthCoord[p] << ",";
            outfile << points.seventhCoord[p] << ",";
            outfile << points.eighthsCoord[p] << ",";

            outfile << points.clusterId[p] << std::endl;
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

    coordinates.push_back(points.firstCoord[index]);
    coordinates.push_back(points.secondCoord[index]);
    coordinates.push_back(points.thirdCoord[index]);
    coordinates.push_back(points.fourthCoord[index]);
    coordinates.push_back(points.fifthCoord[index]);
    coordinates.push_back(points.sixthCoord[index]);
    coordinates.push_back(points.seventhCoord[index]);
    coordinates.push_back(points.eighthsCoord[index]);

    return  coordinates;
}

double KMeansSoA::runSeq( const std::string& output_dir, const std::string& original_filename){
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    // Initializing Clusters, select the first cluster
    int index = getRandomIndex((int)points.num_points, 111);
    // cluster numbered from 0 to k-1
    points.clusterId[index]=0;
    Cluster cluster(0, indexToCoordinates(index));
    clusters.push_back(cluster);

    std::vector<double>distances;
    // kmeans++ initialization for the other clusters
    for (int i = 1; i < K; i++){
        distances.clear();
        // find the closest existing cluster to each point, select the furthest
        for (int p =0; p<points.num_points; p++){
            double dist = 0.0;
            // iterate through all the previously found centroids
            for (int j = 0; j < i; j++) {
                double  sum = 0.0;

                sum += pow(clusters[j].getCentroidCoordByPos(0) - points.firstCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(1) - points.secondCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(2) - points.thirdCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(3) - points.fourthCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(4) - points.fifthCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(5) - points.sixthCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(6) - points.seventhCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(7) - points.eighthsCoord[p], 2.0);

                dist += sqrt(sum);

            }
            distances.push_back(dist);
        }
        // select the point that is the furthest from all the other clusters as a new centroid
        index = std::max_element(distances.begin(), distances.end())-distances.begin();
        points.clusterId[index]=i;
        Cluster cluster(i, indexToCoordinates(index));
        clusters.push_back(cluster);
    }
    printf( "Initialized %d clusters \n", (int)clusters.size() );

    printf("Running K-Means Clustering.. \n");

    for (int iter = 1; iter<=iters; iter++)
    {
        printf( "Iteration %d/%d \n", iter, iters);


        // Calculate each point's new cluster ID
        for (int i =0; i<points.num_points; i++)
        {
            int currentClusterId = points.clusterId[i];
            int nearestClusterId = getNearestClusterId(i);

            if (currentClusterId != nearestClusterId)
            {
                points.clusterId[i]= nearestClusterId;
            }
        }

        // Initialize the sums of each coordinate for each cluster
        std::vector<double> sum1, sum2, sum3, sum4, sum5, sum6, sum7, sum8;
        std::vector<int> nPoints;
        for(int c=0; c<K ; c++) {
            sum1.push_back(0);
            sum2.push_back(0);
            sum3.push_back(0);
            sum4.push_back(0);
            sum5.push_back(0);
            sum6.push_back(0);
            sum7.push_back(0);
            sum8.push_back(0);
            nPoints.push_back(0);
        }
        // Accumulate the sums of each cluster's coordinate
        for(int p = 0; p<points.num_points; p++ ){
            int clusterID = points.clusterId[p];
            sum1[clusterID]+= points.firstCoord[p];
            sum2[clusterID]+= points.secondCoord[p];
            sum3[clusterID]+= points.thirdCoord[p];
            sum4[clusterID]+= points.fourthCoord[p];
            sum5[clusterID]+= points.fifthCoord[p];
            sum6[clusterID]+= points.sixthCoord[p];
            sum7[clusterID]+= points.seventhCoord[p];
            sum8[clusterID]+= points.eighthsCoord[p];
            nPoints[clusterID]++;
        }
        // Recalculate the centroid of each cluster
        for(int c=0; c<K ; c++) {
            clusters[c].setCentroidByPos(0,sum1[c]/nPoints[c]);
            clusters[c].setCentroidByPos(1,sum2[c]/nPoints[c]);
            clusters[c].setCentroidByPos(2,sum3[c]/nPoints[c]);
            clusters[c].setCentroidByPos(3,sum4[c]/nPoints[c]);
            clusters[c].setCentroidByPos(4,sum5[c]/nPoints[c]);
            clusters[c].setCentroidByPos(5,sum6[c]/nPoints[c]);
            clusters[c].setCentroidByPos(6,sum7[c]/nPoints[c]);
            clusters[c].setCentroidByPos(7,sum8[c]/nPoints[c]);
        }

    }
    auto t2 = Clock::now();
    std::chrono::duration<double, std::milli> time = t2 - t1;
    printf("Clustering completed \n");

    createOutputFile(output_dir, original_filename, "seqSOA");

    return time.count();
}

double KMeansSoA::runParPrivate( const std::string& output_dir, const std::string& original_filename){
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();

    // Initializing Clusters, select the first cluster
    int index = getRandomIndex((int)points.num_points, 111);
    // cluster numbered from 0 to k-1
    points.clusterId[index]=0;
    Cluster cluster(0, indexToCoordinates(index));
    clusters.push_back(cluster);

    std::vector<double>distances;
    // kmeans++ initialization for the other clusters
    for (int i = 1; i < K; i++){
        distances.clear();
        // find the closest existing cluster to each point, select the furthest
        for (int p =0; p<points.num_points; p++){
            double dist = 0.0;
            // iterate through all the previously found centroids
            for (int j = 0; j < i; j++) {
                double  sum = 0.0;

                sum += pow(clusters[j].getCentroidCoordByPos(0) - points.firstCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(1) - points.secondCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(2) - points.thirdCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(3) - points.fourthCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(4) - points.fifthCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(5) - points.sixthCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(6) - points.seventhCoord[p], 2.0);
                sum += pow(clusters[j].getCentroidCoordByPos(7) - points.eighthsCoord[p], 2.0);

                dist += sqrt(sum);

            }
            distances.push_back(dist);
        }
        // select the point that is the furthest from all the other clusters as a new centroid
        index = std::max_element(distances.begin(), distances.end())-distances.begin();
        points.clusterId[index]=i;
        Cluster cluster(i, indexToCoordinates(index));
        clusters.push_back(cluster);
    }
    printf( "Initialized %d clusters \n", (int)clusters.size() );

    printf("Running K-Means Clustering.. \n");

    for (int iter = 1; iter<=iters; iter++)
    {
        printf( "Iteration %d/%d \n", iter, iters);
        std::vector<double> sum1Tot, sum2Tot, sum3Tot, sum4Tot, sum5Tot, sum6Tot, sum7Tot, sum8Tot;
        std::vector<int> nPointsTot;
        for (int c = 0; c < K; c++) {
            sum1Tot.push_back(0);
            sum2Tot.push_back(0);
            sum3Tot.push_back(0);
            sum4Tot.push_back(0);
            sum5Tot.push_back(0);
            sum6Tot.push_back(0);
            sum7Tot.push_back(0);
            sum8Tot.push_back(0);
            nPointsTot.push_back(0);
        }
#pragma omp parallel default(none) shared(points, clusters, sum1Tot, sum2Tot, sum3Tot, sum4Tot, sum5Tot, sum6Tot, sum7Tot, sum8Tot, nPointsTot)
        {
            // Calculate each point's new cluster ID
            #pragma omp for
            for (int i = 0; i < points.num_points; i++) {
                int currentClusterId = points.clusterId[i];
                int nearestClusterId = getNearestClusterId(i);

                if (currentClusterId != nearestClusterId) {
                    points.clusterId[i] = nearestClusterId;
                }
            }

            // Initialize the sums of each coordinate for each cluster
            std::vector<double> sum1, sum2, sum3, sum4, sum5, sum6, sum7, sum8;
            std::vector<int> nPoints;
            for (int c = 0; c < K; c++) {
                sum1.push_back(0);
                sum2.push_back(0);
                sum3.push_back(0);
                sum4.push_back(0);
                sum5.push_back(0);
                sum6.push_back(0);
                sum7.push_back(0);
                sum8.push_back(0);
                nPoints.push_back(0);
            }
            // Accumulate the sums of each cluster's coordinate
            #pragma omp for nowait
            for (int p = 0; p < points.num_points; p++) {
                int clusterID = points.clusterId[p];
                sum1[clusterID] += points.firstCoord[p];
                sum2[clusterID] += points.secondCoord[p];
                sum3[clusterID] += points.thirdCoord[p];
                sum4[clusterID] += points.fourthCoord[p];
                sum5[clusterID] += points.fifthCoord[p];
                sum6[clusterID] += points.sixthCoord[p];
                sum7[clusterID] += points.seventhCoord[p];
                sum8[clusterID] += points.eighthsCoord[p];
                nPoints[clusterID]++;
            }
            // collect all the private threads' result in the shared ones
            #pragma omp for
            for (int c = 0; c < K; c++) {
                #pragma omp atomic
                sum1Tot[c] = sum1Tot[c] + sum1[c];
                #pragma omp atomic
                sum2Tot[c] = sum2Tot[c] + sum2[c];
                #pragma omp atomic
                sum3Tot[c] = sum3Tot[c] + sum3[c];
                #pragma omp atomic
                sum4Tot[c] = sum4Tot[c] + sum4[c];
                #pragma omp atomic
                sum5Tot[c] = sum5Tot[c] + sum5[c];
                #pragma omp atomic
                sum6Tot[c] = sum6Tot[c] + sum6[c];
                #pragma omp atomic
                sum7Tot[c] = sum7Tot[c] + sum7[c];
                #pragma omp atomic
                sum8Tot[c] = sum8Tot[c] + sum8[c];

                #pragma omp atomic
                nPointsTot[c] = nPointsTot[c] + nPoints[c];
            }
            // Recalculate the centroid of each cluster
            for (int c = 0; c < K; c++) {
                clusters[c].setCentroidByPos(0, sum1Tot[c] / nPointsTot[c]);
                clusters[c].setCentroidByPos(1, sum2Tot[c] / nPointsTot[c]);
                clusters[c].setCentroidByPos(2, sum3Tot[c] / nPointsTot[c]);
                clusters[c].setCentroidByPos(3, sum4Tot[c] / nPointsTot[c]);
                clusters[c].setCentroidByPos(4, sum5Tot[c] / nPointsTot[c]);
                clusters[c].setCentroidByPos(5, sum6Tot[c] / nPointsTot[c]);
                clusters[c].setCentroidByPos(6, sum7Tot[c] / nPointsTot[c]);
                clusters[c].setCentroidByPos(7, sum8Tot[c] / nPointsTot[c]);
            }
        }
    }
    auto t2 = Clock::now();
    std::chrono::duration<double, std::milli> time = t2 - t1;
    printf("Clustering completed \n");

    createOutputFile(output_dir, original_filename, "par_priv_SOA");

    return time.count();
}




