#include <ambigu.h>

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <cstring>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include <sstream>
#include <vector>

#include <array>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <unistd.h> // Include the library for sleep function

#include <random>

/////////////////////////////////////////////////////////////////// Cellualr part ///////////////////////////////////////////////////////
// Function to open a serial port and configure its settings
int openSerialPort(const char *portName) {
    // Open the serial port in read-write mode and return a file descriptor
    int fd = open(portName, O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("Failed to open serial port");
        return -1;
    }

    // Configure serial port settings using termios
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(fd, &tty) != 0) {
        perror("Failed to get serial port attributes");
        return -1;
    }

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_lflag = 0;
    tty.c_oflag = 0;

    tcflush(fd, TCIFLUSH);

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("Failed to set serial port attributes");
        return -1;
    }

    return fd; // Return the file descriptor for the opened serial port
}

// Function to send an AT command to the serial port and get the response
std::string sendAtCommand(int fd) {
    std::string response;
    write(fd, "AT+CSQ\r\n", 8); // Send the AT command
    usleep(100000); // Sleep for 100ms to allow time for the response
    char buffer[1024];
    int bytesRead = read(fd, buffer, sizeof(buffer)); // Read the response
    if (bytesRead > 0) {
        response.assign(buffer, bytesRead); // Store the response in a string
    }
    return response; // Return the AT command response
}

// Function to calculate dBm (signal strength) from the AT command response
int calculateDbm(const std::string &csq) {
    size_t start = csq.find("+CSQ:");
    if (start != std::string::npos) {
        int rssi = std::stoi(csq.substr(start + 6)); // Extract RSSI value
        return -113 + (rssi * 2); // Calculate and return dBm
    }
    return -1; // Invalid signal strength
}

// Function to map dBm to connection status
std::string connectionStatus(int dbm) {
    if (dbm >= -73) {
        return "Excellent";
    } else if (dbm >= -83) {
        return "Good";
    } else if (dbm >= -93) {
        return "OK";
    } else if (dbm < -93) {
        return "Alert: Marginal";
    }
    return "Unknown";
}

/////////////////////////////////////////////////////////////////// WiFi part ///////////////////////////////////////////////////////
std::vector<int> getRSSIValues() {

    // Create a vector to store numeric RSSI values
    std::vector<int> rssi_values;

    // Run the iw command and capture its output
    std::string iw_command = "iw dev wlan0 station dump";
    FILE *pipe = popen(iw_command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error running the command." << std::endl;
        return rssi_values; // Return an empty vector
    }

    // Read and process each line of the command's output
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        std::string line(buffer);

        // Check if the line contains "signal:"
        size_t pos = line.find("signal:");
        if (pos != std::string::npos) {
            // Extract the RSSI value from the line
            std::istringstream iss(line.substr(pos + 7));
            int rssi;
            if (iss >> rssi) {
                rssi_values.push_back(rssi);
            }
        }
    }

    // Close the command's output pipe
    pclose(pipe);

    return rssi_values;
}

////////////////////////////////////////   Check Speed /////////////////////////////////////////////////////////////////
std::array<double, 3> CheckSpeed() {
    // Run the Speedtest CLI and capture its output
    std::string cmd = "speedtest-cli --simple";
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error: Couldn't open the Speedtest CLI process." << std::endl;
        // Return default values as doubles
        return { -1.0, -1.0, -1.0 };
    }

    // Read the output
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr) {
            result += buffer;
        }
    }

    // Close the Speedtest CLI process
    pclose(pipe);

    // Parse the result to extract ping and data rate values as doubles
    double ping = -1.0;         // Initialize with a default value
    double downloadRate = -1.0; // Initialize with a default value
    double uploadRate = -1.0;   // Initialize with a default value

    char *token = strtok(const_cast<char *>(result.c_str()), "\n");
    while (token != nullptr) {
        if (strstr(token, "Ping:")) {
            sscanf(token, "Ping: %lf.%*lf ms", &ping);
        } else if (strstr(token, "Download:")) {
            sscanf(token, "Download: %lf.%*lf Mbit/s", &downloadRate);
        } else if (strstr(token, "Upload:")) {
            sscanf(token, "Upload: %lf.%*lf Mbit/s", &uploadRate);
        }
        token = strtok(nullptr, "\n");
    }

    // Create and return a std::array of doubles
    return { ping, downloadRate, uploadRate };
}


///////////////////////////////////////////// Random function for user priority ////////////////////////////////////
float random_pri() {
    // Create a random number generator engine
    std::random_device rd;   // Use a hardware random device if available
    std::mt19937 gen(rd()); // Mersenne Twister engine for 32-bit integers

    // Define a uniform distribution for floating-point values between 0.0 and 1.0
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // Generate and print random values with a step of 0.1
    double randomValue;
    for (double value = 0.0; value <= 1.0; value += 0.1) {
        randomValue = distribution(gen);
        // std::cout << "Random Value (" << value << "): " << randomValue << std::endl;
    }
    return randomValue;
}



///////////////////   Fuzzy function /////////////////////////////////////////////////////////
double Fuzzy_decision(double CV,double UP,double SE) {
	double RAT=0;
	// FILE *fp;

	ambgInit();
	// fp=fopen("result","w");

	// for(CV=0.1;CV <= 1.0;CV += 0.4){
	// 	for(UP=0.1;UP <= 1.0;UP += 0.4){
	// 		for(SE=0;SE <= 1;SE += 0.1){
					RAT=ambgRun(3,CV,UP,SE);
					// fprintf(fp,"%.4f\n",RAT);
			// }
	//	}
	//}
	
	ambgDestroy();
	// fclose(fp);

	return RAT;
}

//convert a std::vector<int> to a single double 
double computeAverage(const std::vector<int>& values) {
    double sum = 0.0;
    for (int value : values) {
        sum += value;
    }
    return sum / values.size();
}

/********************************************************************************/
/********************************************************************************/
////////////////////////   Main processing //////////////////////////////////////
/********************************************************************************/
/********************************************************************************/
int main() {
    std::vector<double> CV_a_wifi;
    std::vector<double> SE_a_wifi;
    std::vector<double> UP_a;
    std::vector<double> fuzzy_re_a_wifi;
    std::vector<double> CV_a_4G;
    std::vector<double> SE_a_4G;
    std::vector<double> fuzzy_re_a_4G;
    /// loop for collect data ////
    int loop_n=0;

    // Get starting timepoint
    auto start = std::chrono::high_resolution_clock::now();
    
    printf("Enter number of Collecting RAT info.: ");
    std::cin>>loop_n;
    for(int count=1;count<=loop_n;count++){
    double CV_wifi = -1.0; // Initialization with default values
    double CV_4G = -1.0;  // Initialization with default values

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////       Collect Data from all RAT         ////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////  Get signal strength from WiFi ///////////////////////////////////////
    printf("<-------------------Collecting info.: [%d] ------------------------> \n", count);
    printf("################## WiFi network information [%d] #################\n", count);
    // Get the RSSI values
    std::vector<int> rssi_values = getRSSIValues();
    if (!rssi_values.empty()) {
        // Print WiFi signal strength for each RSSI value
        printf("WiFi signal strengths:");
        for (int rssi : rssi_values) {
            printf("%d dBm\n", rssi);
            int max = -93;
            CV_wifi = 1-(static_cast<double>(rssi)/max);
            printf("CV_wifi (check) = %f\n",CV_wifi);
        }
        
    } else {
        fprintf(stderr, "No RSSI values found.\n");
        return 1;
    }

    ///////////////////////////////////  Get Data rate from WiFi ///////////////////////////////////////
    std::array<double, 3> GetSpeed_wifi = CheckSpeed();
    double ping_wifi = GetSpeed_wifi[0];
    double downloadRate_wifi = GetSpeed_wifi[1];
    double uploadRate_wifi = GetSpeed_wifi[2];
    double SE_wifi = ((downloadRate_wifi + uploadRate_wifi)/2 )/100;
    if (ping_wifi != -1 && downloadRate_wifi != -1 && uploadRate_wifi != -1) {
        printf("# Ping: %.1f ms\n", ping_wifi);
        printf("# Download Rate: %.2f Mbit/s\n", downloadRate_wifi);
        printf("# Upload Rate: %.2f Mbit/s\n", uploadRate_wifi);
    } else {
        fprintf(stderr, "Error: Unable to parse Speedtest result.\n");
    }

    ///////////////////////////////////  Swiching from WiFi to 4G ///////////////////////////////////////
    printf("Switched to cellular connection.\n");
    // // Disable Wi-Fi
    // system("nmcli c down INALAB_5G");
    system("sudo ifmetric wlan0 1000");

    // Enable Cellular
    system("sudo nmcli c up ppsim");
    // Introduce a 10-second delay before enabling Cellular
    sleep(60);

    /////////////////////////////////////////////////////////////////////////////////////////////////

    printf("################## 4G network information [%d] #################\n", count);

    ////////////////////////////////////  Get signal strength from Sim ///////////////////////////////////////
    const char *portName = "/dev/tty4GPI";
    int fd = openSerialPort(portName); // Open the serial port

    if (fd != -1) { // Check if the serial port was opened successfully
        std::string csqResponse = sendAtCommand(fd); // Send AT command and get response
        // std::cout << "AT command response:" << std::endl;
        // std::cout << csqResponse << std::endl;

        int dbm = calculateDbm(csqResponse); // Calculate dBm from response
        printf("SIM signal strength: %s (%d dBm)\n", connectionStatus(dbm).c_str(), dbm);

        close(fd); // Close the serial port
        int max = -93;
        CV_4G = 1-(static_cast<double>(dbm)/-93);
        printf("CV_4G (check) = %f\n",CV_4G);
    }

    ///////////////////////////////////  Get Data rate from Sim ///////////////////////////////////////

    std::array<double, 3> GetSpeed_4G = CheckSpeed();
    double ping_4G = GetSpeed_4G[0];
    double downloadRate_4G = GetSpeed_4G[1];
    double uploadRate_4G = GetSpeed_4G[2];
    double SE_4G = ((downloadRate_4G + uploadRate_4G)/2 )/100;
    if (ping_4G != -1 && downloadRate_4G != -1 && uploadRate_4G != -1) {
        printf("# Ping: %.1f ms\n", ping_4G);
        printf("# Download Rate: %.1f Mbit/s\n", downloadRate_4G);
        printf("# Upload Rate: %.1f Mbit/s\n", uploadRate_4G);
    } else {
        fprintf(stderr, "Error: Unable to parse Speedtest result.\n");
    }

    // Get ending timepoint
    auto stop = std::chrono::high_resolution_clock::now();
    // Get duration. Substart timepoints to get duration. To cast it to proper unit use duration cast method
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by simulation: "
    << duration.count() << " microseconds" << std::endl;

    ///////////////////////////////////  Swiching from 4G to Wifi ///////////////////////////////////////
    // Disable Cellular
    system("sudo nmcli c down ppsim");

    // Enable Wi-Fi
    // system("nmcli c up INALAB_5G");
    // system("sudo ifconfig wlan0 up");

    system("sudo ifmetric wlan0 600");
    printf("Switched to WiFi connection.\n");


    /////////////////////////////////////////////////  user priority //////////////////////////////////////////
    printf("#### Calculate and preparing CV, SE, UP [%d] ##### \n",count);
    
    printf("CV_wifi = %.6f\n", CV_wifi);

    printf("CV_4G = %.6f\n", CV_4G);

    double UP = random_pri();
    printf("User priority (UP) : %.2f\n", UP);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////       Makng decison part (Fuzzy)         ////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // (wifi)downloadRate_wifi, uploadRate_wifi, rssi --->calibrate --> (0,1)
    //  (4G)  dbm, downloadRate_4G, uploadRate_4G ----> calibrate -->(0,1)
 
    printf("SE_wifi = %.2f <--- DL=%.2f, UL=%.2f   \n", SE_wifi, downloadRate_wifi, uploadRate_wifi);
    printf("SE_4G = %.2f <--- DL=%.2f, UL=%.2f   \n", SE_4G, downloadRate_4G, uploadRate_4G);
    
    



    //Fuzzy_decision(user_pri)
        //Just for testing
    double fuzzy_res_wifi;
    double fuzzy_res_4G;
    fuzzy_res_wifi = Fuzzy_decision(CV_wifi, UP, SE_wifi);
    fuzzy_res_4G = Fuzzy_decision(CV_4G, UP, SE_4G);
    printf("Fuzzy Result (WiFi): %f \n", fuzzy_res_wifi);
    printf("Fuzzy Result (4G): %f \n", fuzzy_res_4G);

if (fuzzy_res_4G>fuzzy_res_wifi)
{
    printf("The decision result is 4G\n");
}
else if (fuzzy_res_wifi > fuzzy_res_4G)
{
    printf("The decsion result is WiFi\n");
}
else{
     printf("Using both of its\n");
}
   
////// Writing values to a text file in C++  /////
    // // Close the file
    // outFile.close();
    CV_a_wifi.push_back(CV_wifi);
    CV_a_4G.push_back(CV_4G);
    SE_a_wifi.push_back(SE_wifi);
    SE_a_4G.push_back(SE_4G);
    UP_a.push_back(UP);
    fuzzy_re_a_wifi.push_back(fuzzy_res_wifi);
    fuzzy_re_a_4G.push_back(fuzzy_res_4G);

    }

   printf("------ Saving wifi data -------\n");
    /// loop for saving parameter values 
// CV=0.1-UP=0.1
// CV=0.1-UP=0.5
// CV=0.1-UP=0.9

// CV=0.5-UP=0.1
// CV=0.5-UP=0.5
// CV=0.5-UP=0.9

// CV=0.9-UP=0.1
// CV=0.9-UP=0.5
// CV=0.9-UP=0.9
    // std::ofstream dataFilewifi_CV_10_UP_10("CV=0.1-UP=0.1.txt");
    // std::ofstream dataFilewifi_CV_10_UP_50("CV=0.1-UP=0.5.txt");
    // std::ofstream dataFilewifi_CV_10_UP_90("CV=0.1-UP=0.9.txt");
    // std::ofstream dataFilewifi_CV_50_UP_10("CV=0.5-UP=0.1.txt");
    // std::ofstream dataFilewifi_CV_50_UP_50("CV=0.5-UP=0.5.txt");
    // std::ofstream dataFilewifi_CV_50_UP_90("CV=0.5-UP=0.9.txt");
    // std::ofstream dataFilewifi_CV_90_UP_10("CV=0.9-UP=0.1.txt");
    // std::ofstream dataFilewifi_CV_90_UP_50("CV=0.9-UP=0.5.txt");
    // std::ofstream dataFilewifi_CV_90_UP_90("CV=0.9-UP=0.9.txt");

    // Save wifi info to text file
    for (int i=1;i<=9;i+=4){
        for (int j=1;j<=9;j+=4){
            // Constructing the filename string using string concatenation
            std::string filename = "WIFI-CV=0." + std::to_string(i) + "-UP=0." + std::to_string(j) + ".txt";
            
            // Creating the ofstream object and opening the file
            std::ofstream dataFilewifi;
            dataFilewifi.open(filename);  // open the file with the constructed name
            
            // Check if the file opened successfully
            if (!dataFilewifi) {
                std::cerr << "Cannot open the output file: " << filename << std::endl;
                return 1;  // Return or handle the error as needed
            }
            
            for (size_t x = 0; x < CV_a_wifi.size(); ++x) { // Use size() method to get the size of the vector
                 // if CV is in the range of i/10-0.05 (ex. 0.05) to i/10 (ex. 0.1) and UP is the range of j/10-0.05 (ex. 0.05) to j/10 (ex. 0.1)
                    float cv_c = i/10.0;
                    float cv_c_5 = cv_c-0.05;
                    float up_c = j/10.0;
                    float up_c_5 = up_c-0.05;
                    printf("[WiFi saved]CV is in the range of %f to %f and UP is the range of %f to %f \n",cv_c_5,cv_c, up_c_5 ,up_c );
                    if (CV_a_wifi[x] <= cv_c && CV_a_wifi[x] >= cv_c_5 && UP_a[x] <= up_c && UP_a[x] >= up_c_5) {

                            dataFilewifi << CV_a_wifi[x] << " " << UP_a[x] << " " << SE_a_wifi[x] << " " << fuzzy_re_a_wifi[x] << "\n";
                    }
            
            }
            // Don't forget to close the file
            dataFilewifi.close();
        }  
    }

    std::ofstream dataFilewifiall("data_wifi.txt");
    // Check if the file stream is open/valid.
    if (!dataFilewifiall) {
        std::cerr << "Cannot open the output file!" << std::endl;
        return 1;
    }
    for (size_t i = 0; i < loop_n; ++i) {
        dataFilewifiall << CV_a_wifi[i] << " " << UP_a[i] << " " << SE_a_wifi[i] << " " << fuzzy_re_a_wifi[i] << "\n";
    }
  
  dataFilewifiall.close();

printf("------ Saving 4G data -------\n");
    // Save 4G info to text file
        for (int i=1;i<=9;i+=4){
        for (int j=1;j<=9;j+=4){
            // Constructing the filename string using string concatenation
            std::string filename = "4G-CV=0." + std::to_string(i) + "-UP=0." + std::to_string(j) + ".txt";
            
            // Creating the ofstream object and opening the file
            std::ofstream dataFile4G;
            dataFile4G.open(filename);  // open the file with the constructed name
            
            // Check if the file opened successfully
            if (!dataFile4G) {
                std::cerr << "Cannot open the output file: " << filename << std::endl;
                return 1;  // Return or handle the error as needed
            }
            
            for (size_t x = 0; x < CV_a_4G.size(); ++x) { // Use size() method to get the size of the vector
                  // if CV is in the range of i/10-0.05 (ex. 0.05) to i/10 (ex. 0.1) and UP is the range of j/10-0.05 (ex. 0.05) to j/10 (ex. 0.1)
                    float cv_c = i/10.0;
                    float cv_c_5 = cv_c-0.05;
                    float up_c = j/10.0;
                    float up_c_5 = up_c-0.05;
                    printf("[4G saved]CV is in the range of %f to %f and UP is the range of %f to %f \n",cv_c_5,cv_c, up_c_5 ,up_c );
                    if (CV_a_4G[x] <= cv_c && CV_a_wifi[x] > up_c_5 && UP_a[x] <= up_c && UP_a[x] > cv_c_5) {
                            dataFile4G << CV_a_4G[x] << " " << UP_a[x] << " " << SE_a_4G[x] << " " << fuzzy_re_a_4G[x] << "\n";
                    }
            
            }
            // Don't forget to close the file
            dataFile4G.close();
        }  
    }

    std::ofstream dataFile4Gall("data_4G.txt");
    // Check if the file stream is open/valid.
    if (!dataFile4Gall) {
        std::cerr << "Cannot open the output file!" << std::endl;
        return 1;
    }
    for (size_t i = 0; i < loop_n; ++i) {
        dataFile4Gall << CV_a_4G[i] << " " << UP_a[i] << " " << SE_a_4G[i] << " " << fuzzy_re_a_4G[i] << "\n";
    }
  
  dataFile4Gall.close();

    printf("------ End program -------\n");


    return 0; // Return 0 to indicate successful program execution
}
