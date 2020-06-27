#
#   device
#   created by Ilya Shishkin
#   cortl@8iter.ru
#   https://github.com/cortl0/device
#   licensed by GPL v3.0
#

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -pthread
LIBS += -lboost_system

HEADERS += \
    ../bananapi/communication/communicators/communicator_base.h \
    ../bananapi/communication/communicators/serial_port_boost.h \
    ../bananapi/communication/converter.h \
    ../bananapi/communication/data_transfer_objects/serializable.h \
    ../bananapi/communication/data_transfer_objects/dto_bpi.h \
    test_communication/test_communicators/test_serial_port_boost.hpp \
    test_communication/test_data_transfer_objects/test_dto_bpi.hpp \
    test_communication/test_dto_bpi_through_serial_port_boost.hpp

SOURCES +=  main.cpp \
    ../bananapi/communication/communicators/communicator_base.cpp \
    ../bananapi/communication/communicators/serial_port_boost.cpp \
    ../bananapi/communication/converter.cpp \
    ../bananapi/communication/data_transfer_objects/serializable.cpp \
    ../bananapi/communication/data_transfer_objects/dto_bpi.cpp
