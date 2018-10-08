#ifndef GUIFACADE_H
#define GUIFACADE_H

#include "display.h"
#include "pubsub.h"
#include "xevent.h"
using namespace std;


/**
 * @brief The GuiFacade class describes the interface (facade) between
 * the Qt-/GUI-dependent code and -independent application code.
 * GuiFacade exists as a singleton instance.
 *
 * GuiFacade offers access to underlying Qt-/GUI-widgets:
 *  - 10-digit Display and
 *  - key- or keypad events.
 *
 * GUI-Events are issued as enum GuiFacade::KeyEvt types using a
 * publish/subscribe mechanism to which event recipients can subscribe.
 *
 */
class GuiFacade : public PublisherIntf {
  friend class Builder;
  friend class MainWindow;

  public:
  /**
   * @brief Public access methods to 10-digit display. Digits are
   * indexed [0..9] by index parameter 'i'. Value is indicated by 'n'.
   * Each digit has a dot '.' and a comma ',' at the lower right that
   * can be turned on/off. Furthermore, there are colons ':' between
   * digits for clock mode.
   */
    void setDigit( int i, int n )   { d.setDigit( i, n ); }
    void setDot( int i, bool on )   { d.setDot( i, on ); }
    void setComma( int i, bool on ) { d.setComma( i, on ); }
    void setColon( int i, bool on ) { d.setColon( i, on ); }
    void setMinus( int i )          { d.setMinus( i ); }
    void setAll( int n, bool dot, bool comma, bool colon );
    void clearDigit( int i )        { d.clearDigit( i ); }
    void clearAll()                 { d.clearAll(); }
    void setErr()                   { d.setErr(); }

    /**
     * @brief The KeyEvt enum describes input event types issued by the GUI.
     * keyEvtStr has the string mappings of event names in same order.
     */
    enum KeyEvt {
        K0=0, K1=1, K2=2, K3=3, K4=4, K5=5, K6=6, K7=7, K8=8, K9=9, Comma=10, K000=11,
        Plus=12, Minus=13, Mul=14, Div=15, Percent=16, VAT=17, EQ=18,
        BS, C, CE, Mode, Start, Stop, ParOpen, ParClose
    };
    const string *keyEvtStr = new string[27] {
        "K0", "K1", "K2", "K3", "K4", "K5", "K6", "K7", "K8", "K9", "Comma", "K000",
        "Plus", "Minus", "Mul", "Div", "Percent", "VAT", "EQ",
        "BS", "C", "CE", "Mode", "Start", "Stop", "ParOpen", "ParClose"
    };

    /**
     * @brief Public methods of PublisherIntf that allow subcribing
     * to input events.
     */
    void subscribe( SubscriberIntf& s )     { pub.subscribe( s ); }
    void unsubscribe( SubscriberIntf& s )   { pub.unsubscribe( s ); }
    void clearSubscriptions()       { pub.clearSubscriptions(); }


  private:
    /**
     * @brief Private constructor invoked by GuiFacade::getInstance() only.
     * @param name of component.
     * @param builder associated builder.
     * @param uiDisplay reference to 10-digit Ui::Display.
     * @param publisherImpl Implementation of publisherIntf.
     */
    GuiFacade( const string name, Builder& builder, const Ui::Display& uiDisplay, PublisherIntf& publisherImpl )
        : PublisherIntf( name ), builder( builder ), d( uiDisplay ), pub( publisherImpl )
    {
        XEvent::regEvtStr( XEvent::Type::keyInputEvents, keyEvtStr );
    }

    ~GuiFacade();

    /**
     * @brief Private static method that creates GuiFacade singleton on first
     * invocation and returns reference to that instance on all subsequent
     * invocations. Used by Builder (friend).
     * @param name of component.
     * @param builder associated builder.
     * @param uiDisplay reference to 10-digit Ui::Display.
     * @param publisherImpl Implementation of publisherIntf.
     * @return reference to singleton GuiFacade instance.
     */
    static GuiFacade& getInstance( const string name, Builder& builder, const Ui::Display& uiDisplay, PublisherIntf& publisherImpl );

    /**
     * @brief Method to publish events. Only friend Ui::MainWindow is
     * permitted to publish events through GuiFacade.
     * @param e XEvent created by EventFactory.
     */
    void publish( XEvent& e );


    Builder& builder;
    const Ui::Display &d;
    PublisherIntf &pub;

    static GuiFacade *_this;    // private static pointer declaration
                                // for singleton instance
};

#endif // GUIFACADE_H
