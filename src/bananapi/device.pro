#
#   device
#   created by Ilya Shishkin
#   cortl@8iter.ru
#   https://github.com/cortl0/device
#   licensed by GPL v3.0
#

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -pthread
LIBS += -lboost_system

SOURCES +=  main.cpp \
    communication/communicators/communicator_base.cpp \
    communication/communicators/serial_port_boost.cpp \
    communication/data_transfer_objects/dto_bpi.cpp \
    communication/data_transfer_objects/serializable.cpp \
    communication/converter.cpp

HEADERS += \
    communication/communicators/communicator_base.h \
    communication/communicators/serial_port_boost.h \
    communication/data_transfer_objects/dto_bpi.h \
    communication/data_transfer_objects/serializable.h \
    communication/converter.h

