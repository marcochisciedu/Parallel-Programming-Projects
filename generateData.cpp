

#include "generateData.h"

// given the string length and a random seed generate a random string using the chosen characters
std::string generate_random_string(std::size_t length, int seed) {
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    //initialize random uniform distribution
    std::random_device random_device;
    std::mt19937 generator(random_device());
    generator.seed(seed);
    std::uniform_int_distribution<> distribution(0, (int)CHARACTERS.size() - 1);

    std::string random_string;

    for (std::size_t i = 0; i < length; ++i)
    {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}


// create a txt file with the chosen number of random strings
void generate_random_txt(const std::string& filename, std::size_t num_words, std::size_t length, int seed){
    // place the file in the correct directory
    std::ofstream outputFile("vocabularies/"+filename);

    if (outputFile.is_open()) {  // check if the file was successfully opened
        // write the strings into the file
        for(int i=0; i< num_words;i++) {
            outputFile << generate_random_string(length, seed+10*i) << "\n";
        }
        // close the file
        outputFile.close();
    }
    else
    {
        printf("Error: Unable to write to %s \n", filename.c_str());
    }
}

// given a string generate a txt with the selected percentage of very similar words
void generate_similar_txt(const std::string& filename, std::size_t num_words, std::size_t percentage, int seed){
    // place the file in the correct directory
    std::ofstream outputFile("vocabularies/"+filename);
    // all characters except d
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcefghijklmnopqrstuvwxyz";

    // given string
    std::string string= "dddddddddddddddddddddddddddddd";

    //initialize random uniform distribution
    std::random_device random_device;
    std::mt19937 generator(random_device());
    generator.seed(seed);
    std::uniform_int_distribution<> distribution_char(0, (int)CHARACTERS.size() - 1);
    std::uniform_int_distribution<> distribution(0, (int)string.size() - 1);

    // string that is not similar to the given string
    std::string default_string(string.size(), 'h');

    if (outputFile.is_open()) {  // check if the file was successfully opened
        for(int i=0; i< num_words;i++) {
            if(i < num_words * ((double)percentage/100)){
                // change one random character
                std::string tmp_string = string;
                tmp_string[distribution(generator)] = CHARACTERS[distribution_char(generator)];
                outputFile << tmp_string << "\n";
            }
            else {
                outputFile << default_string << "\n";
            }
        }
        // close the file
        outputFile.close();
    }
    else
    {
        printf("Error: Unable to write to %s \n", filename.c_str());
    }
}

//generate csv file with coordinates given the number of points and their dimension
void generate_random_csv(const std::string& filename, std::size_t num_points, std::size_t dim, int seed){
    // set random
    std::random_device random_device;
    std::mt19937 generator(random_device());
    generator.seed(seed);
    std::uniform_real_distribution<> distribution(-10000, 10000);

    // place the file in the correct directory
    std::ofstream outputFile("clusters/"+filename);

    if (outputFile.is_open()) {  // check if the file was successfully opened
        // write the first row of the file
        for(int k=0; k<dim; k++){
            outputFile << k+1<< "° coordinate,";
        }
        outputFile  << std::endl;
        // write the coordinates into the file
        for(int i=0; i< num_points;i++) {
            for(int k=0; k<dim; k++){
                outputFile<< std::fixed << distribution(generator)<< ",";
            }
            outputFile  << std::endl;
        }
        // close the file
        outputFile.close();
    }
    else
    {
        printf("Error: Unable to write to %s \n", filename.c_str());
    }
}