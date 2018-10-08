#ifndef EVENTFACTORY_H
#define EVENTFACTORY_H

#include<iostream>
using namespace std;
class XEvent;


/**
 * @brief The EventFactory class defines the EventFactory singleton
 * instance that exclusively creates XEvents.
 */
class XEventFactory {

  public:
    /**
     * @brief getInstance creates singleton instance on first invocation.
     * @return reference to singleton EventFactory instance.
     */
    static XEventFactory& getInstance();

    /**
     * @brief Public factory methods to create XEvent instances.
     * @param ev event as int from type set.
     * @param type event type set.
     * @param msg message.
     */
    XEvent& getEvent( const int ev );
    XEvent& getEvent( const int type, const int ev );
    XEvent& getEvent( const string msg  );

  private:
    /**
     * @brief Private constructor invoked by EventFactory::getInstance().
     */
    XEventFactory() {}

    static XEventFactory *_this;     // private static pointer declaration
                                    // for singleton instance
};

#endif // EVENTFACTORY_H
