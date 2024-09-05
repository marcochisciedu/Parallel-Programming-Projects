
#ifndef PARALLEL_PROGRAMMING_PROJECTS_CHECKVOCABULARYPAR_H
#define PARALLEL_PROGRAMMING_PROJECTS_CHECKVOCABULARYPAR_H

#include "LevenshteinDistance.h"

#include <iostream>
#include <fstream>
#include <string>

void checkVocabularyParCrit(const std::string& string, const std::vector<std::string>& words, int threads, bool display);

void checkVocabularyParPrivate(const std::string& string, const std::vector<std::string>& words, int threads, bool display);
#endif //PARALLEL_PROGRAMMING_PROJECTS_CHECKVOCABULARYPAR_H
