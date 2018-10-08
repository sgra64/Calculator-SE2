#include <algorithm>
#include "pubsub.h"


Callback::~Callback() {}

SubscriberIntf::~SubscriberIntf() {}

PublisherIntf::~PublisherIntf() {}

/**
 * @brief logDestructor is a helper method to log destructor invocations
 * when other system components have already been shut down.
 * @param msg log message.
 */
void logDestructor( string msg ) {
    cout << msg << " destructor." << endl;
}


/**
 * @brief SimplePublisherImpl::publish an event by invoking notify(e)
 * on all subscribers.
 * @param e XEvent to publish.
 */
void SimplePublisherImpl::publish( XEvent& e ) {
    vector<SubscriberIntf *>::iterator it;
    for( it = subscribers.begin(); it != subscribers.end(); it++ ) {
        (*it)->notify( e );
    }
}

/**
 * @brief SimplePublisherImpl::subscribe  subscribe a new subscriber,
 * if not yet subscribed.
 * @param s subscriber.
 */
void SimplePublisherImpl::subscribe( SubscriberIntf& s ) {
    vector<SubscriberIntf *>::iterator it;
    it = find( subscribers.begin(), subscribers.end(), &s );
    if( it == subscribers.end() ) {
        cout << s.getName() << " subscribed to " << this->getName() << "." << endl;
        subscribers.push_back( &s );
    }
}

/**
 * @brief SimplePublisherImpl::unsubscribe a subscriber by removing from
 * subscribers vector.
 * @param s subscriber.
 */
void SimplePublisherImpl::unsubscribe( SubscriberIntf& s ) {
    vector<SubscriberIntf*>::iterator it;
    it = find( subscribers.begin(), subscribers.end(), &s );
    if( it != subscribers.end() ) {
        string name = (*it)->getName();
        subscribers.erase( it );
        cout << name << " unsubscribed from " << this->getName() << "."  << endl;
    }
}

SimplePublisherImpl::~SimplePublisherImpl() {
    logDestructor( PublisherIntf::getName() );
}


SimpleLogger::~SimpleLogger() {
    logDestructor( getName() );
}
