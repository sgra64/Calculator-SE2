#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include "controllerintf.h"
class Builder;
class Calculator;
using namespace std;


/**
 * @brief The InputProcessor class defines the InputProcessor controller
 * that processes input events such as keypad or keypress actions assembling
 * sequences of number-events into numerical numbers suitable for calculcation.
 *
 * InputProcessor subscribes at the GuiFacade instance in order to receive
 * input events. Input events are collected in buffers where they can be edited
 * (backspace event). Buffer content is mirrored to the display.
 *
 * Inputprocessor is in one of the following MODE's {CalculatorMode,TimerMode}.
 * In CalculatorMode, input is processed for calculator functions (construct
 * numerical numbers as operands and operators from input events and invoke
 * calculator logic to perform calculations).
 * In TimerMode, input for the time/timer function is processed and the timer
 * logic is invoked.
 * Receiving a mode-event, toggles mode.
 */
class InputProcessor : public ControllerIntf, public SubscriberIntf {
    friend class Builder;

  public:
    /**
     * @brief Methods inherited from ControllerIntf.
     * @param cb callback object.
     * @param exit terminate programm if set to true.
     */
    virtual void probe( Callback *cb = nullptr );
    virtual void start();
    virtual void stop( bool exit = false );

    /**
     * @brief notify method inherited from SubscriberIntf is invoked when
     * an input event such as keypad- or keypress-event has occured.
     * notify() dispatches events to sub-methods notify_CalcMode and
     * notify_TimerMode depending on InputProcessor mode.
     * @param e input event.
     */
    virtual void notify( XEvent& e );

    /**
     * @brief getName returns the name of the InputProcessor instance.
     * @return name of the InputProcessor instance.
     */
    virtual const std::string getName() const { return ControllerIntf::getName(); }

  private:
    /**
     * @brief Private constructor invoked by getInstance( ... ).
     * @param name name of the InputProcessor instance.
     * @param builder reference to builder instance.
     * @param alu reference to Calculator logic (algorithmic logical unit).
     * @param pub reference to optional publisher of controller events.
     */
    InputProcessor( const std::string name, Builder& builder,
        Calculator& alu, PublisherIntf *pub = nullptr )
      : ControllerIntf( name, pub ), SubscriberIntf( name ),
        builder( builder ), alu( alu )
    {}

    virtual ~InputProcessor();

    /**
     * @brief Private static method that creates InputProcessor singleton on first
     * invocation and returns reference to that instance on all subsequent
     * invocations. Invoked by Builder only (friend).
     * @param name name of the InputProcessor instance.
     * @param builder reference to builder instance.
     * @param alu reference to Calculator logic (algorithmic logical unit).
     * @param pub reference to optional publisher of controller events.
     * @return reference to singleton InputProcessor instance.
     */
    static InputProcessor& getInstance( std::string name, Builder& builder, Calculator& alu, PublisherIntf *pub = nullptr );

    /**
     * @brief notify_CalcMode sub-method invoked by notify() in CalculatorMode.
     * @param e input event.
     */
    void notify_CalcMode( XEvent& e );

    /**
     * @brief notify_TimerMode sub-method invoked by notify() in TimerMode.
     * @param e input event.
     */
    void notify_TimerMode( XEvent& e );


    /*
     * @brief Private member variables.
     */
    enum MODE { CalculatorMode, TimerMode };
    MODE mode = CalculatorMode;

    string bufNumber = "0";         // buffer to collect and display input in CalculatorMode
    string bufTime = "12:00:00";    // buffer to collect and display input in TimerMode

    bool err = false;                   // indicates Error-condition

    enum INPUT_MODE { numbers, op };    // sub-mode in CalculatorMode indicating whether
    int inpmode_ = numbers;             // input events relate to numbers or operators

    Builder& builder;                   // reference to builder instance
    Calculator& alu;                    // reference to calculator logic

    static InputProcessor *_this;       // private static pointer declaration
                                        // for singleton instance

};

#endif // INPUTPROCESSOR_H
