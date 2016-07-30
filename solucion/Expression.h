#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <pthread.h>
#include <sstream>
#include <vector>

#include "Function.h"
#include "Environment.h"
#include "Mutex.h"

class Expression {
	public:
		virtual Environment* evaluate(ContextProtected& context) = 0;
		virtual ~Expression();
};

class BasicExpression : public Expression {
	protected:
		LispFunction* function;
		std::vector<Argument*> args;
		
		Environment* create_environment(ContextProtected& context);

	public:
		BasicExpression(LispFunction* function, std::vector<Argument*>& args);	
		Environment* evaluate(ContextProtected& context);
		~BasicExpression();
};

class ConditionalExpression : public Expression {
	protected:
		Argument* condition;
		Argument* true_result;
		Argument* false_result;
		
	public:
		explicit ConditionalExpression(std::vector<Argument*>& args);
		Environment* evaluate(ContextProtected& context);
		~ConditionalExpression();
};

class ExpressionThread {
    protected:
        pthread_t thread;
        ContextProtected& context;
 
        static void* runner(void *data) { 
            ExpressionThread *self = (ExpressionThread*)data;
            self->run();
            
            return NULL;
        }

    public:
        explicit ExpressionThread(ContextProtected& context);
        void start();
        void join();
        virtual void run() = 0;
        virtual ~ExpressionThread();
    
    private:
        ExpressionThread(const ExpressionThread&);
        ExpressionThread& operator=(const ExpressionThread&);
};

class SetqExpressionThread : public Expression, public ExpressionThread {
	private:
		Argument* identifier;
		Argument* variable;
	
	public:
		SetqExpressionThread(std::vector<Argument*>& args, ContextProtected& context);
		Environment* evaluate(ContextProtected& context);
		void run();
		~SetqExpressionThread();
};

class BasicExpressionThread : 
public BasicExpression, public ExpressionThread {
	public:
		BasicExpressionThread(LispFunction* function, 
			std::vector<Argument*>& args, ContextProtected& context);
		void run();
};

class ConditionalExpressionThread : 
public ConditionalExpression, public ExpressionThread {		
	public:
		ConditionalExpressionThread(std::vector<Argument*>& args, 
			ContextProtected& context);
		void run();
};

#endif // EXPRESSION_H
