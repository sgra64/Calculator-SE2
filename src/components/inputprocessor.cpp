#include "maincontroller.h"
#include "inputprocessor.h"
#include "displaycontroller.h"
#include "eventfactory.h"
#include "calculator.h"
#include "guifacade.h"

void clearBuffer( string *buf, string init );
string to_string2( double d );


/**
 * @brief Private static method that creates InputProcessor singleton on first
 * invocation and returns reference to that instance on all subsequent
 * invocations. Invoked by Builder only (friend).
 * @param name name of the InputProcessor instance.
 * @param builder reference to builder instance.
 * @param alu reference to Calculator logic (algorithmic logical unit).
 * @param pub reference to optional publisher of controller events.
 * @return reference to singleton InputProcessor instance.
 */
InputProcessor& InputProcessor::getInstance( const string name, Builder& builder, Calculator& alu, PublisherIntf *pub ) {
    if( _this == nullptr ) {
        _this = new InputProcessor( name, builder, alu, pub );
    }
    return *_this;
}

InputProcessor *InputProcessor::_this = nullptr;

InputProcessor::~InputProcessor() {
    logDestructor( getName() );
}


/**
 * @brief InputProcessor::probe perform functional controller test.
 * @param cb optional callback object at which the callback(XEvent *e)-
 * method is invoked when the probe-cycle has completed.
 */
void InputProcessor::probe( Callback * /* cb */ ) {}

/**
 * @brief InputProcessor::start InputProcessor.
 */
void InputProcessor::start() {
    XEventFactory& ef = builder.getEventFactory();
    publish( ef.getEvent( getName() + " started." ) );
}

/**
 * @brief InputProcessor::stop InputProcessor.
 */
void InputProcessor::stop( bool /* exit */ ) {
    XEventFactory& ef = builder.getEventFactory();
    publish( ef.getEvent( getName() + " stopped." ) );
}


/**
 * @brief notify method inherited from SubscriberIntf is invoked when
 * an input event such as keypad- or keypress-event has occured.
 * notify() dispatches events to sub-methods notify_CalcMode and
 * notify_TimerMode depending on InputProcessor mode.
 * @param e input event.
 */
void InputProcessor::notify( XEvent& e ) {
    try {
        switch( e.ev ) {

        case GuiFacade::Mode:   // toggle mode
            switch( mode ) {
            case InputProcessor::CalculatorMode: mode = InputProcessor::TimerMode; break;
            case InputProcessor::TimerMode: mode = InputProcessor::CalculatorMode; break;
            } break;

        case GuiFacade::C:
        case GuiFacade::CE:
            err = false;
        }
        if( ! err ) {
            switch( mode ) {
            case InputProcessor::CalculatorMode: notify_CalcMode( e ); break;
            case InputProcessor::TimerMode: notify_TimerMode( e ); break;
            }
        }

    } catch( exception& e ) {
        err = true;
        cerr << e.what() << endl;
        builder.getDisplayController().setError();
    }
}


/**
 * @brief notify_CalcMode sub-method invoked by notify() in CalculatorMode.
 * @param e input event.
 */
void InputProcessor::notify_CalcMode( XEvent& e ) {
    bool hasDot = bufNumber.find( "." ) != string::npos;
    switch( e.ev ) {

    case GuiFacade::EQ:
    case GuiFacade::Minus:
    case GuiFacade::Plus:
    case GuiFacade::Mul:
    case GuiFacade::Div:
    case GuiFacade::Percent:
    case GuiFacade::VAT: {
            if( inpmode_ == INPUT_MODE::numbers ) {
                double d1 = stod( bufNumber );
                alu.push( d1 );
                alu.pushOp( e.ev );
            }
            alu.setOp( e.ev );

            double d = alu.top();
            if( d <= 9999999999.999999 && d >= -999999999.999999 ) {
                string d2str = to_string2( d );
                bufNumber = d2str;
            } else {
                cout << "OVERFLOW." << endl;
                throw overflow_error( "OVERFLOW." );
            }
            inpmode_ = INPUT_MODE::op;
        } break;

    case GuiFacade::ParOpen:
    case GuiFacade::ParClose:
        break;

    case GuiFacade::Comma:
        if( hasDot ) {
            break;
        }
        // case fall-through
    case GuiFacade::K0:
    case GuiFacade::K1:
    case GuiFacade::K2:
    case GuiFacade::K3:
    case GuiFacade::K4:
    case GuiFacade::K5:
    case GuiFacade::K6:
    case GuiFacade::K7:
    case GuiFacade::K8:
    case GuiFacade::K9: {
            static const char digits [] = {'0','1','2','3','4','5','6','7','8','9','.'};
            char dig = digits[ e.ev - GuiFacade::K0 ];

            if( inpmode_ == INPUT_MODE::op ) {
                clearBuffer( &bufNumber, "0" );
            }
            unsigned int maxdigits = DisplayController::len + ( hasDot? 1 : 0 );
            bool isleadingZero = bufNumber.length()==1 && bufNumber.at( 0 )=='0';
            if( ! isleadingZero || e.ev == GuiFacade::Comma ) {
                if( bufNumber.length() < maxdigits ) {
                    bufNumber.append( string( 1, dig ) );
                }
            } else {
                bufNumber[0] = dig;
            }
            inpmode_ = INPUT_MODE::numbers;

        } break;

    case GuiFacade::BS: {
            if( bufNumber.length() > 1 ) {
                bufNumber.pop_back();
            } else {
               clearBuffer( &bufNumber, "0" );
            }
        }  break;

    case GuiFacade::C:
            alu.clearAll();
            // case fall-through
    case GuiFacade::CE: {
            alu.clearTop();
            bufNumber = "0";
        } break;

    case GuiFacade::K000: {
            XEventFactory& ef = builder.getEventFactory();
            XEvent& xe0 = ef.getEvent( GuiFacade::KeyEvt::K0 );
            notify( xe0 );
            notify( xe0 );
            notify( xe0 );
        } break;
    }
    builder.getDisplayController().updateDisplay( bufNumber );
}


/**
 * @brief notify_CalcMode sub-method invoked by notify() in CalculatorMode.
 * @param e input event.
 */
void InputProcessor::notify_TimerMode( XEvent& e ) {
    switch( e.ev ) {

    case GuiFacade::Start:
        break;
    case GuiFacade::Stop:
        break;

    case GuiFacade::K0:
    case GuiFacade::K1:
    case GuiFacade::K2:
    case GuiFacade::K3:
    case GuiFacade::K4:
    case GuiFacade::K5:
    case GuiFacade::K6:
    case GuiFacade::K7:
    case GuiFacade::K8:
    case GuiFacade::K9: {
            static const char digits [] = {'0','1','2','3','4','5','6','7','8','9','.'};
            char dig = digits[ e.ev - GuiFacade::K0 ];

            if( dig >= '0' && dig <= '9' ) {
                bufTime[0] = bufTime[1];
                bufTime[1] = bufTime[3];
                bufTime[3] = bufTime[4];
                bufTime[4] = bufTime[6];
                bufTime[6] = bufTime[7];
                bufTime[7] = dig;
            }
        } break;

    case GuiFacade::BS:
    case GuiFacade::C:
            clearBuffer( &bufTime, "12:00:00" );
            break;
    case GuiFacade::CE:
            clearBuffer( &bufTime, "23:59:59" );
            break;
    }
    builder.getDisplayController().updateDisplay( bufTime );
}


/**
 * @brief clearBuffer clears the buffer passed as first argument and
 * initializes it with content passed as second argument.
 * @param buf reference to buffer.
 * @param init value set after buffer has been cleared,
 */
void clearBuffer( string *buf, string init ) {
    buf->clear();
    buf->append( init );
}


/**
 * @brief to_string2 converts a double to a string. The standard conversion
 * is not suitable since it can produce an exponential format such as "2.3e-09"
 * that cannot be displayed in the LCD-display. Furhermore, padding of leading
 * or trailing zeros such as 0009.0000 should be avoided.
 *
 * Source:
 *  https://codereview.stackexchange.com/questions/90565/converting-a-double-to-a-stdstring-without-scientific-notation
 *
 * Further recommended on numeric numbers conversion:
 *  https://thispointer.com/c-convert-double-to-string-and-manage-precision-scientific-notation
 *  https://stackoverflow.com/questions/1786497/sprintf-double-precision-in-c
 *
 * @param d number to be converted to string.
 * @return string representation of double passed as argument.
 */
string to_string2( double d ) {
    //ostringstream oss;
    //oss.precision( numeric_limits<double>::digits10 );
    //oss << fixed << d;
    //string str = oss.str();

    char str1[ 100 ];
    sprintf( str1, "%0.9f", d );    // using 9 digits.
    string str = str1;// = to_string( d );

    // Remove padding.
    // This must be done in two steps because of numbers like 700.00.
    size_t pos1 = str.find_last_not_of( "0" );
    if( pos1 != string::npos ) {
        str.erase( pos1 + 1 );
    }
    size_t pos2 = str.find_last_not_of( "." );
    if( pos2 != string::npos ) {
        str.erase( pos2 + 1 );
    }

    // Limit string to 10/11 characters (11 with dot).
    bool hasDot = str.find( "." ) != string::npos;
    string str2 = str.substr( 0, hasDot? 11 : 10 );

    return str2;
}
