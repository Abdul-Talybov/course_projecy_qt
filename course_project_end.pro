QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ADCRingL.cpp \
    HashTable.cpp \
    Knot.cpp \
    debugwindow.cpp \
    findwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    startwindow.cpp

HEADERS += \
    ADCRingL.h \
    Date.h \
    Direction_code.h \
    HashNode.h \
    Key.h \
    Knot.h \
    auxiliary_functions.h \
    debugwindow.h \
    findwindow.h \
    mainwindow.h \
    startwindow.h

FORMS += \
    debugwindow.ui \
    findwindow.ui \
    mainwindow.ui \
    startwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    AVL_TREE_VIRT.vcxproj \
    AVL_TREE_VIRT.vcxproj.filters
