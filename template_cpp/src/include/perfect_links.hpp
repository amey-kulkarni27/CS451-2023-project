#pragma once

#include <iostream>

#include <parser.hpp>
// #include <stubborn_links.hpp>

class PerfectLinks{
	
public:


	PerfectLinks(int id, std::map<unsigned long, Parser::Host> idToDetail){
		id = id;
		idToDetail = idToDetail;
	}

	void writeLogs(std::string outputPath){
		// Write logs to the output path
	}

	


private:

	int id;
	std::map<unsigned long, Parser::Host> idToDetail;
	std::set<int> delivered;
	std::queue<int> logs;

}
