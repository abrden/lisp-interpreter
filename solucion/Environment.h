#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "Mutex.h"

class Clonable {
	public:
		virtual Clonable* clone() const = 0;
		virtual ~Clonable();		
};

class Element : public Clonable {
	public:
		virtual void add(Element* element) = 0;
		virtual void sub(Element* element) = 0;
		virtual void mult(Element* element) = 0;
		virtual void div(Element* element) = 0;
		virtual Element* equal(Element* element) = 0;
		virtual Element* less(Element* element) = 0;
		virtual Element* greater(Element* element) = 0;
		virtual Element* car() = 0;
		virtual Element* cdr() = 0;
		virtual Element* print() = 0;
		virtual Element* add_to_list(Element* element) = 0;
		virtual Element* append(Element* element) = 0;
		virtual bool is_mergeable() = 0;
		virtual bool is_false() = 0;
		virtual Element* clone() const = 0;
		virtual ~Element();
};

class ContextProtected;

class Argument {
	public:
		virtual Element* to_element(ContextProtected& context) = 0;
		virtual ~Argument();
};

class Symbol : public Argument {
	private:
		std::string identifier;
		
	public:
		explicit Symbol(std::string identifier);
		Element* to_element(ContextProtected& context);
};

class Function : public Argument {
	private:
		std::string sentence;
		
	public:
		explicit Function(std::string sentence);
		Element* to_element(ContextProtected& context);
};

class Integer : public Argument, public Element {
	private:
		int i;
		
	public:
		explicit Integer(int i);
		Integer(const Integer& to_clone);
		int get();
		Element* to_element(ContextProtected& context);
		void add(Element* element);
		void sub(Element* element);
		void mult(Element* element);
		void div(Element* element);
		Element* equal(Element* element);
		Element* less(Element* element);
		Element* greater(Element* element);
		Element* car();
		Element* cdr();
		Element* print();
		Element* add_to_list(Element* element);
		Element* append(Element* element);
		bool is_mergeable();
		bool is_false();
		Integer* clone() const;
};

class String : public Element, public Argument {
	private:
		std::string string;
		
	public:
		explicit String(std::string string);
		String(const String& to_clone);
		std::string get();
		Element* to_element(ContextProtected& context);
		void add(Element* element);
		void sub(Element* element);
		void mult(Element* element);
		void div(Element* element);
		Element* equal(Element* element);
		Element* less(Element* element);
		Element* greater(Element* element);
		Element* car();
		Element* cdr();
		Element* print();
		Element* add_to_list(Element* element);
		Element* append(Element* element);
		bool is_mergeable();
		bool is_false();
		String* clone() const;
};

class List : public Element {
	private:
		std::vector<Element*> list;
		
	public:
		explicit List(std::vector<Element*> list);
		List();
		List(const List& to_clone);
		std::vector<Element*> get();
		void add(Element* element);
		void sub(Element* element);
		void mult(Element* element);
		void div(Element* element);
		Element* equal(Element* element);
		Element* less(Element* element);
		Element* greater(Element* element);
		Element* car();
		Element* cdr();
		Element* print();
		Element* add_to_list(Element* element);
		Element* append(Element* element);
		bool is_mergeable();
		bool is_false();
		List* clone() const;
		List* merge_with(const List* list);
		~List();
};

class Environment {
	private:
		std::vector<Element*> elements;

	public:
		Environment();
		explicit Environment(Element* element);
		void add_element(Element* element);
		std::vector<Element*> get_elements();
		~Environment();
};


#endif // ENVIRONMENT_H
