#pragma once

#include <iostream>
#include <string>
#include <set>

#include <parser.hpp>
#include <StubbornSenderSend.hpp>

class PLSenderSend{
	
public:
	PLSenderSend(Parser::Host Receiver, Parser::Host Self){
		self = Self, receiver = Receiver;

		StubbornSenderSend::StubbornSenderSend(self, receiver);
	}

	void pp2pSend(std::string msg){
		// append id to the start of the message so that the receiver knows
		// get stubborn links to infinitely send that message
		unsigned long ts = id;
		msg = std::to_string(ts) + "_" + msg;
		StubbornSenderSend::sp2pSend(ts, msg);
		id++;
	}

	void stopAll(){
		StubbornSenderSend::stopAll();
	}


private:
	Parser::Host receiver;
	Parser::Host self;
	unsigned long id = 0;

};
