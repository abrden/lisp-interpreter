#include <pthread.h>
#include <sstream>
#include <vector>

#include "Expression.h"
#include "Function.h"
#include "Environment.h"
#include "Mutex.h"

#include <iostream>

Expression::~Expression() {}

BasicExpression::BasicExpression
	(LispFunction* function, std::vector<Argument*>& args) {
	this->function = function;
	this->args = args;
}

Environment* BasicExpression::create_environment(ContextProtected& context) {
	Environment* env = new Environment();
	for (unsigned int i = 0; i < this->args.size(); ++i)
		env->add_element(this->args[i]->to_element(context));
	return env;
}

Environment* BasicExpression::evaluate(ContextProtected& context) {
	Environment* env = this->create_environment(context);
	std::vector<Element*> elements = env->get_elements();
	Element* result = this->function->run(elements); 
	delete env;
	return new Environment(result);
}

BasicExpression::~BasicExpression() {
	delete this->function;
	
	for (unsigned int i = 0; i < this->args.size(); ++i)
		delete this->args[i];
}

ConditionalExpression::ConditionalExpression(std::vector<Argument*>& args) {
	this->condition = args[0];
	this->true_result = args[1];
	this->false_result = args[2];
}

Environment* ConditionalExpression::evaluate(ContextProtected& context) {
	Environment* env = new Environment();
	
	Element* condition_element = this->condition->to_element(context);
	
	if (condition_element->is_false())
		env->add_element(this->false_result->to_element(context));
	else
		env->add_element(this->true_result->to_element(context));
		
	delete condition_element;
	return env;
}

ConditionalExpression::~ConditionalExpression() {
	delete this->condition;
	delete this->true_result;
	delete this->false_result;
}

ExpressionThread::ExpressionThread(ContextProtected& context) :
	context(context) {}

void ExpressionThread::start() {
	pthread_create(&(this->thread), NULL, ExpressionThread::runner, this);
}

void ExpressionThread::join() {
	pthread_join(this->thread, NULL);
}

ExpressionThread::~ExpressionThread() {}

SetqExpressionThread::SetqExpressionThread
	(std::vector<Argument*>& args, ContextProtected& context) :
	ExpressionThread(context) {
	this->identifier = args[0];
	this->variable = args[1];
}

Environment* SetqExpressionThread::evaluate(ContextProtected& context) {
	String* str = (String*)this->identifier;
	Element* var = this->variable->to_element(context);
	context.set_variable(str->get(), var);
	return NULL;
}

void SetqExpressionThread::run() {
	Environment* env = this->evaluate(this->context);
	delete env;
}

SetqExpressionThread::~SetqExpressionThread() {
	delete this->identifier;
	delete this->variable;
}

BasicExpressionThread::BasicExpressionThread(LispFunction* function, 
	std::vector<Argument*>& args, ContextProtected& context) :
	BasicExpression(function, args),
	ExpressionThread(context) {}

void BasicExpressionThread::run() {
	Environment* env = BasicExpression::evaluate(this->context);
	delete env;
}

ConditionalExpressionThread::ConditionalExpressionThread
	(std::vector<Argument*>& args, ContextProtected& context) :
	ConditionalExpression(args),
	ExpressionThread(context) {}

void ConditionalExpressionThread::run() {
	Environment* env = ConditionalExpression::evaluate(this->context);
	delete env;
}
