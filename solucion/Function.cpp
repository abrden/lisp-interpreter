#include <vector>
#include <iostream>

#include "Environment.h"
#include "Function.h"

LispFunction::~LispFunction() {}

LispFunctionFactory::~LispFunctionFactory() {}

Element* AddFunction::run(std::vector<Element*>& parameters) {	
	Element* element = parameters[0]->clone();
	
	for (unsigned int i = 1; i < parameters.size(); ++i) {
		element->add(parameters[i]);
	}
	
	return element;
}

Element* SubFunction::run(std::vector<Element*>& parameters) {
	Element* element = parameters[0]->clone();
	
	for (unsigned int i = 1; i < parameters.size(); ++i) {
		element->sub(parameters[i]);
	}
	
	return element;
}

Element* MultFunction::run(std::vector<Element*>& parameters) {
	Element* element = parameters[0]->clone();
	
	for (unsigned int i = 1; i < parameters.size(); ++i) {
		element->mult(parameters[i]);
	}
	
	return element;
}

Element* DivFunction::run(std::vector<Element*>& parameters) {
	Element* element = parameters[0]->clone();
	
	for (unsigned int i = 1; i < parameters.size(); ++i) {
		element->div(parameters[i]);
	}
	
	return element;
}

Element* EqualFunction::run(std::vector<Element*>& parameters) {
	Element* first = parameters[0];
	Element* second = parameters[1];
	return first->equal(second);
}

Element* LessFunction::run(std::vector<Element*>& parameters) {
	Element* first = parameters[0];
	Element* second = parameters[1];
	return first->less(second);
}

Element* GreaterFunction::run(std::vector<Element*>& parameters) {
	Element* first = parameters[0];
	Element* second = parameters[1];
	return first->greater(second);
}

Element* CarFunction::run(std::vector<Element*>& parameters) {
	Element* list = parameters[0];
	return list->car();
}

Element* CdrFunction::run(std::vector<Element*>& parameters) {
	Element* list = parameters[0];
	return list->cdr();
}

Element* PrintFunction::run(std::vector<Element*>& parameters) {
	for (unsigned int i = 0; i < parameters.size(); ++i) {
		parameters[i]->print();
		if (i + 1 != parameters.size()) std::cout << " ";
	}
	std::cout << "\n";

	return NULL;
}

Element* ListFunction::run(std::vector<Element*>& parameters) {
	List* list = new List();
	for (unsigned int i = 0; i < parameters.size(); ++i)
		list->add_to_list(parameters[i]);
	return list;
}

Element* AppendFunction::run(std::vector<Element*>& parameters) {
	Element* new_list = parameters[0]->clone();
	for (unsigned int i = 1; i < parameters.size(); ++i)
		new_list->append(parameters[i]);
	return new_list;
}

LispFunction* AddFactory::get_function() {
	return new AddFunction();
}
LispFunction* SubFactory::get_function() {
	return new SubFunction();
}
LispFunction* MultFactory::get_function() {
	return new MultFunction();
}
LispFunction* DivFactory::get_function() {
	return new DivFunction();
}
LispFunction* EqualFactory::get_function() {
	return new EqualFunction();
}
LispFunction* LessFactory::get_function() {
	return new LessFunction();
}
LispFunction* GreaterFactory::get_function() {
	return new GreaterFunction();
}
LispFunction* CarFactory::get_function() {
	return new CarFunction();
}
LispFunction* CdrFactory::get_function() {
	return new CdrFunction();
}
LispFunction* PrintFactory::get_function() {
	return new PrintFunction();
}
LispFunction* ListFactory::get_function() {
	return new ListFunction();
}
LispFunction* AppendFactory::get_function() {
	return new AppendFunction();
}
