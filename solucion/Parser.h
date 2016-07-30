#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "Expression.h"
#include "Function.h"
#include "Environment.h"
#include "Mutex.h"

class Parser {
	private:
		std::map<std::string, LispFunctionFactory*> function_factory;
	
		int get_expressions_end(const std::string& line, int start);
		int get_element_end(const std::string& line, int start_index);
	
	public:
		Parser();
		bool valid_line(const std::string& line);
		std::string get_function_name(const std::string& line);
		bool has_function(std::string name);
		LispFunction* get_function(const std::string& function_name);
		std::vector<Argument*> get_arguments(const std::string& line);
		~Parser();
};

class ExpressionThreadParser {
	private:
		Parser parser;
	public:
		int operator()(const std::string& line, 
			std::vector<ExpressionThread*>* threads, ContextProtected& context);
		~ExpressionThreadParser();
};

class ExpressionParser {
	private:
		Parser parser;
	public:
		Expression* operator()(const std::string& line);
		~ExpressionParser();
};

#endif // PARSER_H
