TARGET = prepare
BBN_DIR = ./src/binary_neurons_network
GPIO_DIR = ./src/gpio
LOGGER_DIR = ./src/logger

.PHONY: prepare all clean

all: $(TARGET)

clean:
	rm -rf $(BBN_DIR)
	mkdir $(BBN_DIR)
	rm -rf $(GPIO_DIR)
	mkdir $(GPIO_DIR)
	rm -rf $(LOGGER_DIR)
	mkdir $(LOGGER_DIR)

$(TARGET):
	git clone https://github.com/cortl0/binary_neurons_network.git $(BBN_DIR)
	git clone https://github.com/cortl0/gpio.git $(GPIO_DIR)
	git clone https://github.com/cortl0/logger.git $(LOGGER_DIR)
