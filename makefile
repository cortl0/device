TARGET = prepare
BBN_DIR = ./src/binary_neurons_network
GPIO_DIR = ./src/gpio

.PHONY: prepare all clean

all: $(TARGET)

clean:
	rm -rf $(BBN_DIR)
	mkdir $(BBN_DIR)
	rm -rf $(GPIO_DIR)
	mkdir $(GPIO_DIR)

$(TARGET):
	git clone https://github.com/cortl0/binary_neurons_network.git $(BBN_DIR)
	git clone https://github.com/cortl0/gpio.git $(GPIO_DIR)
