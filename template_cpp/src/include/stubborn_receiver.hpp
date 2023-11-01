#pragma once

#include <iostream>
#include <string>

#include <parser.hpp>
#include <fl_receiver.hpp>
#include <pl_receiver.hpp>

class StubbornReceiver(){

public:

	StubbornReceiver(Parser::Host Self){
		self = Self;
	}

	void sp2pReceive(std::string msg, sockaddr_in clientAddress){
		PLReceiver::pp2pReceive(msg);
	}

	void sp2pSend(std::string msg, sockaddr_in clientAddress){
		FLReceiver::fp2pSend(msg, clientAddress);
	}

	void stopAll(){
		FLReceiver::stopALL();
	}

private:
	Parser::Host self;

}
