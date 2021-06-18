QT += core widgets

CONFIG += no_keywords link_pkgconfig

PKGCONFIG += gio-unix-2.0 udisks2

SOURCES += \
    $$PWD/src/main.cpp                      \
    $$PWD/src/device-manager.cpp            \
    $$PWD/src/device-operation.cpp          \


HEADERS += \
    $$PWD/src/device-manager.h              \
    $$PWD/src/device-operation.h

