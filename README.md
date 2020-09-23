# device
The practical implementation of a physical device with the binary neurons  
The device consists of two components:
arduino for controlling stepper motors,
bananapi for brain and sensors

## For project clone
```
git clone --recursive https://github.com/cortl0/device
```

## If not recursive cloned then project preparation
```
make prepare
```

## Project directories & files

#### ./src/arduino/
arduino sketch

#### ./src/bananapi/
bananapi project

#### ./src/binary_neurons_network/
Github repository
https://github.com/cortl0/binary_neurons_network

#### ./src/gpio/
Github repository
https://github.com/cortl0/gpio

#### ./src/logger/
Github repository
https://github.com/cortl0/logger

#### ./src/unit_test/
Unit test project

## ./device_schematic_diagram.drawio
Schematic diagram of the device in drawio file format

## Build
using a makefile:  
make  
make all  
make clean  
if available:  
make install  
make uninstall  
also if available QT build

## Author
Ilya Shishkin  
mailto:cortl@8iter.ru

## Links
http://8iter.ru/ai.html  
https://t.me/artificial_intelligence_bnn  
https://t.me/artificial_intelligence_bnn_grp  
https://github.com/cortl0/device

## License
This project is licensed under the GPL v3.0 - see the LICENSE file for details

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=DPXPWAL9BQD8Q)
