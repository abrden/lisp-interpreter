#include <iostream>
#include <cstdlib>

#include "LispInterpreter.h"

#define EXIT_FAILURE_1 1
#define EXIT_FAILURE_2 2

int main(int argc, const char* argv[]) {
	if (argc != 1) {
		std::cout << "ERROR: argumentos\n";
		return EXIT_FAILURE_1;
	}
	
	LispInterpreter interpreter;
	
	if (interpreter.execute()) {
		return EXIT_FAILURE_2;
	}
	
	return EXIT_SUCCESS;
}
