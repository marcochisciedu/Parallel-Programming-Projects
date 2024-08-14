# Parallel Programming Projects

## Generate data
generateData.h and generateData.cpp hold the code to generate csv files with random coordinates and txt files with random strings that are used to test the following algorithms.

I also used some coordinates and vocabularies found online to do some tests (csv files found in clusters, txt files in vocabularies).

## Kmeans
Cluster.h and Cluster.cpp contain the definition and implementation of the classes Point and Cluster that are used in the Kmeans algorithm.

In Kmeans.h and Kmeans.cpp the algorithm is implemented as a class with its parameters (k, the number of iterations to cluster), all the points, the clusters and three different run methods.
runSeq() runs a sequential version of Kmeans, while the other two run methods are two different versions of parallel Kmeans using OpenMP.

plotKmeans.py is a small fragment of code that prints all the points color-coded given their final cluster id. It was used to produce 2D visualizations of clustering (saved in images) to show that the K-means algorithm is correct.

Finally, testKmeans.cpp is the executable file that compares the three different kmeans implementations with different points, numbers of clusters and iterations multiple times and calculates the average time that each run method takes to complete.
The average speedup of each parallel method given the sequential one is calculated and saved in a txt file holding all the experiment results.

To prove that the parallel versions are correct (no race conditions), the final clusters of each point, given the sequential or parallel kmeans, are compared.

## Levenshtein distance
LevenshteinDistance.h and LevenshteinDistance.cpp contain a simple implementation of the algorithm that calculates the distance between two given words.

checkVocabularySeq.h and checkVocabularySeq.cpp are a sequential definition and implementation of checking which is the closest word (or words) and its distance from an input string in a vocabulary.

checkVocabularyPar.h and checkVocabularyPar.cpp are the same, but with two different parallel versions of vocabulary check using OpenMP.

Lastly, testLevenshteinDistance.cpp is the executable file that compares the three different ways to check different vocabularies and different input strings.
The average completion time and parallelization speedup of each vocabulary check are calculated over multiple iterations and saved in a txt file.

If the parallel vocabulary check finds the same closest word(s) and the same distance given the same input string and vocabulary, it is correct.