#ifndef PUBSUB_H
#define PUBSUB_H

#include <iostream>
#include <vector>
#include "xevent.h"
using namespace std;
class XEvent;


/**
 * @brief The Callback class is used to implement callback-objects
 * for XEvents.
 */
class Callback {
  public:
    virtual ~Callback();
    virtual void callback( XEvent *e ) = 0;
};


/**
 * @brief Abstract class that defines the subscriber interface.
 */
class SubscriberIntf {

  public:
    SubscriberIntf( const string name ) : name( name ) {}
    virtual ~SubscriberIntf();

    /**
     * @brief notify is a method invoked at a subscriber when an event
     * is published.
     * @param e XEvent published.
     */
    virtual void notify( XEvent& e ) = 0;

    /**
     * @brief getName returns the name of the subscriber.
     * @return name of the subscriber.
     */
    const virtual string getName() const { return name; }

  private:
    const string name;

};


/**
 * @brief Abstract class that defines the publisher interface.
 */
class PublisherIntf {

  public:
    PublisherIntf( const string name ) : name( name ) {}
    virtual ~PublisherIntf();

    /**
     * @brief publish an event by invoking notify(e) on all subscribers.
     * @param e XEvent to publish.
     */
    virtual void publish( XEvent& e ) = 0;

    /**
     * @brief subscribe a new subscriber, if not yet subscribed.
     * unsubscribe( s ) and clearSubscriptions() to remove individual
     * or all subscribers.
     * @param s subscriber.
     */
    virtual void subscribe( SubscriberIntf& s ) = 0;
    virtual void unsubscribe( SubscriberIntf& s ) = 0;
    virtual void clearSubscriptions() = 0;

    /**
     * @brief getName returns the name of the publisher.
     * @return name of the publisher.
     */
    virtual const string getName() const { return name; }

  private:
    const string name;

};


/**
 * @brief SimplePublisherImpl is a simple publisher implementation.
 */
class SimplePublisherImpl : public PublisherIntf {

  public:
    SimplePublisherImpl( const string name ) : PublisherIntf( name ) {}
    virtual ~SimplePublisherImpl();

    /**
     * @brief subscribe a new subscriber, if not yet subscribed.
     * unsubscribe( s ) and clearSubscriptions() to remove individual
     * or all subscribers.
     * @param s subscriber.
     */
    virtual void publish( XEvent& e );
    virtual void subscribe( SubscriberIntf& s );
    virtual void unsubscribe( SubscriberIntf& s );
    virtual void clearSubscriptions() { subscribers.clear(); }

  private:
    vector<SubscriberIntf *> subscribers = {};

};


/**
 * @brief SimpleLogger implements a simple console logger using a
 * SubscriberIntf. Log events published to a SimpleLogger instance
 * as string messages are simply logged to std::cout.
 */
class SimpleLogger : public SubscriberIntf {

  public:
    SimpleLogger( string name ) : SubscriberIntf( name ) {}
    ~SimpleLogger();

    /**
     * @brief notify is invoked when log message is published.
     * @param e published XEvent with string message.
     */
    void notify( XEvent& e ) {
        cout << XEvent::asString( &e ) << endl;
    }

};

#endif // PUBSUB_H
