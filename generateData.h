

#ifndef PARALLEL_PROGRAMMING_PROJECTS_GENERATEDATA_H
#define PARALLEL_PROGRAMMING_PROJECTS_GENERATEDATA_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

std::string generate_random_string(std::size_t length, int seed);

void generate_random_txt(const std::string& filename, std::size_t num_words, std::size_t length, int seed);

void generate_random_csv(const std::string& filename, std::size_t num_points, std::size_t dim, int seed);
#endif //PARALLEL_PROGRAMMING_PROJECTS_GENERATEDATA_H
