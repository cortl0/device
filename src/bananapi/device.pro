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

LIBS += -lstdc++fs
LIBS += -pthread
#LIBS += -lboost_system

SOURCES += main.cpp \
    brain_friend_dev.cpp \
    communication/communicators/communicator_base.cpp \
#    communication/communicators/serial_port_boost.cpp \
    communication/data_transfer_objects/dto_bpi.cpp \
    communication/data_transfer_objects/serializable.cpp \
    communication/converter.cpp \
    sensors/hc_sr04.cpp \
    device.cpp \
    ../binary_neurons_network/src/brain/brain.cpp \
    ../binary_neurons_network/src/brain_friend.cpp \
    ../binary_neurons_network/src/brain/m_sequence.cpp \
    ../binary_neurons_network/src/brain/neurons/binary.cpp \
    ../binary_neurons_network/src/brain/neurons/motor.cpp \
    ../binary_neurons_network/src/brain/neurons/neuron.cpp \
    ../binary_neurons_network/src/brain/neurons/sensor.cpp \
    ../binary_neurons_network/src/brain/random_put_get.cpp \
    ../gpio/src/cpu.cpp \
    ../logger/src/logger.cpp

HEADERS += \
    brain_friend_dev.h \
    communication/communicators/communicator_base.h \
#    communication/communicators/serial_port_boost.h \
    communication/data_transfer_objects/dto_bpi.h \
    communication/data_transfer_objects/serializable.h \
    communication/converter.h \
    sensors/hc_sr04.h \
    device.h \
    ../binary_neurons_network/src/brain/brain.h \
    ../binary_neurons_network/src/brain_friend.h \
    ../binary_neurons_network/src/brain/m_sequence.h \
    ../binary_neurons_network/src/brain/random_put_get.h \
    ../binary_neurons_network/src/brain/simple_math.h \
    ../binary_neurons_network/src/brain/config.h \
    ../gpio/src/cpu.h \
    ../logger/src/logger.h

