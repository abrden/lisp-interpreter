#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <iostream>

#include "Environment.h"
#include "Expression.h"
#include "Parser.h"
#include "Mutex.h"

Clonable::~Clonable() {}

Element::~Element() {}

Argument::~Argument() {}

Symbol::Symbol(std::string identifier) {
	this->identifier = identifier;
}

Element* Symbol::to_element(ContextProtected& context) {
	if (context.has_variable(this->identifier))
		return context.get_variable(this->identifier);
	return new String(this->identifier);
}

Element* Function::to_element(ContextProtected& context) {
	ExpressionParser parser;
	Expression* expression = parser(this->sentence);
	Environment* env = expression->evaluate(context);
	delete expression;
	Element* element = env->get_elements()[0]->clone();
	delete env;
	return element;
}

Function::Function(std::string sentence) {
	this->sentence = sentence;
}

Integer::Integer(int i) {
	this->i = i;
}

Integer::Integer(const Integer& to_clone) {
	this->i = to_clone.i;
}

int Integer::get() {
	return this->i;
}

Element* Integer::to_element(ContextProtected& context) {
	return this->clone();
}

void Integer::add(Element* element) {
	this->i += ((Integer*)element)->get();
}

void Integer::sub(Element* element) {
	this->i -= ((Integer*)element)->get();
}

void Integer::mult(Element* element) {
	this->i *= ((Integer*)element)->get();
}

void Integer::div(Element* element) {
	this->i /= ((Integer*)element)->get();
}

Element* Integer::equal(Element* element) {
	if (this->i == ((Integer*)element)->get()) {
		return new Integer(1);
	}
	return new List();
}

Element* Integer::less(Element* element) {
	if (this->i < ((Integer*)element)->get()) {
		return new Integer(1);
	}
	return new List();
}

Element* Integer::greater(Element* element) {
	if (this->i > ((Integer*)element)->get()) {
		return new Integer(1);
	}
	return new List();
}

Element* Integer::car() {
	return new Integer(-1);
}
Element* Integer::cdr() {
	return new Integer(-1);
}

Element* Integer::print() {
	std::cout << this->get();
	return this;
}

Element* Integer::add_to_list(Element* element) {
	return new Integer(-1);
}

Element* Integer::append(Element* element) {
	return new Integer(-1);
}

bool Integer::is_mergeable() {
	return false;
}

bool Integer::is_false() {
	return (this->i == 0);
}

Integer* Integer::clone() const {
	return new Integer(*this);
}

String::String(std::string string) {
	this->string = string;
}

String::String(const String& to_clone) {
	this->string = to_clone.string;
}

std::string String::get() {
	return this->string;
}

Element* String::to_element(ContextProtected& context) {
	return this->clone();
}

void String::add(Element* element) {}

void String::sub(Element* element) {}

void String::mult(Element* element) {}

void String::div(Element* element) {}

Element* String::equal(Element* element) {
	return new Integer(-1);
}

Element* String::less(Element* element) {
	return new Integer(-1);
}

Element* String::greater(Element* element) {
	return new Integer(-1);
}

Element* String::car() {
	return new Integer(-1);
}

Element* String::cdr() {
	return new Integer(-1);
}

Element* String::print() {
	std::cout << this->get();
	return this;
}

Element* String::add_to_list(Element* element) {
	return new Integer(-1);
}

Element* String::append(Element* element) {
	return new Integer(-1);
}

bool String::is_mergeable() {
	return false;
}

bool String::is_false() {
	return false;
}

String* String::clone() const {
	return new String(*this);
}

List::List() {}

List::List(std::vector<Element*> list) {
	this->list = list;
}

List::List(const List& to_clone) {
	for (unsigned int i = 0; i < to_clone.list.size(); ++i)
		this->list.push_back(to_clone.list[i]->clone());
}

std::vector<Element*> List::get() {
	return this->list;
}

void List::add(Element* element) {}

void List::sub(Element* element) {}

void List::mult(Element* element) {}

void List::div(Element* element) {}

Element* List::equal(Element* element) {
	return new Integer(-1);
}
Element* List::less(Element* element) {
	return new Integer(-1);
}
Element* List::greater(Element* element) {
	return new Integer(-1);
}

Element* List::car() {
	return this->list.front()->clone();
}

Element* List::cdr() {
	if (!this->list.size()) return new List();
	List* copy = this->clone();
	delete copy->list[0];
	copy->list.erase(copy->list.begin());
	return copy;
}

Element* List::print() {
	std::cout << "(";
	for (unsigned int i = 0; i < this->list.size(); ++i) {
		this->list[i]->print();
		if (i + 1 != this->list.size()) std::cout << " ";
	}
	std::cout << ")";
	return this;
}

Element* List::add_to_list(Element* element) {
	Element* clone = element->clone();
	this->list.push_back(clone);
	return this;
}

Element* List::append(Element* element) {
	if (element->is_mergeable())
		return this->merge_with((List*)element);
	return this->add_to_list(element);
}

bool List::is_mergeable() {
	return true;
}

List* List::merge_with(const List* list) {
	for (unsigned int i = 0; i < list->list.size(); ++i)
		this->list.push_back(list->list[i]->clone());
	return this;
}

bool List::is_false() {
	return (this->list.size() == 0);
}

List* List::clone() const {
	return new List(*this);
}

List::~List() {
	for (unsigned int i = 0; i < this->list.size(); ++i)
		delete this->list[i];
}

Environment::Environment() {}

Environment::Environment(Element* element) {
	this->elements.push_back(element);
}

void Environment::add_element(Element* element) {
	this->elements.push_back(element);
}

std::vector<Element*> Environment::get_elements() {
	return this->elements;
}

Environment::~Environment() {
	for (unsigned int i = 0; i < elements.size(); ++i)
		delete this->elements[i];
}
