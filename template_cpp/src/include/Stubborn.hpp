#pragma once

#include <iostream>
#include <string>
#include <map>
#include <thread>

#include "parser.hpp"
#include "Helper.hpp"

#include "FLSenderSend.hpp"

class Stubborn{

public:

	// Using a member initialiser list to 
	Stubborn(const char *ip, unsigned short port) :fss(ip, port){

		// use a separate thread
		std::thread contSending(&Stubborn::continuousSend, this);
		contSending.detach();
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
	FLSenderSend fss;
	std::map<unsigned long, std::string> tsToMsg;
	bool keep_sending = true;

	void continuousSend(){
		while(keep_sending){
			Helper::printText("Printing");
			for(const auto& tm: tsToMsg){
				(this->fss).fp2pSend(tm.second);
			}
		}
	}
};
