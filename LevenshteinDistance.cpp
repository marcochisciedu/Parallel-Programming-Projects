
#include "LevenshteinDistance.h"

int min(int x, int y, int z) { return std::min(std::min(x, y), z); }

int levenshteinDistanceSeq(const std::string& str1, const int length1, const std::string& str2, const int length2){
    // Create a table to store results of subproblems
    int table[length1 + 1][length2 + 1];

    // Fill the table
    for (int i = 0; i <= length1; i++) {
        for (int j = 0; j <= length2; j++) {
            // First row contains the cost of inserting all the characters
            if (i == 0)
                table[i][j] = j;
            // First column contains the cost of deleting all the characters
            else if (j == 0)
                table[i][j] = i;

            // If the current characters are the same no additional cost is added
            else if (str1[i - 1] == str2[j - 1])
                table[i][j] = table[i - 1][j - 1];

            // If the currents characters are different choose the least expensive operation
            else
                table[i][j]
                        = 1
                          + min(table[i][j - 1], // Insert
                                table[i - 1][j], // Remove
                                table[i - 1][j - 1]); // Replace
        }
    }

    return table[length1][length2];
}