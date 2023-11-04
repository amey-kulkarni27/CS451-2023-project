#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>

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
		Helper::printText("SEND");
		std::lock_guard<std::mutex> lock(mapLock);
		tsToMsg[ts] = msg;
		for(auto const& [key, val]: tsToMsg){
			Helper::printText("Printing");
			Helper::printText(val);
			Helper::printText(this->tsToMsg[1]);
			(this->fss).fp2pSend(val);
		}
	}

	void sp2pStop(unsigned long ts){
		Helper::printText("STOP");
		std::lock_guard<std::mutex> lock(mapLock);
		tsToMsg.erase(ts);
	}

	void stopAll(){
		keep_sending = false;
		(this->fss).stopAll();
	}

private:
	FLSenderSend fss;
	std::unordered_map<unsigned long, std::string> tsToMsg;
	std::mutex mapLock;
	bool keep_sending = true;

	void flood(){
		std::lock_guard<std::mutex> lock(mapLock);
		for(auto const& [key, val]: tsToMsg){
			Helper::printText("Printing");
			Helper::printText(val);
			Helper::printText(this->tsToMsg[1]);
			(this->fss).fp2pSend(val);
		}
	}

	void continuousSend(){
		
		while(keep_sending){
			flood();
		}
	}
};
