#pragma once 
 
#include <iostream> 
#include <string>
#include <map>
#include <queue>
#include <memory>

#include "parser.hpp" 
#include "Helper.hpp"

// Sender side
#include "PLSenderSend.hpp" 
#include "FLSenderReceive.hpp" 


class HandlerSender1 {

public:
	PLSenderSend pss;

	// Constructor named initialise, because we wanted to create a global object
	HandlerSender1(unsigned long id, const char *outputPath, unsigned long num_messages_, unsigned long target_, const char *ip, unsigned short port, const char *ip_self, unsigned short port_self) : pss(id, ip, port){
		outPath = outputPath;
		num_messages = num_messages_;
		target = target_;
	 
		initReceiver(ip_self, port_self);
		// std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	void initReceiver(const char *ip_self, unsigned short port_self){
		std::unique_ptr<FLSenderReceive> fsr = std::make_unique<FLSenderReceive>(&((this->pss).s), (this->pss).getSocket(), ip_self, port_self);
		// Transfers ownership
		this -> fsrptr = std::move(fsr);
	}



	void startExchange(){
		sendMessage();
	}


	void stopExchange(){
		// stop perfect links
		Helper::flush(logs, outPath, false);
		(this->pss).stopAll();
		(this->fsrptr) -> stopAll();

	}

private:
	std::unique_ptr<FLSenderReceive> fsrptr;
	unsigned long num_messages;
	unsigned long target;
	bool receiver = false;
	std::queue<std::pair<unsigned long, std::string> > logs;
	unsigned thresh = 500;
	const char *outPath;

	std::string createMsgAppendToLogs(unsigned long st, unsigned long en){
		std::string payload = "";
		while(st < en){
			std::string msg = std::to_string(st);
			logs.push(make_pair(1, msg));
			payload += msg + "_";
			st++;
		}
		return payload;
	}

	void sendMessage(){
		// 1) Create packets containing 8 messages
		// 2) Log them and send them through the perfect links abstraction
		unsigned long i = 1;
		while(i <= num_messages){
			unsigned long end = std::min(i + 8, num_messages + 1);
			std::string msgToSend = createMsgAppendToLogs(i, end);
			(this->pss).pp2pSend(msgToSend);
			// do this using a separate thread
			Helper::flush(logs, outPath, false);
			i = end;
		}
		
		// std::this_thread::sleep_for(std::chrono::seconds(1));

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
