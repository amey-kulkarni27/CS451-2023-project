#pragma once

#include <iostream>

#include <parser.hpp>
// #include <stubborn_links.hpp>

class PLReceiver{
	
public:
	PLReceiver(Parser::Host Self){
		self = Self, receiver = Receiver;
	}

	pp2pReceive(std::string msg){
	}


private:
	std::set<unsigned long> delivered;
	Parser::Host self;

	pp2pSend(unsigned long id){
		// send an ack, ie, just an ID
	}

}
