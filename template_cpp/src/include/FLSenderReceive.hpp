#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <cassert>

#include "parser.hpp"
#include "PLSenderReceive.hpp"
#include "Stubborn.hpp"


class FLSenderReceive{

public:
	FLSenderReceive(Stubborn s, int sock_) : sock(sock_), psr(s){

		// activate listening
		std::thread receiverThread(&FLSenderReceive::fp2pReceive, this);
		receiverThread.detach();
		// main function continues working as before

	}

	int getSocket(){
		return sock;
	}

	void stopAll(){
		listen = false;
	}

private:
	int sock;
	PLSenderReceive psr;
	bool listen = true;

	void fp2pReceive(){
		char buffer[1024];
		while(listen){
			ssize_t readLen = recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL);
			if(readLen == -1){
				perror("Could not read the contents of the datagram(ACK) sent by the receiver.\n");
				return;
			}
			assert(readLen < 1024);
			buffer[readLen] = '\0';

			std::string recvMsg(buffer);
			(this->psr).pp2pReceive(recvMsg);
		}
		if (close(sock) == 0) {
        std::cout << "Socket closed successfully." << std::endl;
    }
	 	else {
        std::cerr << "Failed to close the socket." << std::endl;
    }
	}

};
