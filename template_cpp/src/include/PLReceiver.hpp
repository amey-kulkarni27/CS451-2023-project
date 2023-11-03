#pragma once

#include <iostream>
#include <string>
#include <queue>

#include <parser.hpp>
#include <FLReceiverSend.hpp>

class PLReceive{
	
public:
	PLReceive(int sock, const char *oPath) : frs(sock), outPath(oPath){
	}

	void pp2pReceive(std::string msg, sockaddr_in clientAddress){
		// get timestamp
		// send corresponding ACK
		// if this has not yet been delivered, deliver it
		size_t firstUnderscore = msg.find('_');
		std::string tsStr = msg.substr(0, firstUnderscore);
		std::string msgWithoutId = msg.substr(firstUnderscore + 1); // there will always be something to the right of the first underscore
		pp2pSend(tsStr, clientAddress);
    unsigned long ts = std::stoul(tsStr);
    std::pair<std::set<unsigned long>::iterator,bool> ret; // store return value of insert
    ret = delivered.insert(ts);
    if(ret.second)
			deliver(msgWithoutId);
			// deliver the message
	}

	void stopAll(){
		callFlush();
	}


private:
	FLReceiverSend frs;
	const char *outPath;
	std::set<unsigned long> delivered; // lighter to store numbers in the set rather than strings
	Parser::Host self;
	std::queue<std::string> logs;
	int thresh = 5;

	void pp2pSend(std::string ts_str, sockaddr_in clientAddress){
		// send an ack, ie, just a timestamp
		(this->frs).fp2pSend(ts_str, clientAddress);
	}

	void deliver(std::string msg){
		// 1) Spawn a thread to deal with it (optimisation)
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
			callFlush(logs);

		// Perfect Links
		// receive packet, resend an ACK

		// if packet received first time, go through the contents and log them, update the set of messages received, resend ACK
		// if packet already received, discard and resend ACK
	}

	void callFlush(){
		Helper::flush(logs, outPath);
	}

};
