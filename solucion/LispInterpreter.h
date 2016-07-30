#ifndef LISP_INTERPRETER_H
#define LISP_INTERPRETER_H

#include <vector>

#include "Expression.h"
#include "Mutex.h"

class LispInterpreter {
	private:
		ContextProtected context;
		std::vector<ExpressionThread*> threads;
		
		void join_threads();
		void delete_threads();

	public:
		int execute();
		~LispInterpreter();
};

#endif // LISP_INTERPRETER_H
