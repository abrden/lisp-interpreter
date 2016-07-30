#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>

#include "Environment.h"

class LispFunction {
	public:
		virtual Element* run(std::vector<Element*>& parameters) = 0;
		virtual ~LispFunction();
};

class AddFunction : public LispFunction {
	public:
		Element* run(std::vector<Element*>& parameters);
};
class SubFunction : public LispFunction {
	public:
		Element* run(std::vector<Element*>& parameters);
};
class MultFunction : public LispFunction {
	public:
		Element* run(std::vector<Element*>& parameters);
};
class DivFunction : public LispFunction {
	public:
		Element* run(std::vector<Element*>& parameters);
};
class EqualFunction : public LispFunction {
	public:
		Element* run(std::vector<Element*>& parameters);
};
class LessFunction : public LispFunction {
	public:
		Element* run(std::vector<Element*>& parameters);
};
class GreaterFunction : public LispFunction {
	public:
		Element* run(std::vector<Element*>& parameters);
};
class CarFunction : public LispFunction {
	public:
		Element* run(std::vector<Element*>& parameters);
};
class CdrFunction : public LispFunction {
	public:
		Element* run(std::vector<Element*>& parameters);
};
class PrintFunction : public LispFunction {
	public:
		Element* run(std::vector<Element*>& parameters);
};
class ListFunction : public LispFunction {
	public:
		Element* run(std::vector<Element*>& parameters);
};
class AppendFunction : public LispFunction {
	public:
		Element* run(std::vector<Element*>& parameters);
};

class LispFunctionFactory {
	public:
		virtual LispFunction* get_function() = 0;
		virtual ~LispFunctionFactory();
};

class AddFactory : public LispFunctionFactory {
	public:
		LispFunction* get_function();
};
class SubFactory : public LispFunctionFactory {
	public:
		LispFunction* get_function();
};
class MultFactory : public LispFunctionFactory {
	public:
		LispFunction* get_function();
};
class DivFactory : public LispFunctionFactory {
	public:
		LispFunction* get_function();
};
class EqualFactory : public LispFunctionFactory {
	public:
		LispFunction* get_function();
};
class LessFactory : public LispFunctionFactory {
	public:
		LispFunction* get_function();
};
class GreaterFactory : public LispFunctionFactory {
	public:
		LispFunction* get_function();
};
class CarFactory : public LispFunctionFactory {
	public:
		LispFunction* get_function();
};
class CdrFactory : public LispFunctionFactory {
	public:
		LispFunction* get_function();
};
class PrintFactory : public LispFunctionFactory {
	public:
		LispFunction* get_function();
};
class ListFactory : public LispFunctionFactory {
	public:
		LispFunction* get_function();
};
class AppendFactory : public LispFunctionFactory {
	public:
		LispFunction* get_function();
};

#endif // FUNCTION_H
