

#ifndef PARALLEL_PROGRAMMING_PROJECTS_CHECKVOCABULARYSEQ_H
#define PARALLEL_PROGRAMMING_PROJECTS_CHECKVOCABULARYSEQ_H
#include "LevenshteinDistance.h"

#include <iostream>
#include <fstream>
#include <string>

void checkVocabularySeq(const std::string& string, const std::vector<std::string>& words, int threads, bool display);

#endif //PARALLEL_PROGRAMMING_PROJECTS_CHECKVOCABULARYSEQ_H
