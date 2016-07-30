#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>

#include "Parser.h"
#include "Mutex.h"
#include "Expression.h"
#include "LispInterpreter.h"

#define SYNC 1
#define INVALID_INPUT_ERROR 2
#define EMPTY_INPUT 3

int LispInterpreter::execute() {
	ExpressionThreadParser parser;
	int result, i;
	std::string line;
	
	i = 0;
	while (std::getline(std::cin, line)) {
		result = parser(line, &(this->threads), this->context);
		
		if (result == SYNC) {
			this->join_threads();
			i = 0;
			continue;
		
		} else if (result == EMPTY_INPUT) {
			continue;
		
		} else if (result == INVALID_INPUT_ERROR) {
			std::cout << "ERROR: " + line + "\n";
			return INVALID_INPUT_ERROR;
		}
		
		this->threads[i]->start();
		++i;
	}
	
	return EXIT_SUCCESS;
}

void LispInterpreter::join_threads() {
	for (unsigned int i = 0; i < this->threads.size(); ++i)
		this->threads[i]->join();
	this->delete_threads();
	this->threads = std::vector<ExpressionThread*>();
}

void LispInterpreter::delete_threads() {
	for (unsigned int i = 0; i < this->threads.size(); ++i)
		delete this->threads[i];
}

LispInterpreter::~LispInterpreter() {
	this->join_threads();
}
