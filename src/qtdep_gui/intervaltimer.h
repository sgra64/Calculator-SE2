#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <QObject>
#include <QTimer>
class SubscriberIntf;
using namespace std;


/**
 * @brief The IntervalTimer class is a Qt-dependent subclass, which
 * implements an interval timer that fires a limited number of timer
 * events with a specified delay between them.
 *
 */
class IntervalTimer : public QObject {
    Q_OBJECT    // Qt object (macro).

  public:
    /**
     * @brief IntervalTimer constructor.
     * @param name of the timer.
     * @param the number of events fired / or intervals.
     * @param msec duration between timer events.
     * @param subscriber to timer events.
     */
    IntervalTimer( string name, int limit, int msec, SubscriberIntf *subscriber = nullptr );
    ~IntervalTimer() {}

    /**
     * @brief start IntervalTimer.
     */
    void start() {
        if( limit > 0 ) {
            timer->start( msec );
        }
    }

    string getName() { return name; }

    /*
     * Slots are a Qt-concept to bind methods to event sources.
     * http://doc.qt.io/archives/qt-4.8/signalsandslots.html
     */
  public slots:
    /**
     * @brief intervalCallback is the method registered to the
     * Qt timer slot. intervalCallback-method is invoked for each
     * timer event.
     * ATTENTION: method is invoked by a different thread than the
     * thread that started the timer.
     */
    void intervalCallback();

  private:
    string name;

    QTimer *timer;
    int limit;
    int count;
    int msec;
    SubscriberIntf *subscriber;

};

#endif // TIMER_H
