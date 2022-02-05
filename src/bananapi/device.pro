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

LIBS += \
    -lstdc++fs \
    -lpthread
#    -lboost_system

SOURCES += \
    main.cpp \
    brain_tools_dev.cpp \
    communication/communicators/communicator_base.cpp \
#    communication/communicators/serial_port_boost.cpp \
    communication/data_transfer_objects/dto_bpi.cpp \
    communication/data_transfer_objects/serializable.cpp \
    communication/converter.cpp \
    sensors/hc_sr04.cpp \
    device.cpp \
    ../bnn/src/brain/brain.cpp \
    ../bnn/src/brain_tools.cpp \
    ../bnn/src/brain/m_sequence.cpp \
    ../bnn/src/brain/neurons/binary.cpp \
    ../bnn/src/brain/neurons/motor.cpp \
    ../bnn/src/brain/neurons/neuron.cpp \
    ../bnn/src/brain/neurons/sensor.cpp \
    ../bnn/src/brain/random/random.cpp \
    ../bnn/src/brain/storage.cpp \
    ../bnn/src/brain/thread.cpp \
    ../gpio/src/cpu.cpp \
    ../logger/src/logger.cpp

HEADERS += \
    brain_tools_dev.h \
    communication/communicators/communicator_base.h \
#    communication/communicators/serial_port_boost.h \
    communication/data_transfer_objects/dto_bpi.h \
    communication/data_transfer_objects/serializable.h \
    communication/converter.h \
    sensors/hc_sr04.h \
    device.h \
    ../bnn/src/brain/brain.h \
    ../bnn/src/brain_tools.h \
    ../bnn/src/brain/m_sequence.h \
    ../bnn/src/brain/random/random.h \
    ../bnn/src/brain/storage.h \
    ../bnn/src/brain/thread.h \
    ../bnn/src/brain/simple_math.h \
    ../bnn/src/brain/config.h \
    ../gpio/src/cpu.h \
    ../logger/src/logger.h
