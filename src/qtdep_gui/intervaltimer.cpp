#include "intervaltimer.h"
#include "pubsub.h"
#include "eventfactory.h"
#include "xevent.h"


/**
 * @brief IntervalTimer::IntervalTimerconstructor.
 * @param name of the timer.
 * @param the number of events fired / or intervals.
 * @param msec duration between timer events.
 * @param subscriber to timer events.
 */
IntervalTimer::IntervalTimer( std::string name, int limit, int msec, SubscriberIntf *subscriber )
    : name( name ), limit( limit ), count( 0 ), msec( msec ), subscriber( subscriber )
{
    timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( intervalCallback() ) );
}

/**
 * @brief IntervalTimer::intervalCallback is the method registered
 * to the Qt timer slot. intervalCallback-method is invoked for
 * each timer event.
 * ATTENTION: method is invoked by a different thread than the
 * thread that started the timer.
 */
void IntervalTimer::intervalCallback() {
    if( --limit <= 0 ) {
        timer->stop();
    }
    if( subscriber != nullptr ) {
        XEventFactory& ef = XEventFactory::getInstance();
        XEvent& e = ef.getEvent( XEvent::Type::timerEvents, count++ );
        subscriber->notify( e );
    }
}
