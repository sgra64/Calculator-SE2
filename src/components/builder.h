#ifndef BUILDER_H
#define BUILDER_H

#include <iostream>
#include "display.h"
using namespace std;
class XEventFactory;
class GuiFacade;
class MainController;
class DisplayController;
class InputProcessor;
class Calculator;
class SubscriberIntf;


/**
 * @brief The Builder class defines a singleton instance that creates
 * and configures system components. Builder is a design pattern.
 * Builder also stores references to system components and provides
 * getter methods.
 *
 * The Builder instance itself is created in MainWindow::launch().
 *
 */
class Builder {
    friend class MainWindow;        // invokes getInstance(&gui), build()
    friend class MainController;    // invokes builder.destroy();

  public:

    static Builder& getInstance() { return *_this; }

    XEventFactory& getEventFactory() { return *eventFactory; }
    GuiFacade *getGui() { return guiFacade; }
    MainController *getMainController() { return mainController; }
    DisplayController& getDisplayController() { return *displayController; }
    InputProcessor& getInputProcessor() { return *inputProcessor; }
    Calculator& getCalculatorUnit() { return *calculatorUnit; }

  private:

    /**
     * @brief Private constructor invoked by getInstance( ... ).
     * @param uiDisplay reference to uiDisplay that is temporarily stored
     * in Builder until build() is called and reference passed to
     * creation of guiFacade component.
     */
    Builder( const Ui::Display& uiDisplay ) : uiDisplay( uiDisplay ) {
        _this = this;
    }

    ~Builder();

    /**
     * @brief Private static method that creates Builder singleton on first
     * invocation and returns reference to that instance on all subsequent
     * invocations. Invoked by MainWindow::launch() only (friend).
     * @param uiDisplay reference to uiDisplay.
     * @return reference to singleton Builder instance.
     */
    static Builder& getInstance( const Ui::Display& uiDisplay );

    /**
     * @brief build central method of Builder singleton instance to create
     * and configure system components.
     * @return true in case of successful completion.
     */
    bool build();

    /**
     * @brief destroy is the counter-method to build tearing down all
     * previously built components.
     */
    void destroy();


    /*
     * @brief Private member variables.
     */
    XEventFactory *eventFactory;
    GuiFacade *guiFacade;
    MainController *mainController;
    DisplayController *displayController;
    InputProcessor *inputProcessor;
    Calculator *calculatorUnit;

    SubscriberIntf *ctrlMsgLogger = nullptr;
    SubscriberIntf *keyEventLogger = nullptr;

    const Ui::Display& uiDisplay;   // temp, passed to GuiFacade for build
    static Builder *_this;          // private static pointer declaration
                                    // for singleton instance
};


/**
 * @brief logDestructor is a helper method to log destructor invocations
 * when other system components have already been shut down.
 * @param msg log message.
 */
void logDestructor( string msg );


#endif // BUILDER_H
