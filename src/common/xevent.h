#ifndef XEVENT_H
#define XEVENT_H

#include<iostream>
using namespace std;


/**
 * @brief The XEvent class defines event objects used by the app's
 * publish/subscribe mechanism. XEvents are issued by EventFactory.
 * XEvent contains:
 *      - XEvent::Type event type sets (as enums), which can be
 *              keyInputEvents, timerEvents, callbackEvents, logEvents,
 *      - event as int from type sets (enums),
 *      - msg as string.
 */
class XEvent {
    friend class XEventFactory;

  private:
    /**
     * @brief Private constructors used by friend EventFactory.
     * @param ev event as int from type set.
     * @param type event type set.
     * @param msg message.
     */
    XEvent( const int ev ) : type( Type::keyInputEvents ), ev( ev ) { }
    XEvent( const int type, const int ev ) : type( type ), ev( ev ) {}
    XEvent( const string msg  ) : type( Type::logEvents ), ev( 0 ), msg( msg ) {}

  public:
    enum Type {
        keyInputEvents, timerEvents, callbackEvents, logEvents
    };

    const int type;
    const int ev;
    const static string defaultMsg;
    const string msg = defaultMsg;

    /**
     * @brief returns XEvent as string.
     * @param e XEvent.
     * @return XEvent as string.
     */
    static string asString( XEvent *e );

    /**
     * @brief Register event names (from enums) as string. Used for asString().
     * @param type event type set.
     * @param evStr array of event names from type set.
     */
    static void regEvtStr( int type, const string* evStr );
};

#endif // XEVENT_H
