#include "calculator.h"
#include "guifacade.h"
void logDestructor( string msg );


/**
 * @brief Private static method that creates Calculator singleton on first
 * invocation and returns reference to that instance on all subsequent
 * invocations. Used by Builder (friend).
 * @param name of calculator singleton instance.
 * @return reference to singleton Calculator instance.
 */
Calculator& Calculator::getInstance( const string name ) {
    if( _this == nullptr ) {
        _this = new Calculator( name );
    }
    return *_this;
}

Calculator *Calculator::_this = nullptr;


Calculator::~Calculator() {
    logDestructor( name );
}


/**
 * @brief Calculator::push operand onto stack. Calculator::top() returns
 * the top element of the operand stack without changing the the operand
 * stack.
 * @param d operand (double).
 */
void Calculator::push( double d ) {
    operandSt.push_back( d );
}

double Calculator::top() {
    return operandSt.size() > 0? operandSt.back() : 0.0;
}


/**
 * @brief Calculator::pushOp pushes operator onto operator stack.
 * Calculator::SetOp replaces the current top element of operator
 * stack.
 * Pushing operators triggers calculations and reduction of the
 * operand stack. Results are at the top of the operand stack.
 * @param op.
 */
void Calculator::pushOp( int op ) {
    calc();     // trigger calculation
    if( op != GuiFacade::EQ ) {
        opSt.push_back( op );
    }
}

void Calculator::setOp( int op ) {
    if( op != GuiFacade::EQ && opSt.size() > 0 ) {
        opSt.pop_back();
    }
    //opSt.push_back( op );
    pushOp( op );
}

/**
 * @brief Calculator::pop top level operand from operand stack.
 * @return top level element of operand stack
 */
double Calculator::pop() {
    double d = top();
    if( operandSt.size() > 0 ) {
        operandSt.pop_back();
    }
    return d;
}

/**
 * @brief Calculator::topOp top-level operator. No change of stack.
 * @return top level element of operator stack.
 */
int Calculator::topOp() {
    return opSt.size() > 0? opSt.back() : GuiFacade::EQ;
}

/**
 * @brief Calculator::popOp pop top-level operator.
 * @return top level element of operator stack.
 */
int Calculator::popOp() {
    int op = topOp();
    if( opSt.size() > 0 ) {
        opSt.pop_back();
    }
    return op;
}


/**
 * @brief Calculator::calc perform calculation of supported operators.
 */
void Calculator::calc() {
    showStacks( "===> CALCULATE:\t{ " );
    if( opSt.size() >= 1 && operandSt.size() >= 2 ) {
        int op = popOp();
        if( op==GuiFacade::EQ || op==GuiFacade::Plus || op==GuiFacade::Minus ||
            op==GuiFacade::Mul || op==GuiFacade::Div )
        {
            double d2 = pop();
            double d1 = pop();
            switch( op ) {
            case GuiFacade::EQ:     break;
            case GuiFacade::Plus:   d1 = d1 + d2; break;
            case GuiFacade::Minus:  d1 = d1 - d2; break;
            case GuiFacade::Mul:    d1 = d1 * d2; break;
            case GuiFacade::Div:
                if( d2 < 0.000000001 && d2 > -0.000000001 ) {
                    throw invalid_argument( "div/0" );
                } else {
                    d1 = d1 / d2;
                } break;
            }
            push( d1 );
        }
    }
    showStacks( " };\tRESULT: { ", " }\n" );
}


/**
 * @brief Calculator::clearAll clears operator and operand stacks.
 */
void Calculator::clearAll() {
    operandSt.clear();
    opSt.clear();
    showStacks( "===> CLEARED:\t{ ", " }\n" );
}

/**
 * @brief Calculator::clearTop clears top-level elements of operand
 * and operator stacks.
 */
void Calculator::clearTop() {
    if( operandSt.size() > 0 ) {
        operandSt.pop_back();
        operandSt.push_back( 0.0 );
    }
    showStacks( "===> CLEAR_CE:\t{ ", "\n" );
}


/**
 * @brief Calculator::showStacks lists operand and operator stacks
 * to cout, if enabled.
 */
void Calculator::showStacks( string , string ) { //prefix, string postfix ) {
  /*
    cout << prefix << "OP[ ";
    for( vector<int>::iterator itOp = opSt.begin(); itOp != opSt.end(); itOp++ ) {
        int op = *itOp;
        const static string OP_[] = { "+", "-", "*", "/", "%", "VAT", "=", };
        cout << "" << OP_[ op - GuiFacade::Plus ] << ", ";
    }
    cout << "], n[ ";
    for( vector<double>::iterator itN = operandSt.begin(); itN != operandSt.end(); itN++ ) {
        double d = *itN;
        cout << "" << d << ", ";
    }
    cout << "]" << postfix;
  */
}
