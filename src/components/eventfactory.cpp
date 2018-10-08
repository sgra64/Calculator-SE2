#include "eventfactory.h"
#include "xevent.h"


/**
 * @brief getInstance creates singleton instance on first invocation.
 * @return reference to singleton EventFactory instance.
 */
XEventFactory& XEventFactory::getInstance() {
    if( _this == nullptr ) {
        _this = new XEventFactory();
    }
    return *_this;
}

XEventFactory *XEventFactory::_this = nullptr;


/**
 * @brief Public factory methods to create XEvent instances.
 * @param ev event as int from type set.
 * @param type event type set.
 * @param msg message.
 */
XEvent& XEventFactory::getEvent( const int ev ) {
    return *new XEvent( ev );
}

XEvent& XEventFactory::getEvent( const int type, const int ev ) {
    return *new XEvent( type, ev );
}

XEvent& XEventFactory::getEvent( const string msg  ) {
    return *new XEvent( msg );
}
