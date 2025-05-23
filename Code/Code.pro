QT       += core gui 

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += charts

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bolus.cpp \
    createprofile.cpp \
    insulin.cpp \
    machine.cpp \
    main.cpp \
    mainwindow.cpp \
    options.cpp \
    profile.cpp \
    simulation.cpp

HEADERS += \
    bolus.h \
    createprofile.h \
    insulin.h \
    machine.h \
    mainwindow.h \
    options.h \
    profile.h \
    simulation.h \
    defs.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
