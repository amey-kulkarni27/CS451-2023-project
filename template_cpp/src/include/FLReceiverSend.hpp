#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#include <parser.hpp>


class FLReceiverSend{

public:
	FLReceiverSend(int sock_) : sock(sock_){
	}

	void fp2pSend(std::string msg, sockaddr_in clientAddress){
		// Send the ACK to the client
		if(sendto(sock, msg.c_str(), msg.length(), 0, reinterpret_cast<struct sockaddr*>(&clientAddress), sizeof(clientAddress)) == -1)
			perror("Error while sending the ACK.\n");
	}


private:
	int sock;

};
