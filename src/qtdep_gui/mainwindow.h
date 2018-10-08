#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLCDNumber>
#include <QLabel>
#include <QKeyEvent>

namespace Ui {
  class MainWindow;
}
class Builder;
class GuiFacade;


/**
 * @brief MainWindow is a Qt-dependent class representing the GUI main window.
 * Qt-runtime in main() invokes MainWindow::launch() that becomes the entry
 * point into the application code.
 * Slots provide hooks for handling GUI-events defined in Qt's GUI designer.
 * http://doc.qt.io/archives/qt-4.8/signalsandslots.html
 *
 * Slots are used here for keypad-, key-press and exit events.
 * Qt-dependent class MainWindow is hidden behind GuiFacade.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT    // Qt object (macro).

  public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

    /*
     * Main method "launched" by Qt-runtime in main().
     */
    void launch();

    /*
     * Slots are a Qt-concept to bind methods to event sources.
     * http://doc.qt.io/archives/qt-4.8/signalsandslots.html
     */
  private slots:
    void on_key_0_pressed();
    void on_key_000_pressed();
    void on_key_1_pressed();
    void on_key_2_pressed();
    void on_key_3_pressed();
    void on_key_4_pressed();
    void on_key_5_pressed();
    void on_key_6_pressed();
    void on_key_7_pressed();
    void on_key_8_pressed();
    void on_key_9_pressed();
    void on_key_Backspace_pressed();
    void on_key_C_pressed();
    void on_key_CE_pressed();
    void on_key_Comma_pressed();
    void on_key_Div_pressed();
    void on_key_EQ_pressed();
    void on_key_OnOff_pressed();
    void on_key_Start_pressed();
    void on_key_Stop_pressed();
    void on_key_Mode_pressed();
    void on_key_Minus_pressed();
    void on_key_Mul_pressed();
    void on_key_ParClose_pressed();
    void on_key_ParOpen_pressed();
    void on_key_Percent_pressed();
    void on_key_Plus_pressed();
    void on_key_VAT_pressed();

    void keyPressEvent( QKeyEvent *event );
    void closeEvent( QCloseEvent *event );

  private:
    void fireKeyEvent( int ev );
    Ui::MainWindow *ui;
    GuiFacade *guiFacade;
    Builder *builder;
};

#endif // MAINWINDOW_H
