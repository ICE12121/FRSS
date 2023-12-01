#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

struct Row {
    double CV;
    double UP; // Assuming you might need these values
    double SE;
    double RDV; // Assuming you might need these values
};

int main() {
    std::ifstream inputFile("data_4G-raw.txt");
    std::ofstream outputFile("data_4G.txt");
    std::string line;
    std::vector<Row> data;

    // Read data from file
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        Row row;
        if (iss >> row.CV >> row.UP >> row.SE >> row.RDV) {
            data.push_back(row);
        }
    }

    // Sort data based on CV and SE
    std::sort(data.begin(), data.end(), [](const Row& a, const Row& b) {
        if (a.CV == b.CV) {
            return a.SE < b.SE;
        }
        return a.CV < b.CV;
    });

    // Write sorted data to file or console
    for (const auto& row : data) {
        outputFile << row.CV << " " << row.UP << " " << row.SE << " " << row.RDV << std::endl;
    }

    inputFile.close();
    outputFile.close();
    return 0;
}
