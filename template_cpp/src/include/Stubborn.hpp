#pragma once

#include <iostream>
#include <string>
#include <map>
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
	std::map<unsigned long, std::string> tsToMsg;
	std::mutex mapLock;
	bool keep_sending = true;

	void flood(){
		Helper::printText("FLOOD");
		std::lock_guard<std::mutex> lock(mapLock);
		for(const std::pair<unsigned long, std::string>& tm: tsToMsg){
			Helper::printText("Printing");
			std::cout<<tm.first<<std::endl;
			Helper::printText(tm.second);
			Helper::printText(this->tsToMsg[1]);
			(this->fss).fp2pSend(tm.second);
		}
	}

	void continuousSend(){
		while(keep_sending){
			std::cout<<tsToMsg.size()<<std::endl;
			flood();
		}
	}
};
