#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "display.h"
#include "eventfactory.h"
#include "guifacade.h"
#include "builder.h"
#include "maincontroller.h"


/**
 * Public constructor.
 * @brief MainWindow::MainWindow
 * @param parent QWidget of main Window.
 */
MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ), ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    setWindowTitle( tr( "SE-2 Calculator" ) );
    QMainWindow::setFixedSize( 362, 510 );

    QFile file( ":/style.qss" );
    if( file.open( QFile::ReadOnly ) ) {
       QString styleSheet = QLatin1String( file.readAll() );
       this->setStyleSheet( styleSheet );
    }
}

/**
 * Public destructor.
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow() {
    logDestructor( "Ui::MainWindow" );
    delete ui;
}

/**
 * Entry point into allocation code.
 * @brief MainWindow::launch.
 */
void MainWindow::launch() {
    /*
     * 1. Collect display digits from ui widgets and pass to
     * Ui::Display instance.
     */
    Ui::Display::Digit digitBindings[] = {
        { ui->dig_0, ui->dot_0, ui->comma_0, ui->colon_0 },
        { ui->dig_1, ui->dot_1, ui->comma_1, ui->colon_1 },
        { ui->dig_2, ui->dot_2, ui->comma_2, ui->colon_2 },
        { ui->dig_3, ui->dot_3, ui->comma_3, ui->colon_3 },
        { ui->dig_4, ui->dot_4, ui->comma_4, ui->colon_4 },
        { ui->dig_5, ui->dot_5, ui->comma_5, ui->colon_5 },
        { ui->dig_6, ui->dot_6, ui->comma_6, ui->colon_6 },
        { ui->dig_7, ui->dot_7, ui->comma_7, ui->colon_7 },
        { ui->dig_8, ui->dot_8, ui->comma_8, ui->colon_8 },
        { ui->dig_9, ui->dot_9, ui->comma_9, ui->colon_9 },
    };
    Ui::Display *uiDisplay = new Ui::Display( digitBindings, 10 );

    /*
     * 2. Create Builder instance.
     */
    builder = &Builder::getInstance( *uiDisplay );

    /*
     * 3. Invoke Builder::build() to build and configure app components.
     */
    if( builder->build() ) {
        ControllerIntf *controller = builder->getMainController();
        /*
         * 4. Start application controller.
         */
        controller->start();
    }
}

/**
 * Invoked by Qt-dependent slot methods catching UI events to
 * transform events into application events from EventFactory.
 * @brief MainWindow::fireKeyEvent
 * @param ev event code from GuiFacade::KeyEvt passed as integer.
 */
void MainWindow::fireKeyEvent( int ev ) {
    if( builder != nullptr ) {
        XEventFactory& ef = builder->getEventFactory();
        GuiFacade *gui = builder->getGui();
        if( gui != nullptr ) {
            gui->publish( ef.getEvent( ev ) );
        }
    }
}

/**
 * Slot methods invoked by keypad-press events.
 * @brief MainWindow::on_key_XXX_pressed
 */
void MainWindow::on_key_0_pressed() { fireKeyEvent( GuiFacade::KeyEvt::K0 ); }
void MainWindow::on_key_1_pressed() { fireKeyEvent( GuiFacade::KeyEvt::K1 ); }
void MainWindow::on_key_2_pressed() { fireKeyEvent( GuiFacade::KeyEvt::K2 ); }
void MainWindow::on_key_3_pressed() { fireKeyEvent( GuiFacade::KeyEvt::K3 ); }
void MainWindow::on_key_4_pressed() { fireKeyEvent( GuiFacade::KeyEvt::K4 ); }
void MainWindow::on_key_5_pressed() { fireKeyEvent( GuiFacade::KeyEvt::K5 ); }
void MainWindow::on_key_6_pressed() { fireKeyEvent( GuiFacade::KeyEvt::K6 ); }
void MainWindow::on_key_7_pressed() { fireKeyEvent( GuiFacade::KeyEvt::K7 ); }
void MainWindow::on_key_8_pressed() { fireKeyEvent( GuiFacade::KeyEvt::K8 ); }
void MainWindow::on_key_9_pressed() { fireKeyEvent( GuiFacade::KeyEvt::K9 ); }
void MainWindow::on_key_000_pressed() { fireKeyEvent( GuiFacade::KeyEvt::K000 ); }
void MainWindow::on_key_Backspace_pressed() { fireKeyEvent( GuiFacade::KeyEvt::BS ); }
void MainWindow::on_key_C_pressed() { fireKeyEvent( GuiFacade::KeyEvt::C ); }
void MainWindow::on_key_CE_pressed() { fireKeyEvent( GuiFacade::KeyEvt::CE ); }
void MainWindow::on_key_Comma_pressed() { fireKeyEvent( GuiFacade::KeyEvt::Comma ); }
void MainWindow::on_key_Div_pressed() { fireKeyEvent( GuiFacade::KeyEvt::Div ); }
void MainWindow::on_key_EQ_pressed() { fireKeyEvent( GuiFacade::KeyEvt::EQ ); }
void MainWindow::on_key_Start_pressed() { fireKeyEvent( GuiFacade::KeyEvt::Start ); }
void MainWindow::on_key_Stop_pressed() { fireKeyEvent( GuiFacade::KeyEvt::Stop ); }
void MainWindow::on_key_Mode_pressed() { fireKeyEvent( GuiFacade::KeyEvt::Mode ); }
void MainWindow::on_key_Minus_pressed() { fireKeyEvent( GuiFacade::KeyEvt::Minus ); }
void MainWindow::on_key_Mul_pressed() { fireKeyEvent( GuiFacade::KeyEvt::Mul ); }
void MainWindow::on_key_ParClose_pressed() { fireKeyEvent( GuiFacade::KeyEvt::ParClose ); }
void MainWindow::on_key_ParOpen_pressed() { fireKeyEvent( GuiFacade::KeyEvt::ParOpen ); }
void MainWindow::on_key_Percent_pressed() { fireKeyEvent( GuiFacade::KeyEvt::Percent ); }
void MainWindow::on_key_Plus_pressed() { fireKeyEvent( GuiFacade::KeyEvt::Plus ); }
void MainWindow::on_key_VAT_pressed() { fireKeyEvent( GuiFacade::KeyEvt::VAT ); }
void MainWindow::on_key_OnOff_pressed() {
    MainController *mc = builder->getMainController();
    mc->isRunning()? mc->stop() : mc->start();
}

/**
 * Slot method invoked for key-press events.
 * @brief MainWindow::keyPressEvent
 * @param event Qt-Event.
 */
void MainWindow::keyPressEvent( QKeyEvent *event ) {
    switch( event->key() ) {
    case Qt::Key_0:     fireKeyEvent( GuiFacade::KeyEvt::K0 ); break;
    case Qt::Key_T:     fireKeyEvent( GuiFacade::KeyEvt::K000 ); break;
    case Qt::Key_1:     fireKeyEvent( GuiFacade::KeyEvt::K1 ); break;
    case Qt::Key_2:     fireKeyEvent( GuiFacade::KeyEvt::K2 ); break;
    case Qt::Key_3:     fireKeyEvent( GuiFacade::KeyEvt::K3 ); break;
    case Qt::Key_4:     fireKeyEvent( GuiFacade::KeyEvt::K4 ); break;
    case Qt::Key_5:     fireKeyEvent( GuiFacade::KeyEvt::K5 ); break;
    case Qt::Key_6:     fireKeyEvent( GuiFacade::KeyEvt::K6 ); break;
    case Qt::Key_7:     fireKeyEvent( GuiFacade::KeyEvt::K7 ); break;
    case Qt::Key_8:     fireKeyEvent( GuiFacade::KeyEvt::K8 ); break;
    case Qt::Key_9:     fireKeyEvent( GuiFacade::KeyEvt::K9 ); break;
    case Qt::Key_Backspace:
    case Qt::Key_Delete: fireKeyEvent( GuiFacade::KeyEvt::BS ); break;
    case Qt::Key_C:     fireKeyEvent( GuiFacade::KeyEvt::C ); break;
    case Qt::Key_Comma:
    case Qt::Key_Period: fireKeyEvent( GuiFacade::KeyEvt::Comma ); break;
    case Qt::Key_Slash: fireKeyEvent( GuiFacade::KeyEvt::Div ); break;
    case Qt::Key_Equal:
    case Qt::Key_Enter:
    case Qt::Key_Return: fireKeyEvent( GuiFacade::KeyEvt::EQ ); break;
    case Qt::Key_Minus: fireKeyEvent( GuiFacade::KeyEvt::Minus ); break;
    case Qt::Key_Asterisk: fireKeyEvent( GuiFacade::KeyEvt::Mul ); break;
    case Qt::Key_ParenRight: fireKeyEvent( GuiFacade::KeyEvt::ParClose ); break;
    case Qt::Key_ParenLeft: fireKeyEvent( GuiFacade::KeyEvt::ParOpen ); break;
    case Qt::Key_Percent: fireKeyEvent( GuiFacade::KeyEvt::Percent ); break;
    case Qt::Key_Plus:  fireKeyEvent( GuiFacade::KeyEvt::Plus ); break;
    case Qt::Key_M:     fireKeyEvent( GuiFacade::KeyEvt::Mode ); break;
    case Qt::Key_V:     fireKeyEvent( GuiFacade::KeyEvt::VAT ); break;

    case Qt::Key_X: close(); /* triggers QCloseEvent */ break;

    case Qt::Key_P: builder->getMainController()->probe(); break;
    }
}

/**
 * Slot method invoked when main window is closed or application is exited.
 * @brief MainWindow::closeEvent
 * @param event Qt-Event.
 */
void MainWindow::closeEvent( QCloseEvent *event ) {
    if( builder != nullptr ) {
        builder->getMainController()->stop( true );
    }
    event->accept();    //accept finally closes app, event.ignore() keeps it open
}

/**
 * Slot method to catch key-release events.
 * @brief keyReleaseEvent
 * @param event Qt-Event.
 * /
void keyReleaseEvent( QKeyEvent *event ) {
    if(event.key() == Qt::Key_Escape) {
        std::cout << "You released ESC" << std::endl;
    }
}
QCloseEvent event(QEvent::QClose, pos, 0, 0, 0);
QApplication::sendEvent(mainWindow, &event);
*/
