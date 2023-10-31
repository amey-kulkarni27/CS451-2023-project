#pragma once 
 
#include <iostream> 
#include <sstream>
#include <string>
#include <set>

#include <parser.hpp> 
// #include <perfect_links.hpp> 


class Handler_T1 {

public:

	// Constructor named initialise, because we wanted to create a global object
	void initialise(unsigned long Id, std::vector<Parser::Host> hosts, const char *outputPath, const char *configPath){
		id = Id; 
		if(readParams(configPath, num_messages, target) == false) 
			std::cerr<<"Failed to read parameters from the config file "<<std::endl; 
	 
		if(id == target) 
			receiver = true; 	
	}

	void startExchange(){
		if(receiver){
			// 1) Always listen for an incoming message. If one is received from PerfectLinks, spawn a thread to deal with it
			// 2) Read the "_" separated messages (into strings) and log each of these messages into a queue

			// Perfect Links
			// receive packet, resend an ACK

			// if packet received first time, go through the contents and log them, update the set of messages received, resend ACK
			// if packet already received, discard and resend ACK
		}
		else{
			// 1) Create packets containing 8 messages
			// 2) Log them and send them through the perfect links abstraction

			// Perfect Links
			// A possible optimisation is:
			// A producer creating the packets
			// A consumer reading the packets and sending them to the Perfect Links

			// send the first packet till you receive an ACK

			// create a packet of 8 messages
			// log it
			// wait for an ACK

		}
	}

private:
	unsigned long id;
	int num_messages;
	unsigned long target;
	bool receiver = false;
	int send_id = 1;

	bool readParams (const char *configPath, int &num_messages, unsigned long &target){
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


};
