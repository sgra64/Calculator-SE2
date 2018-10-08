#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "controllerintf.h"
class Builder;
using namespace std;


/**
 * @brief The MainController class defines the central controller instance that
 * controls all other controllers. Invoking probe(), start(), stop() will propagate
 * invocations to other controllers.
 *
 * MainController also manages transitions of operational state (OpSt) of the
 * application between { Undef, Stopped, Probing, RUNNING, Stopping }.
 *
 * Input events received asynchronously from the Gui are only passed for further
 * processing in state RUNNING and ignored in other states.
 *
 */
class MainController : public ControllerIntf {
    friend class Builder;

  public:
    /**
     * Operational states of the application.
     */
    enum OpSt { Undef, Stopped, Probing, RUNNING, Stopping };

    /**
     * @brief probe propagates probe() invocations to other controllers and
     * attempts to transition operational state to Probing.
     * @param cb optional callback object at which the callback(XEvent *e)-
     * method is invoked when the probe-cycle has completed.
     */
    virtual void probe( Callback *cb = nullptr );

    /**
     * @brief start propagates start() invocations to other controllers and
     * attempts to transition operational state to RUNNING.
     */
    virtual void start();

    /**
     * @brief stop transitions operational state to Stopping and propagates
     * stop() invocations to other controllers ending in state Stopped.
     * @param exit terminate programm if set to true.
     */
    virtual void stop( bool exit = false );

    /**
     * @brief isRunning test application is in RUNNING state.
     * @return true if application is in RUNNING state.
     */
    bool isRunning() { return opState==RUNNING; }


  private:
    /**
     * @brief Private constructor invoked by getInstance( ... ).
     * @param name of controller.
     * @param builder reference to builder instance.
     * @param pub reference to optional publisher of controller events.
     */
    MainController( const string name, Builder& builder, PublisherIntf *pub = nullptr )
        : ControllerIntf( name, pub ), builder( builder ) {}

    virtual ~MainController();

    /**
     * @brief Private static method that creates MainController singleton on first
     * invocation and returns reference to that instance on all subsequent
     * invocations. Invoked by Builder only (friend).
     * @param name of controller.
     * @param builder reference to builder instance.
     * @param pub reference to optional publisher of controller events.
     * @return reference to singleton MainController instance.
     */
    static MainController& getInstance( const string name, Builder& builder, PublisherIntf *pub = nullptr );

    /**
     * @brief transitionTo internal method to perform transitions of operational state.
     * @param to desired next state.
     * @return state that actually has been achieved.
     */
    OpSt transitionTo( OpSt to );


    /*
     * @brief Private member variables.
     */
    bool enableProbing = true;      // flag to disable probe cycle.
    OpSt opState = OpSt::Undef;     // operational state variable initialized with Undef.

    Builder& builder;
    static MainController *_this;   // private static pointer declaration
                                    // for singleton instance
};

#endif // MAINCONTROLLER_H
