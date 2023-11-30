#include <ambigu.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

// Declare the Fuzzy_decision function
double Fuzzy_decision(double CV, double UP, double SE);

int main() {
    FILE *fp;
    double CV, UP, SE, RAT;

    // Open the text file for reading
    fp = fopen("data_5G-raw.txt", "r");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    std::ofstream dataFile5Gall("data_5G.txt");

    // Check if the file stream is open/valid.
    if (!dataFile5Gall) {
        std::cerr << "Cannot open the output file!" << std::endl;
        return 1;
    }

    // Read and process each line in the file
    while (fscanf(fp, "%lf %lf %lf", &CV, &UP, &SE) == 3) {
        // Call Fuzzy_decision function with the read values
        RAT = Fuzzy_decision(CV, UP, SE);

        // Print the result for each set of values
        printf("Result: %.4f\n", RAT);

        // Save the parameters and result in the file
        dataFile5Gall << CV << " " << UP << " " << SE << " " << RAT << "\n";
    }

    // Close the file
    fclose(fp);

    // Close the output file
    dataFile5Gall.close();

    return 0;
}

double Fuzzy_decision(double CV, double UP, double SE) {
    double RAT = 0;

    ambgInit();

    // Call ambgRun with the provided CV, UP, and SE values
    RAT = ambgRun(3, CV, UP, SE);

    ambgDestroy();

    return RAT;
}
