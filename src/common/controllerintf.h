#ifndef CONTROLLERINTF_H
#define CONTROLLERINTF_H

#include "pubsub.h"
class Callback;


/**
 * @brief Abstract class that defines the controller interface.
 * A controller is a component that supports a number of control
 * operations such as probe, start, stop.
 *
 * A controller can also be a publisher for logging control operations.
 *
 */
class ControllerIntf : public PublisherIntf {

  public:
    /**
     * @brief Public constructor.
     * @param name of controller.
     * @param pub implementation of external log msg publisher.
     */
    ControllerIntf( const std::string name, PublisherIntf *pub )
        : PublisherIntf( name ), pub( pub ) {}
    virtual ~ControllerIntf();

    /**
     * @brief probe is a contol operation that probes a controller
     * before start. Since probing can be asynchronous, a callback
     * object is used to report completion.
     * @param cb callback to report completion of probing.
     */
    virtual void probe( Callback *cb = nullptr ) = 0;

    /**
     * @brief start is a control operation that starts a controller.
     */
    virtual void start() = 0;

    /**
     * @brief stop is a control operation that stopps a controller.
     * @param exit the main process if set to true.
     */
    virtual void stop( bool exit = false ) = 0;

    virtual const std::string getName() const { return PublisherIntf::getName(); }

    /**
     * @brief Methods inherited from PublisherIntf to publish controller
     * log messages, if a publisher instance is provided, and to manage
     * subscriptions.
     */
    virtual void publish( XEvent& e ) { if( pub ) pub->publish( e ); }
    virtual void subscribe( SubscriberIntf& s ) { if( pub ) pub->subscribe( s ); }
    virtual void unsubscribe( SubscriberIntf& s ) { if( pub ) pub->unsubscribe( s ); }
    virtual void clearSubscriptions() { if( pub ) pub->clearSubscriptions(); }

  protected:
    PublisherIntf *pub = nullptr;   // externally injected publisher instance.

};

#endif // CONTROLLERINTF_H
