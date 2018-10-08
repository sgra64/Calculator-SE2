#include "builder.h"
#include "maincontroller.h"
#include "guifacade.h"


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
GuiFacade& GuiFacade::getInstance( const string name, Builder& builder, const Ui::Display& uiDisplay, PublisherIntf& publisherImpl ) {
    if( _this == nullptr ) {
        _this = new GuiFacade( name, builder, uiDisplay, publisherImpl );
    }
    return *_this;
}

GuiFacade *GuiFacade::_this = nullptr;


GuiFacade::~GuiFacade() {
    logDestructor( PublisherIntf::getName() );
    delete &d;
}


/**
 * @brief GuiFacade::publish events. Only friend Ui::MainWindow is
 * permitted to publish events through GuiFacade.
 * @param e XEvent created by EventFactory.
 */
void GuiFacade::publish( XEvent& e ) {
    if( builder.getMainController()->isRunning() ) {
        pub.publish( e );
    }
}

/**
 * @brief GuiFacade::setAll
 * @param n
 * @param dot
 * @param comma
 * @param colon
 */
void GuiFacade::setAll( int n, bool dot, bool comma, bool colon ) {
    for( int i = 0; i < d.size; i++ ) {
        d.setDigit( i, n );
        d.setDot( i, dot );
        d.setComma( i, comma );
        d.setColon( i, colon );
    }
}
