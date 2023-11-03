#pragma once 
 
#include <iostream> 
#include <string>
#include <map>
#include <queue>

#include "parser.hpp" 
#include "Helper.hpp"

// Sender side
#include "PLSenderSend.hpp" 
#include "FLSenderReceive.hpp" 


class HandlerSender1 {

public:

	// Constructor named initialise, because we wanted to create a global object
	HandlerSender1(unsigned long Id, const char *outputPath, unsigned long num_messages_, unsigned long target_, const char *ip, unsigned short port) : pss(ip, port){
		outPath = outputPath;
		id = Id; 
		num_messages = num_messages_;
		target = target_;
	 
		initReceiver();
	}


	void startExchange(){
		sendMessage();
	}


	void stopExchange(){
		// stop perfect links
		(this->pss).stopAll();
		(this->fsrptr) -> stopAll();

	}

private:
	PLSenderSend pss;
	FLSenderReceive *fsrptr;

	unsigned long id;
	unsigned long num_messages;
	unsigned long target;
	bool receiver = false;
	std::queue<std::string> logs;
	unsigned thresh = 5;
	const char *outPath;

	void initReceiver(){
		FLSenderReceive fsr((this->pss).s, (this->pss).getSocket());
		this -> fsrptr = &fsr;
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
