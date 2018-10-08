#include "maincontroller.h"
#include "builder.h"
#include "displaycontroller.h"
#include "inputprocessor.h"
#include "eventfactory.h"
#include "guifacade.h"


/**
 * @brief Private static method that creates MainController singleton on first
 * invocation and returns reference to that instance on all subsequent
 * invocations. Invoked by Builder only (friend).
 * @param name of controller.
 * @param builder reference to builder instance.
 * @param pub reference to optional publisher of controller events.
 * @return reference to singleton MainController instance.
 */
MainController& MainController::getInstance( const string name, Builder& builder, PublisherIntf *pub ) {
    if( _this == nullptr ) {
        _this = new MainController( name, builder, pub );
    }
    return *_this;
}

MainController *MainController::_this = nullptr;


ControllerIntf::~ControllerIntf() {
    //logDestructor( getName() );
}

MainController::~MainController() {
    logDestructor( getName() );
}


/**
 * @brief probe propagates probe() invocations to other controllers and
 * attempts to transition operational state to Probing.
 * @param cb optional callback object at which the callback(XEvent *e)-
 * method is invoked when the probe-cycle has completed.
 */
void MainController::probe( Callback* /* cb */ ) {
    if( opState==Stopped || opState==RUNNING ) {
        opState = transitionTo( OpSt::Probing );

        builder.getInputProcessor().probe();

        class DisplayControllerProbeCallback : public Callback {
          public:
            DisplayControllerProbeCallback( MainController& me ) : me( me ) {}
            virtual ~DisplayControllerProbeCallback() {}

            void callback( XEvent *probeResult ) {
                if( probeResult->ev == RUNNING ) {
                    // completes probing stage and transitions to running
                    me.opState = me.transitionTo( OpSt::RUNNING );
                    Builder::getInstance().getDisplayController().start();
                    XEventFactory& ef = Builder::getInstance().getEventFactory();
                    XEvent& xe_C = ef.getEvent( GuiFacade::KeyEvt::C );
                    Builder::getInstance().getInputProcessor().notify( xe_C );
                }
            }
          private:
            MainController& me;
        };

        DisplayControllerProbeCallback *cb1 = new DisplayControllerProbeCallback( *this );
        builder.getDisplayController().probe( cb1 );
    }
}

/**
 * @brief start propagates start() invocations to other controllers and
 * attempts to transition operational state to RUNNING.
 */
void MainController::start() {
    if( opState==Undef ) {  // initial state
        opState = transitionTo( OpSt::Stopped );
    }
    if( opState==Stopped ) {
        if( enableProbing ) {
            probe();
        } else {
            opState = transitionTo( OpSt::RUNNING );
            Builder::getInstance().getDisplayController().start();
        }
        XEventFactory& ef = builder.getEventFactory();
        publish( ef.getEvent( getName() + " started." ) );
    }
}

/**
 * @brief stop transitions operational state to Stopping and propagates
 * stop() invocations to other controllers ending in state Stopped.
 * @param exit terminate programm if set to true.
 */
void MainController::stop( bool exit ) {
    XEventFactory& ef = builder.getEventFactory();
    if( opState==Probing || opState==RUNNING ) {
        publish( ef.getEvent( getName() + " stopping." ) );
                opState = transitionTo( OpSt::Stopping );
        builder.getDisplayController().stop( false );

        publish( ef.getEvent( getName() + " stopped." ) );
        opState = transitionTo( OpSt::Stopped );
    }
    if( exit ) {
        opState = transitionTo( OpSt::Undef );
        publish( ef.getEvent( "Exiting." ) );
        builder.destroy();
        delete &builder;
    }
}

/**
 * @brief transitionTo internal method to perform transitions of operational state.
 * @param to desired next state.
 * @return state that actually has been achieved.
 */
MainController::OpSt MainController::transitionTo( OpSt to ) {
    if( opState != RUNNING && to==RUNNING ) {
        builder.getInputProcessor().start();
    }
    if( opState==RUNNING && to != RUNNING ) {
        builder.getInputProcessor().stop();
    }
    return to;
}
