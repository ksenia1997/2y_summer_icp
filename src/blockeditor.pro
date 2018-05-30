TEMPLATE = app
TARGET = blockeditor

QT += core gui
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    scene.cpp \
    block.cpp \
    blockconnection.cpp \
    port.cpp \
    line.cpp

HEADERS  += \
    mainwindow.h \
    scene.h \
    block.h \
    blockconnection.h \
    port.h \
    line.h

RESOURCES += \
    blockeditor.qrc
