#pragma once

#include <iostream>
#include <set>

#include <parser.hpp>
#include <stubborn_sender.hpp>

class PLSender{
	
public:
	PLSender(Parser::Host Receiver, Parser::Host Self){
		self = Self, receiver = Receiver;
	}

	pp2pSend(std::string msg){
		// append id to the start of the message so that the receiver knows
		// get stubborn links to infinitely send that message
		unsigned long ts = id;
		msg = std::to_string(ts) + "_" + msg;
		StubbornSender::sp2pSend(ts, msg);
		id++;
	}


private:
	Parser::Host receiver;
	Parser::Host self;
	unsigned long id = 0;
	set<unsigned long> acked; // those messages that we have received an ACK for

	pp2pReceive(std::string ackMsg){
		// get the ID of the message
		// get stubborn links to stop sending that message
		std::string ts_str = ackMsg.substr(0, ackMsg.find('_'));
		unsigned long ts = std::stoul(ts_str);
		std::pair<std::set<int>::iterator,bool> ret; // store return value of insert
		ret = ackMsg.insert(ts);
		if(ret -> second)
			StubbornSender::stop(ts);
	}

}
