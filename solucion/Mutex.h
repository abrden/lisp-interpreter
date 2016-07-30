#ifndef MUTEX_T
#define MUTEX_T

#include <sstream>
#include <string>
#include <map>
#include <pthread.h>

#include "Environment.h"
class Element;

class Mutex {
	private:
		pthread_mutex_t mutex;

	public:
		Mutex();
		void lock();
		void unlock();
        ~Mutex();

    private:
        Mutex(const Mutex&);
        Mutex& operator=(const Mutex&);
};

class Lock {
    private:
        Mutex &m;

    public:
        explicit Lock(Mutex &m);
        ~Lock();

    private:
        Lock(const Lock&);
        Lock& operator=(const Lock&);
};

class Context {
	private:
		std::map<std::string, Element*> variables;
		
	public:
		Element* get_variable(std::string identifier);
		void set_variable(std::string identifier, Element* element);
		bool has_variable(std::string identifier);
		~Context();
};

class ContextProtected {
	private:
		Mutex m;
		Context context;
	
	public:
		Element* get_variable(std::string identifier);
		void set_variable(std::string identifier, Element* element);
		bool has_variable(std::string identifier);
		~ContextProtected();
};

#endif // MUTEX_T
