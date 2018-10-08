#include "displaycontroller.h"
#include "maincontroller.h"
#include "guifacade.h"
#include "intervaltimer.h"
#include "eventfactory.h"


/**
 * @brief Private static method that creates DisplayController singleton on first
 * invocation and returns reference to that instance on all subsequent
 * invocations. Invoked by Builder only (friend).
 * @param name of controller.
 * @param builder reference to builder instance.
 * @param pub reference to optional publisher of controller events.
 * @return reference to singleton DisplayController instance.
 */
DisplayController& DisplayController::getInstance( const string name, Builder& builder, PublisherIntf *pub ) {
    if( _this == nullptr ) {
        _this = new DisplayController( name, builder, pub );
    }
    return *_this;
}

DisplayController *DisplayController::_this = nullptr;


DisplayController::~DisplayController() {
    logDestructor( getName() );
}


/**
 * @brief updateDisplay pushes 10-digit buffer content to display.
 * Buffer may contain '.', ',' or ':' that will be displayed by turning
 * on repsective symbols on corresponding digits. Buffer length thus
 * can be >10 digits.
 * @param buffer string to display, e.g. "12.345" or "12:00:00"
 */
void DisplayController::updateDisplay( string buffer ) {
    //cout << "--> '" << buffer << "'\t";
    gui->clearAll();
    int ndots = count( buffer.begin(), buffer.end(), '.');
    int ncolons = count( buffer.begin(), buffer.end(), ':');
    gui->setDot( 0, ndots==0 && ncolons==0 );
    int lbuf = int( buffer.length() );
    int lmax = len + ndots + ncolons;
    int l = lbuf > lmax? lmax : lbuf;
    int j = 0;
    for( int i=0; i < l; i++ ) {
        char d = buffer.at( unsigned( l - i - 1 ) );
        switch( d ) {

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            int n = d - '0';
            gui->setDigit( j, n );
        } break;

        case '.':
            gui->setDot( j--, true );
            break;

        case ':':
            gui->setColon( j--, true );
            break;

        case '-':
            gui->setMinus( j );
            break;
        }
        j++;
    }
}


/**
 * @brief setError display "Error".
 */
void DisplayController::setError() {
    gui->setErr();
}


/**
 * @brief Private ProbeCycle class implements the probe cycle that is
 * comprised of sequence of intervall timer events each advancing the
 * cycle by one tick displaying the encoding of const int ticks[ len ].
 *
 * The probe cycle is divided into several modes probing and displaying
 * different patterns such as an advancing dot from left to right and back
 * or blinking numbers in the display.
 */
class ProbeCycle : public SubscriberIntf {

  public:
    ProbeCycle( Callback *cb ) : SubscriberIntf( "ProbeCycle" ), cb( cb ) {}

    void notify( XEvent& e );

    /*
     * @brief Private member variables.
     */
    int probeMode = 0;
    Callback *cb;
    static const int len = 82;
    const int ticks[ len ] = {
        // mode 0: show advancing/decling dots, each bit represents a dot
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0x1ff, 0x3ff,
        0x1ff, 0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x0f, 0x03, 0x01, 0x00,
        0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0x1ff, 0x3ff,
        0x1ff, 0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x0f, 0x03, 0x01, 0x00,

        // mode 1: show alternating "8.8.8.8.8.8.8.8.8.8." (1) and "12:00:00" (2)
        0x8001,
        1, 1, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0,
        2, 2, 2, 2, 0, 0,
        2, 2, 2, 2, 0, 0, 0, 0,

        // mode f: reset display to "0."
        0x800f, 0
    };
};


/**
 * @brief probe is a method that asynchronously runs the display test cycle.
 * @param cb optional callback object at which the callback(XEvent *e)-
 * method is invoked when the probe-cycle has completed. Controller transitions
 * to Probing mode during execution of probe cycle.
 *
 * Decoupling execution of probe() from its invocation prevents main
 * thread from being blocked for the duration of the probe cycle.
 */
void DisplayController::probe( Callback *cb ) {
    XEvent& xe0 = builder.getEventFactory().getEvent( getName() + " probing..." );
    publish( xe0 );
    gui->clearAll();
    ProbeCycle *c1 = new ProbeCycle( cb );
    IntervalTimer *t1 = new IntervalTimer( "Display, Cycle_1", c1->len, 20, c1 );
    t1->start();
}

/**
 * @brief start and transition controller to RUNNING state.
 */
void DisplayController::start() {
    gui->clearAll();
    gui->setDigit( 0, 0 );
    gui->setDot( 0, true );
    XEvent& xe0 = builder.getEventFactory().getEvent( getName() + " started." );
    publish( xe0 );
}

/**
 * @brief stop and transition controller trough Stopping to Stopped state.
 * @param exit terminate programm if set to true.
 */
void DisplayController::stop( bool /* exit */ ) {
    gui->clearAll();
    XEvent& xe0 = builder.getEventFactory().getEvent( getName() + " stopped." );
    publish( xe0 );
}


/**
 * @brief ProbeCycle::notify invoked at the end of a passed interval
 * advancing the probe cycle by 1 tick, decoding content from ticks[ tick ]
 * and loading it to the display.
 * @param e timer event.
 */
void ProbeCycle::notify( XEvent& e ) {
    GuiFacade *gui = Builder::getInstance().getGui();
    int i = e.ev;
    if( i < len ) {
        int t = ticks[ i ];
        if( t & 0x8000 ) {  // mode switch
            probeMode = t & 0xf;
        } else {
            switch( probeMode ) {
\
            case 0:
                for( int j=0; j < 10; j++ ) {
                    gui->setDot( j, t & 0x01 );
                    t = t >> 1;
                }
                break;

            case 1:
                switch( t ) {
                case 0:
                    gui->clearAll();
                    break;
                case 1:
                    // show: "8.8.8.8.8.8.8.8.8.8."
                    gui->setAll( 8, true, false, false );
                    break;
                case 2:
                    // show: "12:00:00"
                    gui->setDigit( 0, 0 );
                    gui->setDigit( 1, 0 );
                    gui->setDigit( 2, 0 );
                    gui->setDigit( 3, 0 );
                    gui->setDigit( 4, 2 );
                    gui->setDigit( 5, 1 );
                    gui->setColon( 2, true );
                    gui->setColon( 4, true );
                    break;
                }
                break;

            case 0xf:
                gui->clearAll();
                gui->setDigit( 0, 0 );
                gui->setDot( 0, true );
                if( cb != nullptr ) {
                    XEvent& xe0 = Builder::getInstance().getEventFactory()
                            .getEvent( XEvent::Type::callbackEvents, MainController::OpSt::RUNNING );
                    cb->callback( new XEvent( xe0 ) );
                }
            }
        }
    }
}
