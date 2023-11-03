#pragma once 
 
#include <iostream> 
#include <string>
#include <map>
#include <queue>

#include "parser.hpp" 
#include "Helper.hpp"

// Sender side
#include "PLSenderSend.hpp" 
#include "Stubborn.hpp"
#include "FLSenderReceive.hpp" 


class HandlerSender1 {

public:

	// Constructor named initialise, because we wanted to create a global object
	HandlerSender1(unsigned long Id, std::vector<Parser::Host> hosts, const char *outputPath, int target_, int num_messages_){
		outPath = outputPath;
		createHostMap(hosts);
		id = Id; 
	 
		if(id == target) 
			receiver = true; 	

		// Initialise Perfect Links
		if(receiver){
			this -> frr = FLReceiverReceive(outPath, id);
		}
		else{
			this -> pss = PLSenderSend(hostMap[target]);
			this -> fsr = FLSenderReceive((this->pss).s, (this->pss).getSocket());
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
			logs.push(underlying_msg);
			curpos = found + 1;
			found = msg.find('_', curpos);
		}
		if(logs.size() >= thresh)
			Helper::flush(logs, outPath, id);

		// Perfect Links
		// receive packet, resend an ACK

		// if packet received first time, go through the contents and log them, update the set of messages received, resend ACK
		// if packet already received, discard and resend ACK
	}


	void startExchange(){
		if(!receiver)
			sendMessage();
	}


	void stopExchange(){
		// stop perfect links
		if(receiver)
			(this->frr).stopAll();
		else{
			(this->pss).stopAll();
			(this->fsr).stopAll();
		}

	}

private:
	FLReceiverReceive frr;
	PLSenderSend pss;
	FLSenderReceive fsr;

	std::map<unsigned long, Parser::Host> hostMap;
	unsigned long id;
	unsigned long num_messages;
	unsigned long target;
	bool receiver = false;
	int send_id = 1;
	std::queue<std::string> logs;
	unsigned thresh = 5;
	const char *outPath;

	void createHostMap(std::vector<Parser::Host> hosts){
		for (auto &host : hosts)
			hostMap[host.id] = host;
	}

	std::string createMsgAppendToLogs(unsigned long st, unsigned long en){
		std::string payload = "";
		while(st < en){
			std::string msg = std::to_string(st);
			logs.push(msg);
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
			(this->pss).pp2pSend(msgToSend);
			// do this using a separate thread
			Helper::flush(logs, outPath, id);
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
