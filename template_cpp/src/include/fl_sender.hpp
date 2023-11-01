#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <cassert>

#include <parser.hpp>
#include <stubborn_sender.hpp>


class FLSender{

public:
	FLSender(Parser::Host Receiver, Parser::Host Self){
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if(sock == -1){
        perror("Failed to create socket");
    }

		serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(Receiver.portReadable());
    serverAddress.sin_addr.s_addr = inet_addr(Receiver.ipReadable().c_str());

		// activate listening
		std::thread receiverThread(&FLSender::fp2pReceive, this);
		receiverThread.detach();
		// main function continues working as before
	}

	void fp2pSend(std::string msg){
		if(sendto(sock, msg.c_str(), msg.length(), 0, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1)
			perror("Error while sending the message.\n");
	}

	void stopAll(){
		listen = false;
		if (close(sock) == 0) {
        std::cout << "Socket closed successfully." << std::endl;
    }
	 	else {
        std::cerr << "Failed to close the socket." << std::endl;
    }
	}

private:
	bool listen = true;
	int sock;
	sockaddr_in clientAddress, serverAddress;

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
			StubbornSender::sp2pReceive(recvMsg);
		}
	}

};
