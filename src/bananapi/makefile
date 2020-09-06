#
#   gpio
#   created by Ilya Shishkin
#   cortl@8iter.ru
#   https://github.com/cortl0/device
#   licensed by GPL v3.0
#

NAME = device
TARGET = $(NAME).out

#PREFIX = /usr/local/bin

.PHONY: all clean
#install uninstall

all: $(TARGET)

clean:
	rm $(TARGET)

$(TARGET): ../binary_neurons_network/src/brain/brain.cpp ../binary_neurons_network/src/brain/random_put_get.cpp ../gpio/src/cpu.cpp ./sensors/hc_sr04.cpp brain_friend.cpp device.cpp main.cpp
	g++ -std=c++17 -c -o random_put_get.o ../binary_neurons_network/src/brain/random_put_get.cpp
	g++ -std=c++17 -c -o binary_neurons_network.o ../binary_neurons_network/src/brain/brain.cpp
	g++ -std=c++17 -c -o cpu.o ../gpio/src/cpu.cpp
	g++ -std=c++17 -c -o brain_friend.o brain_friend.cpp
	g++ -std=c++17 -c -o device.o device.cpp
	g++ -std=c++17 -c -o hc_sr04.o ./sensors/hc_sr04.cpp
	g++ -std=c++17 -pthread -o device.out main.cpp device.o brain_friend.o binary_neurons_network.o random_put_get.o cpu.o hc_sr04.o

#install:
#	install $(TARGET) $(PREFIX)

#uninstall:
#	rm -rf $(PREFIX)/$(TARGET)

copy:
	sudo cp ./device.out /opt/bnn_device/device

set_service:
	sudo cp ./bnn_device.service /etc/systemd/system/bnn_device.service

start_service:
	sudo systemctl enable bnn_device.service
	sudo systemctl daemon-reload
	sudo systemctl restart bnn_device.service
