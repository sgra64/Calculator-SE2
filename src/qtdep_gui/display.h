#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <QLCDNumber>
#include <QLabel>

namespace Ui {
  class Display;
}
class GuiFacade;
class MainWindow;
class Builder;


/**
 * @brief Ui::Display is a Qt-dependent class representing the
 * Qt-widgets for the display. It consists of 10 digits. Each digit
 * has a LCD-Number, a Dot- and a Comma-symbol at the lower right,
 * and Colon-symbols ':' between digits.
 *
 */
class Ui::Display {
    friend class ::GuiFacade;
    friend class ::MainWindow;
    friend class ::Builder;

  private:
    struct Digit {
        QLCDNumber *lcddigit;
        QLabel *dot;
        QLabel *comma;
        QLabel *colon;
    };

    /**
     * @brief Private constructor.
     * @param digits bindings to actual Qt-widgets used for digits
     * according to Digit struct.
     * @param size number of display digits.
     */
    Display( const Digit digits[], const int size );
    ~Display();

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
    void setDigit( int i, int lcdnum ) const;
    void setDot( int i, bool on ) const;
    void setComma( int i, bool on ) const;
    void setColon( int i, bool on ) const;
    void setMinus( int i ) const;
    void clearDigit( int i ) const;
    void clearAll() const;
    void setErr() const;

    Digit *digarray;
    const int size;

    void setQLCDNumber( int i, const QString &lcdvalue ) const;

};

#endif // DISPLAY_H
