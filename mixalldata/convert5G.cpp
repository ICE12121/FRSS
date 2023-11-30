#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::ifstream inputFile("data_5G.txt");  // Assuming the input is also a text file
    std::ofstream file1("5G-CV=0.1-UP=0.1.txt");
    std::ofstream file2("5G-CV=0.1-UP=0.5.txt");
    std::ofstream file3("5G-CV=0.1-UP=0.9.txt");
    std::ofstream file4("5G-CV=0.5-UP=0.1.txt");
    std::ofstream file5("5G-CV=0.5-UP=0.5.txt");
    std::ofstream file6("5G-CV=0.5-UP=0.9.txt");
    std::ofstream file7("5G-CV=0.9-UP=0.1.txt");
    std::ofstream file8("5G-CV=0.9-UP=0.5.txt");
    std::ofstream file9("5G-CV=0.9-UP=0.9.txt");

    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream ss(line);
        std::string token;
        std::vector<double> values;

        // Split the line by spaces or your specific delimiter
        while (std::getline(ss, token, ' ')) {  // Change ' ' to your delimiter if different
            values.push_back(std::stod(token));
        }

        double CV = values[0];
        double UP = values[1];

        // Check conditions and write to corresponding file
        // if (CV > 0 && CV < 0.5 && UP > 0 && UP < 0.5) {
        //     file1 << line << std::endl;
        // } else if (CV >= 0.5 && CV < 0.6 && UP >= 0.5 && UP < 0.6) {
        //     file2 << line << std::endl;
        // } else if (CV > 0.6 && UP > 0.6) {
        //     file3 << line << std::endl;
        // }

                // Check conditions and write to corresponding file
        if (CV > 0 && CV < 0.5 && UP > 0 && UP < 0.5) {
            file1 << line << std::endl;
        } else if (CV > 0 && CV < 0.5 && UP >= 0.5 && UP < 0.6) {
            file2 << line << std::endl;
        } else if (CV > 0 && CV < 0.5 && UP > 0.6) {
            file3 << line << std::endl;
        }


        else if (CV >= 0.5 && CV < 0.6 && UP > 0 && UP < 0.5) {
            file4 << line << std::endl;
        }
        else if (CV >= 0.5 && CV < 0.6 && UP >= 0.5 && UP < 0.6) {
            file5 << line << std::endl;
        }
        else if (CV >= 0.5 && CV < 0.6 && UP > 0.6) {
            file6 << line << std::endl;
        }


        else if (CV > 0.6 && UP > 0 && UP < 0.5) {
            file7 << line << std::endl;
        }
        else if (CV > 0.6 && UP >= 0.5 && UP < 0.6) {
            file8 << line << std::endl;
        }
        else if (CV > 0.6 && UP > 0.6) {
            file9 << line << std::endl;
        }
    }

    inputFile.close();
    file1.close();
    file2.close();
    file3.close();
    file4.close();
    file5.close();
    file6.close();
    file7.close();
    file8.close();
    file9.close();

    return 0;
}
