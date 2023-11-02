#pragma once

#include <iostream>
#include <string>
#include <map>
#include <thread>

#include "parser.hpp"
#include "FLSender.hpp"

class StubbornSenderSend{

public:

	StubbornSenderSend(Parser::Host Receiver, Parser::Host Self){
		self = Self, receiver = Receiver;

		// use a separate thread
		std::thread contSending(continuousSend);
		contSending.detach();
	}

	void sp2pSend(unsigned long ts, std::string msg){
		tsToMsg[ts] = msg;
	}

	void sp2pStop(unsigned long ts){
		tsToMsg.erase(ts);
	}

	void stopAll(){
		keep_sending = false;
		FLSender::stopAll();
	}

private:
	Parser::Host self, receiver;
	std::map<unsigned long, std::string> tsToMsg;
	bool keep_sending = true;

	static void continuousSend(){
		while(keep_sending){
			for(const auto& tm: tsToMsg){
				FLSender::fp2pSend(tm.second);
			}
		}
	}
};
