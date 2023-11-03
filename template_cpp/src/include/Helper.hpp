#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <map>
#include <cassert>
#include <sys/stat.h>
#include <cstdio>

#include "parser.hpp"


class Helper {

public:

	static void printThreadId(){
		std::cout<<std::this_thread::get_id()<<std::endl;
	}

	static void printText(std::string s){
		std::cout<<s<<std::endl;
	}

	static void removeFile(const char *outPath){
		if (std::remove(outPath) != 0) {
				// If the file could not be removed, an error occurred
				std::cerr << "Error deleting the file" << std::endl;
		} else {
				// File was successfully removed
				std::cout << "File deleted successfully" << std::endl;
		}
	}

	static Parser::Host getReceiverInfo(std::vector<Parser::Host> hosts, unsigned long target){
		Parser::Host dummy;
		for (auto &host : hosts)
		if(host.id == target)
			return host;
		assert(false);
		return dummy;
	}

 	static bool readParams (const char *configPath, unsigned long &num_messages, unsigned long &target){
		std::ifstream configFile(configPath);

		if(configFile.is_open()){
			std::string firstLine;
			if(getline(configFile, firstLine)){
				std::istringstream iss(firstLine);
				if(iss>>num_messages>>target){
					return true;
				}
				else
					std::cerr<<"Failed to read in the two integers "<<std::endl;
			}
			else
				std::cerr<<"Failed to read the first line "<<std::endl;

		}
		else
			std::cerr<<"Failed to open config file "<<std::endl;

		return false;
	}

  static void flush(std::queue<std::string> &logs, const char *o_path, unsigned long id){

		// Convert const char * to string
		std::string outPath(o_path);

    // Make Separate for receiver
    std::string filePath = outPath;
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
