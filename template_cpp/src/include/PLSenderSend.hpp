#pragma once

#include <iostream>
#include <string>
#include <set>

#include <parser.hpp>
#include <Stubborn.hpp>

class PLSenderSend{
	
public:
	Parser::Host receiver;
	Stubborn s;

	PLSenderSend(Parser::Host Receiver) : receiver(Receiver), s(Receiver){
	}

	int getSocket(){
		return (this->s).getSocket();
	}

	void pp2pSend(std::string msg){
		// append id to the start of the message so that the receiver knows
		// get stubborn links to infinitely send that message
		unsigned long ts = id;
		msg = std::to_string(ts) + "_" + msg;
		(this->s).sp2pSend(ts, msg);
		id++;
	}

	void stopAll(){
		(this->s).stopAll();
	}


private:
	unsigned long id = 0;

};
