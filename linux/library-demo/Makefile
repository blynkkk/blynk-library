# 
# To build on RaspberryPi:
# 1. Install WiringPi: http://wiringpi.com/download-and-install/
# 2. Build & install the blynk library system-wide
# 2. Run:
#    make

CC ?= gcc
CXX ?= g++

CXXFLAGS+= `pkg-config --cflags libblynk`  
LDFLAGS +=  `pkg-config --libs libblynk`

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif
FULLPREFIX=${DESTDIR}/${PREFIX}

CXXFLAGS += -O3 -w
LDFLAGS += -s
LDFLAGS += -lwiringPi

all: blynkTest

clean:
	-rm -f blynkTest

$(EXECUTABLE): blynkTest.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) blynkTest.cpp -o blynkTest

