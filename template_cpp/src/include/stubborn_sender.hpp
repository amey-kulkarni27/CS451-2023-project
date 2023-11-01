#pragma once

#include <iostream>
#include <string>
#include <map>
#include <thread>

#include <parser.hpp>
#include <fl_sender.hpp>
#include <pl_sender.hpp>

class StubbornSender{

public:

	StubbornSender(Parser::Host Receiver, Parser::Host Self){
		self = Self, receiver = Receiver;

		FLSender::FLSender(self, receiver);

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

	void sp2pReceive(std::string msg){
		PL_Sender::pp2pReceive(msg);
	}

private:
		std::map<unsigned long, std::string> tsToMsg;
		bool keep_sending = true;

		void continuousSend(){
			while(keep_sending){
				for(const auto& tm: tsToMsg){
					FLSender::fp2pSend(tm.second);
				}
			}
		}
}
