#pragma once 
 
#include <iostream> 

#include "Helper.hpp"

// Receiver side
#include "FLReceiverReceive.hpp" 


class HandlerReceiver1 {

public:

	// Constructor named initialise, because we wanted to create a global object
	HandlerReceiver1(unsigned long Id, const char *outputPath, const char *ip, unsigned short port) : frr(outputPath, Id, ip, port){
		Helper::printText("Handler Receiver!");
	}

	void stopExchange(){
		(this->frr).stopAll();
	}

private:
	FLReceiverReceive frr;

};
