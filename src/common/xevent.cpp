#include "xevent.h"


const string XEvent::defaultMsg = "";

const static string *keyInputEvtStr = nullptr;
const static string *timerEvtStr = nullptr;

/**
 * @brief Register event names (from enums) as string. Used for asString().
 * @param type event type set.
 * @param evStr array of event names from type set.
 */
void XEvent::regEvtStr( int type, const string *evStr ) {
    switch( type ) {
    case keyInputEvents: keyInputEvtStr = evStr; break;
    case timerEvents: timerEvtStr = evStr; break;
    }
}


/**
 * @brief returns XEvent as string.
 * @param e XEvent.
 * @return XEvent as string.
 */
string XEvent::asString( XEvent *e ) {
    string str = "";
    switch( e->type ) {
    case XEvent::Type::keyInputEvents:
        str = keyInputEvtStr? keyInputEvtStr[ e->ev ] : str;
        break;
    case XEvent::Type::timerEvents:
        str = timerEvtStr? timerEvtStr[ e->ev ] : str;
        break;
    case XEvent::Type::logEvents:
        str = e->msg;
        break;
    }
    return str;
}
