#include "builder.h"
#include "eventfactory.h"
#include "guifacade.h"
#include "maincontroller.h"
#include "displaycontroller.h"
#include "calculator.h"
#include "inputprocessor.h"


/**
 * @brief Private static method that creates Builder singleton on first
 * invocation and returns reference to that instance on all subsequent
 * invocations. invoked by MainWindow::launch() only (friend).
 * @param uiDisplay reference to uiDisplay.
 * @return reference to singleton Builder instance.
 */
Builder& Builder::getInstance( const Ui::Display& uiDisplay ) {
    if( _this == nullptr ) {
        _this = new Builder( uiDisplay );
    }
    return *_this;
}

Builder *Builder::_this = nullptr;


Builder::~Builder() {
    logDestructor( "Builder" );
}


/**
 * @brief build central method of Builder singleton instance to create
 * and configure system components.
 * @return true in case of successful completion.
 */
bool Builder::build() {
    // Build EventFactory singleton instance and initialize member variable.
    eventFactory = &XEventFactory::getInstance();

    // Controllers can use a simple PublisherImpl to publish control messages.
    PublisherIntf *ctrlMsgPublisherImpl = new SimplePublisherImpl( "CtrlMsgPublisherImpl" );
    mainController = &MainController::getInstance( "MainController", *this, ctrlMsgPublisherImpl );

    // GuiFacade requires a binding to the underlying uiDisplay passed from the
    // Builder's constructor. Then, a publisher of input events is injected to
    // which inputProcessor and potentially other subscribers can subscribe.
    PublisherIntf *inpEvtPublisherImpl = new SimplePublisherImpl( "inpEvtPublisherImpl" );
    guiFacade = &GuiFacade::getInstance( "GuiFacade", *this, uiDisplay, *inpEvtPublisherImpl );

    displayController = &DisplayController::getInstance( "DisplayController", *this, ctrlMsgPublisherImpl );
    calculatorUnit = &Calculator::getInstance( "CalculatorUnit" );
    inputProcessor = &InputProcessor::getInstance( "InputProcessor", *this, *calculatorUnit, ctrlMsgPublisherImpl );

    // inputProcessor must subscribe at guiFacade to receive input events.
    // This is the same as:
    // inpEvtPublisherImpl->subscribe( *inputProcessor );
    guiFacade->subscribe( *inputProcessor );

    // Logging can be configured by creating logger instances that implement
    // SubscriberIntf and subscribe to publishers in controllers.
    ctrlMsgLogger = new SimpleLogger( "Simple Ctrl-msg logger" );
    if( ctrlMsgLogger ) {
        mainController->subscribe( *ctrlMsgLogger );
        displayController->subscribe( *ctrlMsgLogger );
        inputProcessor->subscribe( *ctrlMsgLogger );
    }

    keyEventLogger = new SimpleLogger( "Key Input Logger" );
    if( keyEventLogger ) {
        guiFacade->subscribe( *keyEventLogger );
    }

    return true;
}


/**
 * @brief destroy is the counter-method to build tearing down all
 * previously built components.
 */
void Builder::destroy() {
    if( keyEventLogger ) {
        guiFacade->unsubscribe( *keyEventLogger );
        delete keyEventLogger;
    }
    if( ctrlMsgLogger ) {
        inputProcessor->unsubscribe( *ctrlMsgLogger );
        displayController->unsubscribe( *ctrlMsgLogger );
        mainController->unsubscribe( *ctrlMsgLogger );
        delete ctrlMsgLogger;
    }
    delete inputProcessor;
    delete calculatorUnit;
    delete displayController;
    delete mainController;
    delete guiFacade;
    delete eventFactory;
}
