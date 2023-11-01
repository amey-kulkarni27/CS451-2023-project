#pragma once

#include <iostream>
#include <string>

#include <parser.hpp>
#include <stubborn_receiver.hpp>
#include <handler_t1.hpp>

class PLReceiver{
	
public:
	PLReceiver(Parser::Host Self){
		self = Self;
	}

	void pp2pReceive(std::string msg, sockaddr_in clientAddress){
		// get timestamp
		// send corresponding ACK
		// if this has not yet been delivered, deliver it
		std::string ts_str = msg.substr(0, msg.find('_'));
		pp2pSend(ts_str, clientAddress);
    unsigned long ts = std::stoul(ts_str);
    std::pair<std::set<int>::iterator,bool> ret; // store return value of insert
    ret = delivered.insert(ts);
    if(ret -> second)
	}

	void stopALL(){
		StubbornReceive::stopAll();
	}


private:
	std::set<unsigned long> delivered; // lighter to store numbers in the set rather than strings
	Parser::Host self;

	void pp2pSend(std::string ts_str, sockaddr_in clientAddress){
		// send an ack, ie, just a timestamp
		StubbornReceiver::sp2pSend(ts, clientAddress);
	}

}
