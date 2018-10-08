#include "mainwindow.h"
#include <QApplication>


/**
 * @brief Main entry point.
 * @param argc argument number.
 * @param argv argument vector.
 * @return  exit code.
 */
int main( int argc, char *argv[] ) {
    QApplication a( argc, argv );
    MainWindow w;
    w.show();
    w.launch();
    int rc = a.exec();
    return rc;
}
