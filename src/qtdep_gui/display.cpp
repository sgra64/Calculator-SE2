#include "display.h"

using namespace Ui;
void logDestructor( std::string msg );


/**
 * @brief Private constructor.
 * @param digits bindings to actual Qt-widgets used for digits
 * according to Digit struct.
 * @param size number of display digits.
 */
Display::Display( const Digit digits[], const int size ) : size( size ) {
    // copy pointer array from stack to heap for permanent use.
    digarray = new Digit[ size ];
    for( int i=0; i < size; i++ ) {
        this->digarray[i].lcddigit = digits[i].lcddigit;
        this->digarray[i].dot = digits[i].dot;
        this->digarray[i].comma = digits[i].comma;
        this->digarray[i].colon = digits[i].colon;
    }
}

Display::~Display() {
    logDestructor( "Ui::Display" );
}


/**
 * @brief Private access methods to 10-digit display. Methods
 * can only be used by friend GuiFacade.
 *
 * Digits are indexed [0..9] by index parameter 'i'. Value is
 * indicated by 'n'.
 * Each digit has a dot '.' and a comma ',' at the lower right that
 * can be turned on/off. Furthermore, there are colons ':' between
 * digits for clock mode.
 */
void Display::setDigit( int i, int lcdnum ) const {
    setQLCDNumber( i, lcdnum < 0? " " : QString::number( lcdnum % 10 ) );
}

void Display::setDot( int i, bool on ) const {
    QLabel *p = i >= 0 && i < size? digarray[i].dot : nullptr;
    if( p ) {
        QString s = on? "." : " ";
        p->setText( s );
    }
}

void Display::setComma( int i, bool on ) const {
    QLabel *p = i >= 0 && i < size? digarray[i].comma : nullptr;
    if( p ) {
        QString s = on? "," : " ";
        p->setText( s );
    }
}

void Display::setColon( int i, bool on ) const {
    QLabel *p = i >= 0 && i < size? digarray[i].colon : nullptr;
    if( p ) {
        QString s = on? ":" : " ";
        p->setText( s );
    }
}

void Display::setMinus( int i ) const {
    setQLCDNumber( i, "-" );
}

void Display::clearDigit( int i ) const {
    setQLCDNumber( i, " " );
    setDot( i, false );
    setComma( i, false );
    setColon( i, false );
}

void Display::clearAll() const {
    for( int i=0; i < size; i++ ) {
        clearDigit( i );
    }
}

void Display::setErr() const {
    clearAll();
    char msg[] = "Error";
    for( int i=0; i < 5 && i < size; i++ ) {
        setQLCDNumber( size - i - 1, QChar( msg[ i ] ) );
    }
}

inline void Display::setQLCDNumber( int i, const QString &lcdvalue ) const {
    QLCDNumber *p = i >= 0 && i < this->size? digarray[i].lcddigit : nullptr;
    if( p ) {
        p->display( lcdvalue );
    }
}
