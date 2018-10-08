#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include "controllerintf.h"
#include "builder.h"
using namespace std;


/**
 * @brief The DisplayController class defines the singleton instance of the
 * display controller providing Qt-independent access to the 10-digit display.
 * Methods of ControllerIntf are implemented, inclduing a probe cycle that
 * runs a series of test-pattern for calculator and timer modes.
 */
class DisplayController : public ControllerIntf {
    friend class Builder;

  public:
    /**
     * @brief probe is a method that asynchronously runs the display test cycle.
     * @param cb optional callback object at which the callback(XEvent *e)-
     * method is invoked when the probe-cycle has completed. Controller transitions
     * to Probing mode during execution of probe cycle.
     *
     * Decoupling execution of probe() from its invocation prevents main
     * thread from being blocked for the duration of the probe cycle.
     */
    virtual void probe( Callback *cb = nullptr );

    /**
     * @brief start and transition controller to RUNNING state.
     */
    virtual void start();

    /**
     * @brief stop and transition controller trough Stopping to Stopped state.
     * @param exit terminate programm if set to true.
     */
    virtual void stop( bool exit = false );

    /**
     * @brief updateDisplay pushes 10-digit buffer content to display.
     * Buffer may contain '.', ',' or ':' that will be displayed by turning
     * on repsective symbols on corresponding digits. Buffer length thus
     * can be >10 digits.
     * @param buffer string to display, e.g. "12.345" or "12:00:00"
     */
    void updateDisplay( string buffer );

    /**
     * @brief setError display "Error".
     */
    void setError();

    int static const len = 10;          // 10 digits in display


  private:
    /**
     * @brief Private constructor invoked by getInstance( ... ).
     * @param name of controller.
     * @param builder reference to builder instance.
     * @param pub reference to optional publisher of controller events.
     */
    DisplayController( const string name, Builder& builder, PublisherIntf *pub = nullptr )
        : ControllerIntf( name, pub ), builder( builder ), gui( builder.getGui() ) {}

    virtual ~DisplayController();

    /**
     * @brief Private static method that creates DisplayController singleton on first
     * invocation and returns reference to that instance on all subsequent
     * invocations. Invoked by Builder only (friend).
     * @param name of controller.
     * @param builder reference to builder instance.
     * @param pub reference to optional publisher of controller events.
     * @return reference to singleton DisplayController instance.
     */
    static DisplayController& getInstance( const string name, Builder& builder, PublisherIntf *pub = nullptr );


    /*
     * @brief Private member variables.
     */
    Builder& builder;
    GuiFacade* gui = nullptr;

    static DisplayController *_this;    // private static pointer declaration
                                        // for singleton instance
};

#endif // DISPLAYCONTROLLER_H
