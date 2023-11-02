#pragma once

#include <iostream>
#include <string>
#include <set>

#include <parser.hpp>
#include <StubbornSenderSend.hpp>

// called by FLSenderReceive Exclusively
class PLSenderReceive{
	
public:
	PLSenderReceive(){
	}

	void pp2pReceive(std::string ackMsg){
		// get the ID of the message
		// get stubborn links to stop sending that message
		unsigned long ts = std::stoul(ackMsg); // ackMsg is just a single number, as a string
		std::pair<std::set<int>::iterator,bool> ret; // store return value of insert
		ret = ackMsg.insert(ts);
		if(ret -> second)
			StubbornSenderSend::sp2pStop(ts);
	}


private:
	set<unsigned long> acked; // those messages that we have received an ACK for

};
