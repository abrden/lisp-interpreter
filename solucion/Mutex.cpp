#include <sstream>
#include <string>
#include <map>
#include <pthread.h>

#include "Environment.h"

Mutex::Mutex() {
	pthread_mutex_init(&this->mutex, NULL);	
}

void Mutex::lock() {
	pthread_mutex_lock(&this->mutex);
}

void Mutex::unlock() {
	pthread_mutex_unlock(&this->mutex);
}
        
Mutex::~Mutex() {
	pthread_mutex_destroy(&this->mutex);
}

Lock::Lock(Mutex &m) : m(m) {
	m.lock();
}

Lock::~Lock() {
	m.unlock();
}

Element* Context::get_variable(std::string identifier) {
	return this->variables[identifier]->clone();
}

void Context::set_variable(std::string identifier, Element* element) {
	if (this->has_variable(identifier))
		delete this->variables[identifier];
	this->variables[identifier] = element;
}

bool Context::has_variable(std::string identifier) {
	if (this->variables.find(identifier) == this->variables.end())
		return false;
	return true;
}

Context::~Context() {
	for (std::map<std::string, Element*>::iterator it = this->variables.begin(); 
		it != this->variables.end(); ++it)
		delete this->variables[it->first];
}

Element* ContextProtected::get_variable(std::string identifier) {
	Lock l(this->m);
	return this->context.get_variable(identifier);
}

void ContextProtected::set_variable(std::string identifier, Element* element) {
	Lock l(this->m);
	this->context.set_variable(identifier, element);
}

bool ContextProtected::has_variable(std::string identifier) {
	Lock l(this->m);
	return this->context.has_variable(identifier);
}

ContextProtected::~ContextProtected() {}
