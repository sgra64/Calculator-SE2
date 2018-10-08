#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <iostream>
#include <vector>

using namespace std;


/**
 * @brief The Calculator class implements simple calculator logic using
 * two stacks, one for operands and results and one for operators.
 * Numbers of type double are used for operands. Operators are of type
 * GuiFacade::KeyEvt: { Plus=12, Minus=13, Mul=14, Div=15, Percent=16,
 * VAT=17, EQ=18 };
 *
 * Calculations are triggered by pushing operators to the operator stack.
 * For a calculation, operand(s) are popped from the operand stack and
 * the result is pushed back onto the operand stack.
 *
 * TODO: The current implementation does not consider operator precedense
 * and brackets '(' and ')'. Percent and value-added-tax (VAT) calculation
 * are also not supported and have no effect.
 */
class Calculator {
    friend class Builder;

  public:
    /**
     * @brief push operand onto stack. top() returns the top element of
     * the operand stack without changing the the operand stack.
     * @param d operand (double).
     */
    void push( double d );
    double top();

    /**
     * @brief pushOp pushes operator onto operator stack.
     * SetOp replaces the current top element of operator stack.
     * Pushing operators triggers calculations and reduction of the
     * operand stack. Results are at the top of the operand stack.
     * @param op.
     */
    void pushOp( int op );
    void setOp( int op );

    /**
     * @brief clear methods clear operand and operator stacks.
     */
    void clearAll();
    void clearTop();

  private:
    /**
     * @brief Private constructor invoked by Calculator::getInstance() only.
     * @param name of calculator singleton instance.
     */
    Calculator( const string name ) : name( name ) {}
    ~Calculator();

    /**
     * @brief Private static method that creates Calculator singleton on first
     * invocation and returns reference to that instance on all subsequent
     * invocations. Used by Builder (friend).
     * @param name of calculator singleton instance.
     * @return reference to singleton Calculator instance.
     */
    static Calculator& getInstance( const string name );

    /**
     * @brief Private methods used by Calculator internally.
     */
    double pop();
    int topOp();
    int popOp();

    void calc();

    void showStacks( string prefix, string postfix = "" );


    vector<double> operandSt = {};  // vector used for operand stack
    vector<int> opSt = { };         // vector used for operator stack

    const string name;

    static Calculator *_this;   // private static pointer declaration
                                // for singleton instance
};

#endif // CALCULATOR_H
