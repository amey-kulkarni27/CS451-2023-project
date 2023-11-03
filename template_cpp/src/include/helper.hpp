#include <iostream>
#include <sstream>
#include <string>
#include <queue>


class Helper {

public:

  static void flush(std::queue<std::string> &logs, const char *o_path){
    // Make Separate for receiver
    std::string filePath = outPath + "/" + std::to_string(id) + ".txt";
    // Check if the file exists
    struct stat buffer;
    if (stat(filePath.c_str(), &buffer) == 0) {
        // File exists, open it in append mode
        std::ofstream outputFile(filePath, std::ios::app);

        if (!outputFile.is_open()) {
            std::cerr << "Error opening the file for appending: " << filePath << std::endl;
            return;
        }

        // Append each underlying_msg from the logs to the file
        while (!logs.empty()) {
            const std::string& underlying_msg = logs.front();
            outputFile << underlying_msg << std::endl; // Append the underlying_msg to the file
            logs.pop(); // Remove the processed underlying_msg from the queue
        }
    }
    else {
        // File doesn't exist, create a new file
        std::ofstream outputFile(filePath);

        if (!outputFile.is_open()) {
            std::cerr << "Error creating the file: " << filePath << std::endl;
            return;
        }

        // Write each underlying_msg from the queue to the file
    while (!logs.empty()) {
            const std::string& underlying_msg = logs.front();
            outputFile << underlying_msg << std::endl; // Write the underlying_msg to the file
            logs.pop(); // Remove the processed underlying_msg from the queue
        }
    }

    std::cout << "Contents have been written to the file: " << filePath << std::endl;
  }

};
