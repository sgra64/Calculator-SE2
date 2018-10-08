#-------------------------------------------------
#
# Project created by QtCreator 2018-09-20T16:57:26
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Calculator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH += \
    src \
    src/common \
    src/components \
    src/qtdep_gui \
    src/logic

HEADERS += \
    src/common/controllerintf.h \
    src/common/pubsub.h \
    src/common/xevent.h \
    \
    src/components/builder.h \
    src/components/displaycontroller.h \
    src/components/eventfactory.h \
    src/components/inputprocessor.h \
    src/components/maincontroller.h \
    \
    \
    src/qtdep_gui/display.h \
    src/qtdep_gui/guifacade.h \
    src/qtdep_gui/intervaltimer.h \
    src/qtdep_gui/mainwindow.h \
    src/logic/calculator.h

SOURCES += \
    src/common/pubsub.cpp \
    src/common/xevent.cpp \
    \
    src/components/builder.cpp \
    src/components/displaycontroller.cpp \
    src/components/eventfactory.cpp \
    src/components/inputprocessor.cpp \
    src/components/maincontroller.cpp \
    \
    \
    src/qtdep_gui/display.cpp \
    src/qtdep_gui/guifacade.cpp \
    src/qtdep_gui/intervaltimer.cpp \
    src/qtdep_gui/mainwindow.cpp \
    \
    src/main.cpp \
    src/logic/calculator.cpp

FORMS += \
    src/qtdep_gui/mainwindow.ui \
    src/qtdep_gui/mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/res.qrc

