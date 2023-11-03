#pragma once

#include <iostream>
#include <string>
#include <map>
#include <thread>

#include "parser.hpp"
#include "FLSenderSend.hpp"

class Stubborn{

public:

	// Using a member initialiser list to 
	Stubborn(Parser::Host Receiver) : receiver(Receiver), fss(Receiver){

		// use a separate thread
		std::thread contSending(&Stubborn::continuousSend, this);
		contSending.detach();
	}

	// Creating a copy constructor
	Stubborn(const Stubborn &x) : receiver(x.receiver), fss(x.fss){
	}

	int getSocket(){
		return (this->fss).getSocket();
	}

	void sp2pSend(unsigned long ts, std::string msg){
		tsToMsg[ts] = msg;
	}

	void sp2pStop(unsigned long ts){
		tsToMsg.erase(ts);
	}

	void stopAll(){
		keep_sending = false;
		(this->fss).stopAll();
	}

private:
	Parser::Host receiver;
	FLSenderSend fss;
	std::map<unsigned long, std::string> tsToMsg;
	bool keep_sending = true;

	void continuousSend(){
		while(keep_sending){
			for(const auto& tm: tsToMsg){
				(this->fss).fp2pSend(tm.second);
			}
		}
	}
};
