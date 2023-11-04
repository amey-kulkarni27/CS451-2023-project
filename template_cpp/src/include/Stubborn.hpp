#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <cassert>
#include <thread>
#include <chrono>
#include <mutex>

#include "parser.hpp"
#include "Helper.hpp"

#include "FLSenderSend.hpp"

class Stubborn{

public:
	std::unordered_map<unsigned long, std::string> tsToMsg;

	// Using a member initialiser list to 
	Stubborn(const char *ip, unsigned short port) :fss(ip, port){
		Helper::printText("STUBBORN");
		prt = port;

		// use a separate thread
		std::thread contSending(&Stubborn::continuousSend, this);
		contSending.detach();
	}

	int getSocket(){
		return (this->fss).getSocket();
	}

	void sp2pSend(unsigned long ts, std::string msg){
		Helper::printText("SEND");
		const std::lock_guard<std::mutex> lock(mapLock);
		std::cout<<&tsToMsg<<std::endl;
		tsToMsg[ts] = msg;
		std::cout<<"MAIN: "<<tsToMsg.size()<<std::endl;
	}

	void sp2pStop(unsigned long ts){
		Helper::printText("STOP");
		const std::lock_guard<std::mutex> lock(mapLock);
		tsToMsg.erase(ts);
	}

	void stopAll(){
		Helper::printText("STOP");
		keep_sending = false;
		(this->fss).stopAll();
	}

private:
	FLSenderSend fss;
	std::mutex mapLock;
	bool keep_sending = true;
	unsigned short prt = 0;

	int flood(){
		const std::lock_guard<std::mutex> lock(mapLock);
		if(tsToMsg.size() == 0)
			return 0;
		// std::cout<<"THREAD: "<<tsToMsg.size()<<std::endl;
		for(auto const& [key, val]: tsToMsg){
			// Helper::printText("Above");
			// std::cout<<key<<" "<<val<<std::endl;
			// Helper::printText("Below");
			(this->fss).fp2pSend(val);
		}
		return 1;
	}

	void continuousSend(){
		
		std::cout<<"THREAD: "<<tsToMsg.size()<<std::endl;
		std::cout<<prt<<std::endl;
		std::cout<<&tsToMsg<<std::endl;
		std::cout<<tsToMsg.size()<<std::endl;
		while(keep_sending){
			if(flood() == 0)
				std::this_thread::sleep_for(std::chrono::nanoseconds(10));
		}
		Helper::printText("Cont Send Thread terminates");
	}
};
