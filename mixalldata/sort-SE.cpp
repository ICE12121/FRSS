#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

struct Data {
    double CV, UP, SE, RDV;
};

bool compareBySE(const Data &a, const Data &b) {
    return a.SE < b.SE;
}

int main() {
    std::vector<std::string> fileNames = {
        "5G-CV=0.1-UP=0.1.txt", "5G-CV=0.1-UP=0.5.txt", "5G-CV=0.1-UP=0.9.txt", "5G-CV=0.5-UP=0.1.txt", "5G-CV=0.5-UP=0.5.txt", "5G-CV=0.5-UP=0.9.txt", "5G-CV=0.9-UP=0.1.txt", "5G-CV=0.9-UP=0.5.txt", "5G-CV=0.9-UP=0.9.txt", "4G-CV=0.1-UP=0.1.txt", "4G-CV=0.1-UP=0.5.txt", "4G-CV=0.1-UP=0.9.txt", "4G-CV=0.5-UP=0.1.txt", "4G-CV=0.5-UP=0.5.txt", "4G-CV=0.5-UP=0.9.txt", "4G-CV=0.9-UP=0.1.txt", "4G-CV=0.9-UP=0.5.txt", "4G-CV=0.9-UP=0.9.txt", "WIFI-CV=0.1-UP=0.1.txt", "WIFI-CV=0.1-UP=0.5.txt", "WIFI-CV=0.1-UP=0.9.txt", "WIFI-CV=0.5-UP=0.1.txt", "WIFI-CV=0.5-UP=0.5.txt", "WIFI-CV=0.5-UP=0.9.txt", "WIFI-CV=0.9-UP=0.1.txt", "WIFI-CV=0.9-UP=0.5.txt", "WIFI-CV=0.9-UP=0.9.txt"
    };

    for (const auto& fileName : fileNames) {
        std::ifstream file(fileName);
        std::vector<Data> dataList;
        Data temp;

        // Read data from file
        while(file >> temp.CV >> temp.UP >> temp.SE >> temp.RDV) {
            dataList.push_back(temp);
        }
        file.close();

        // Sort data
        std::sort(dataList.begin(), dataList.end(), compareBySE);

        // Write sorted data to a file
        std::ofstream outFile(fileName);
        for(const auto& data : dataList){
            outFile << data.CV << " " << data.UP << " " << data.SE << " " << data.RDV << std::endl;
        }
        outFile.close();
    }

    return 0;
}

// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <algorithm>
// #include <string>

// struct Data {
//     double CV, UP, SE, RDV;
// };

// bool compareBySE(const Data &a, const Data &b) {
//     return a.SE < b.SE;
// }

// int main() {
//     std::ifstream file("5G-CV=0.1-UP=0.1.txt");
//     std::vector<Data> dataList;
//     Data temp;

//     // Read data from file
//     while(file >> temp.CV >> temp.UP >> temp.SE >> temp.RDV) {
//         dataList.push_back(temp);
//     }
//     file.close();

//     // Sort data
//     std::sort(dataList.begin(), dataList.end(), compareBySE);

//     // Write sorted data to a file
//     std::ofstream outFile("5G-CV=0.1-UP=0.1.txt");
//     for(const auto& data : dataList){
//         outFile << data.CV << " " << data.UP << " " << data.SE << " " << data.RDV << std::endl;
//     }
//     outFile.close();

//     return 0;
// }
