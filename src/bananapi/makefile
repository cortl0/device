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
../binary_neurons_network/src/brain/brain.cpp \
../binary_neurons_network/src/brain/random_put_get.cpp \
../gpio/src/cpu.cpp \
../logger/src/logger.cpp \
./brain_friend.cpp \
./device.cpp \
./sensors/hc_sr04.cpp

#PREFIX = /usr/local/bin

.PHONY: all clean
#install uninstall

all: $(TARGET)

clean:
	rm $(TARGET)

$(TARGET): $(SRCS)
	g++ -std=c++17 -pthread -o $(TARGET) $(SRCS)

#install:
#	install $(TARGET) $(PREFIX)

#uninstall:
#	rm -rf $(PREFIX)/$(TARGET)

service_up: $(TARGET) ./bnn_device.service
	sudo mkdir $(PREFIX)
	sudo cp ./$(TARGET) $(PREFIX)/$(NAME)
	sudo cp ./bnn_device.service /etc/systemd/system/bnn_device.service
	sudo systemctl enable bnn_device.service
	sudo systemctl daemon-reload
	sudo systemctl start bnn_device.service

service_down:
	sudo systemctl stop bnn_device.service
	sudo systemctl disable bnn_device.service
	sudo rm /etc/systemd/system/bnn_device.service
	sudo rm -r $(PREFIX)
	sudo systemctl daemon-reload
