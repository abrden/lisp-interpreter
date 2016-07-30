#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "Expression.h"
#include "Parser.h"
#include "Function.h"
#include "Environment.h"
#include "Mutex.h"

#define SYNC_DETECTED 1
#define INVALID_LINE_ERROR 2
#define EMPTY_LINE_DETECTED 3

int Parser::get_expressions_end(const std::string& line, int start) {
	int count = 0;
	for (unsigned int i = start + 1; i < line.size(); ++i) {
		if (line[i] == '(') ++count;
		else if (line[i] == ')' && count) count--;
		else if (line[i] == ')') return i;
	}
	return -1;
}

int Parser::get_element_end(const std::string& line, int start_index) {
	int space_index = line.find_first_of(' ', start_index + 1);
	int parenthesis_index = line.find_first_of(')', start_index + 1);
	if (space_index < 0 || parenthesis_index < space_index) {
		return parenthesis_index;
	} else if (space_index < parenthesis_index) {
		return space_index;
	}
	return -1;
}

Parser::Parser() {
	this->function_factory["+"] = new AddFactory();
	this->function_factory["-"] = new SubFactory();
	this->function_factory["*"] = new MultFactory();
	this->function_factory["/"] = new DivFactory();
	this->function_factory["="] = new EqualFactory();
	this->function_factory["<"] = new LessFactory();
	this->function_factory[">"] = new GreaterFactory();
	this->function_factory["car"] = new CarFactory();
	this->function_factory["cdr"] = new CdrFactory();
	this->function_factory["print"] = new PrintFactory();
	this->function_factory["list"] = new ListFactory();
	this->function_factory["append"] = new AppendFactory();
}

bool Parser::valid_line(const std::string& line) {
	int count = 0;
	for (unsigned int i = 0; i < line.size(); ++i) {
		if (line[i] == '(') ++count;
		else if (line[i] == ')') count--;
		if (count < 0) return false;
	}
	return count == 0;
}

std::string Parser::get_function_name(const std::string& line) {
	int start_index = line.find_first_of('(') + 1;
	int end_index = this->get_element_end(line, start_index);
	std::string name = line.substr(start_index, end_index - start_index);
	return name;
}

bool Parser::has_function(std::string name) {
	if (this->function_factory.find(name) == this->function_factory.end())
		return false;
	return true;
}

LispFunction* Parser::get_function(const std::string& function_name) {
	LispFunctionFactory* factory = this->function_factory[function_name];
	return factory->get_function();
}

std::vector<Argument*> Parser::get_arguments(const std::string& line) {
	std::vector<Argument*> args;

	int args_start = line.find_first_of(' ');
	
	// If the expression has no arguments
	if (args_start < 0) return args;
	
	int end_index;
	std::string str;
	
	for (unsigned int i = args_start; i < line.size(); ++i) {
		if (line[i] == '(') {
			// It's an expression
			end_index = get_expressions_end(line, i);
			str = line.substr(i, end_index - i + 1);
			args.push_back(new Function(str));
			i = end_index;
					
		} else if (line[i] == '"') {
			// It's a string
			end_index = line.find_first_of('"', i + 1);
			str = line.substr(i + 1, end_index - (i + 1));
			args.push_back(new String(str));
			i = end_index;

		} else if (line[i] == ' ') {
			// It's a space
			continue;

		} else if (isdigit(line[i])) {
			// It's an integer
			end_index = this->get_element_end(line, i);
			str = line.substr(i, end_index - i);
			int value;
			std::istringstream(str) >> value;
			args.push_back(new Integer(value));
			i = end_index;
		
		} else if (isalpha(line[i])) {
			// It's a symbol
			end_index = this->get_element_end(line, i);
			str = line.substr(i, end_index - i);
			args.push_back(new Symbol(str));
			i = end_index;
		}
	}
			
	return args;
}

Parser::~Parser() {
	delete this->function_factory["+"];
	delete this->function_factory["-"];
	delete this->function_factory["*"];
	delete this->function_factory["/"];
	delete this->function_factory["="];
	delete this->function_factory["<"];
	delete this->function_factory[">"];
	delete this->function_factory["car"];
	delete this->function_factory["cdr"];
	delete this->function_factory["print"];
	delete this->function_factory["list"];
	delete this->function_factory["append"];
}

int ExpressionThreadParser::operator()(const std::string& line, 
	std::vector<ExpressionThread*>* threads, ContextProtected& context) {
	std::vector<Argument*> args;
	
	if (!line.compare("")) return EMPTY_LINE_DETECTED;
	
	if (!this->parser.valid_line(line)) return INVALID_LINE_ERROR;
	
	std::string function_name = this->parser.get_function_name(line);
	args = this->parser.get_arguments(line);
	
	if (this->parser.has_function(function_name)) {
		LispFunction* function = this->parser.get_function(function_name);
		threads->push_back(new BasicExpressionThread(function, args, context));
		
	} else if (!function_name.compare("setq")) {
		threads->push_back(new SetqExpressionThread(args, context));

	} else if (!function_name.compare("if")) {
		threads->push_back(new ConditionalExpressionThread(args, context));

	} else if (!function_name.compare("sync")) {
		return SYNC_DETECTED;
	}

	return EXIT_SUCCESS;
}

ExpressionThreadParser::~ExpressionThreadParser() {}

Expression* ExpressionParser::operator()(const std::string& line) {
	std::string function_name = this->parser.get_function_name(line);
	std::vector<Argument*> args = this->parser.get_arguments(line);
	
	if (this->parser.has_function(function_name)) {
		LispFunction* function = this->parser.get_function(function_name);
		return new BasicExpression(function, args);

	} else if (!function_name.compare("if")) {
		return new ConditionalExpression(args);
	}
	
	return NULL;
}

ExpressionParser::~ExpressionParser() {}
