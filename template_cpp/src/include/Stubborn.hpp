#pragma once

#include <iostream>
#include <string>
#include <map>
#include <thread>

#include "parser.hpp"
#include "FLSenderSend.hpp"

class Stubborn{

public:

	Stubborn(Parser::Host Receiver){
		receiver = Receiver;

		this->fss = FLSenderSend::FLSenderSend(receiver);

		// use a separate thread
		std::thread contSending(continuousSend);
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
	FLSenderSend::FLSenderSend fss;
	Parser::Host receiver;
	std::map<unsigned long, std::string> tsToMsg;
	bool keep_sending = true;

	static void continuousSend(){
		while(keep_sending){
			for(const auto& tm: tsToMsg){
				(this->fss).fp2pSend(tm.second);
			}
		}
	}
};
