#
#   gpio
#   created by Ilya Shishkin
#   cortl@8iter.ru
#   https://github.com/cortl0/device
#   licensed by GPL v3.0
#

NAME = device

TARGET = $(NAME).out

PREFIX = /opt/bnn_device

SRCS = \
main.cpp \
../bnn/src/brain/brain.cpp \
../bnn/src/brain_tools.cpp \
../bnn/src/brain/m_sequence.cpp \
../bnn/src/brain/neurons/binary.cpp \
../bnn/src/brain/neurons/neuron.cpp \
../bnn/src/brain/neurons/motor.cpp \
../bnn/src/brain/neurons/sensor.cpp \
../bnn/src/brain/random/random.cpp \
../bnn/src/brain/storage.cpp \
../bnn/src/brain/thread.cpp \
../gpio/src/cpu.cpp \
../logger/src/logger.cpp \
./brain_tools_dev.cpp \
./device.cpp \
./sensors/hc_sr04.cpp

.PHONY: all clean service_up service_down

all: $(TARGET)

clean:
	rm $(TARGET)

$(TARGET): $(SRCS)
	g++ -o $(TARGET) $(SRCS) -std=c++17 -pthread -lstdc++fs

service_up: $(TARGET) ./bnn_device.service
	sudo mkdir $(PREFIX)
	sudo cp ./$(TARGET) $(PREFIX)/$(NAME)
	sudo cp ./bnn_device.service /etc/systemd/system/bnn_device.service
	sudo systemctl enable bnn_device.service
	sudo systemctl daemon-reload
	sudo systemctl start bnn_device.service

service_down: /etc/systemd/system/bnn_device.service
	sudo systemctl stop bnn_device.service
	sudo systemctl disable bnn_device.service
	sudo rm /etc/systemd/system/bnn_device.service
	sudo rm -r $(PREFIX)
	sudo systemctl daemon-reload
