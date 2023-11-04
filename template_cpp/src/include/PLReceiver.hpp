#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>

#include "parser.hpp"
#include "FLReceiverSend.hpp"
#include "Helper.hpp"

class PLReceiver{
	
public:
	PLReceiver(const char *oPath) : frs(), outPath(oPath){
	}

	int getSocket(){
		return (this->frs).getSocket();
	}

	void pp2pReceive(std::string msg, sockaddr_in clientAddress){
		// get timestamp
		// send corresponding ACK
		// if this has not yet been delivered, deliver it
		size_t firstUnderscore = msg.find('_');
		std::string idStr = msg.substr(0, firstUnderscore);
		size_t secondUnderscore = msg.find('_', firstUnderscore + 1);
		std::string tsStr = msg.substr(firstUnderscore + 1, secondUnderscore - firstUnderscore - 1);
		std::string msgWithoutId = msg.substr(secondUnderscore + 1); // there will always be something to the right of the second underscore
		pp2pSend(tsStr, clientAddress);
    unsigned long id = std::stoul(idStr);
    unsigned long ts = std::stoul(tsStr);
    if(delivered.find(id) == delivered.end() || delivered[id].find(ts) == delivered[id].end()){
			delivered[id].insert(ts);
			deliver(msgWithoutId, id);
			// deliver the message
		}
	}

	void stopAll(){
		callFlush();
	}


private:
	FLReceiverSend frs;
	const char *outPath;
	std::unordered_map<unsigned long, std::unordered_set<unsigned long>> delivered;
	Parser::Host self;
	std::queue<std::pair<unsigned long, std::string> > logs;
	unsigned long thresh = 1000;

	void pp2pSend(std::string ts_str, sockaddr_in clientAddress){
		// send an ack, ie, just a timestamp
		(this->frs).fp2pSend(ts_str, clientAddress);
	}

	void deliver(std::string msg, unsigned long id){
		// 1) Spawn a thread to deal with it (optimisation)
		// 2) Read the "_" separated messages (into strings) and log each of these messages into a queue

		// RECEIVER CODE GOES HERE
		size_t curpos = 0;
		size_t found = msg.find('_');
		while(found != std::string::npos){
			std::string underlying_msg = msg.substr(curpos, found - curpos);
			logs.push(make_pair(id, underlying_msg));
			curpos = found + 1;
			found = msg.find('_', curpos);
		}
		if(logs.size() >= thresh)
			callFlush();

		// Perfect Links
		// receive packet, resend an ACK

		// if packet received first time, go through the contents and log them, update the set of messages received, resend ACK
		// if packet already received, discard and resend ACK
	}

	void callFlush(){
		Helper::flush(logs, outPath, true);
	}

};
