#pragma once

#include <iostream>

#include <parser.hpp>
// #include <stubborn_links.hpp>

class PLSender{
	
public:
	PLSender(Parser::Host Receiver, Parser::Host Self){
		self = Self, receiver = Receiver;
	}

	pp2pSend(std::string msg){
		// append id to the start of the message so that the receiver knows
		// get stubborn links to infinitely send that message
	}


private:
	Parser::Host receiver;
	Parser::Host self;
	unsigned long id = 0;

	pp2pReceive(std::string ackMsg){
		// get the ID of the message
		// get stubborn links to stop sending that message
	}

}
