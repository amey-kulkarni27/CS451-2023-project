#pragma once 
 
#include <iostream> 
#include <sstream>
#include <string>
#include <set>

#include <parser.hpp> 

// Sender side
#include "PLSenderSend.hpp" 
#include "Stubborn.hpp"
#include "FLSenderReceive.hpp" 

// Receiver side
#include "FLReceiverReceive.hpp" 


class HandlerT1 {

public:

	void flush(){
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


	// Constructor named initialise, because we wanted to create a global object
	void initialise(unsigned long Id, std::vector<Parser::Host> hosts, const char *outputPath, const char *configPath){
		outPath = std::string(outputPath);
		createHostMap(hosts);
		id = Id; 
		if(readParams(configPath, num_messages, target) == false) 
			std::cerr<<"Failed to read parameters from the config file "<<std::endl; 
	 
		if(id == target) 
			receiver = true; 	

		// Initialise Perfect Links
		if(receiver){
			this -> frr = FLReceiverReceive::FLReceiverReceive(outPath);
		}
		else{
			this -> pss = PLSenderSend::PLSenderSend();
			this -> fsr = FLSenderReceive::FLSenderReceive((this->pss).s, (this->pss).getSocket());
		}
	}


	void deliverMessage(std::string msg){
		// 1) If one is received from PerfectLinks, spawn a thread to deal with it (optimisation)
		// 2) Read the "_" separated messages (into strings) and log each of these messages into a queue

		// RECEIVER CODE GOES HERE
    size_t curpos = 0;
    size_t found = msg.find('_');
		while(found != std::string::npos){
			std::string underlying_msg = msg.substr(curpos, found - curpos);
			logs.push_back(underlying_msg);
			curpos = found + 1;
			found = msg.find('_', curpos);
		}
		if(logs.size() >= thresh)
			flush(logs);

		// Perfect Links
		// receive packet, resend an ACK

		// if packet received first time, go through the contents and log them, update the set of messages received, resend ACK
		// if packet already received, discard and resend ACK
	}


	void startExchange(){
		else
			sendMessage();
	}


	void stopExchange(){
		// stop perfect links
		if(receiver)
			f;
		else
			PLSender::stop();
	}

private:
	PLSenderSend::PLSenderSend pss;
	FLSenderReceive::FLSenderReceive fsr;

	map<unsigned long, Parser::Host> hostMap;
	unsigned long id;
	unsigned long num_messages;
	unsigned long target;
	bool receiver = false;
	int send_id = 1;
	queue<std::string> logs;
	int thresh = 1000;
	std::string outPath;

	void createHostMap(std::vector<Parser::Host> hosts){
		for (auto &host : hosts)
			hostMap[host.id] = host;
	}

	bool readParams (const char *configPath, unsigned long &num_messages, unsigned long &target){
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

	std::string createMsgAppendToLogs(unsigned long st, unsigned long en){
		std::string payload = "";
		while(st < en){
			std::string msg = std::to_string(st);
			logs.push_back(msg);
			payload += msg + "_";
			st++;
		}
		return payload;
	}
	void sendMessage(){
		// 1) Create packets containing 8 messages
		// 2) Log them and send them through the perfect links abstraction
		unsigned long i = 0;
		while(i < num_messages){
			unsigned long end = std::min(i + 8, num_messages);
			std::string msgToSend = createMsgAppendToLogs(i, end);
			PLSender::pp2pSend(msgToSend);
			// do this using a separate thread
			flush(logs);
			i = end;
		}

		// Perfect Links
		// A possible optimisation is:
		// A producer creating the packets
		// A consumer reading the packets and sending them to the Perfect Links

		// send the first packet till you receive an ACK

		// create a packet of 8 messages
		// log it
		// wait for an ACK
	}


};
