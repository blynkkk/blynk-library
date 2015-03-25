# 
# To build on RaspberryPi:
# 1. Install WiringPi: http://wiringpi.com/download-and-install/
# 2. Run:
#    make target=raspberry
#    sudo blynk --auth=YourAuthToken
#

CC = g++
CFLAGS  = -I ../ -I ./ -DLINUX
LDFLAGS =

ifeq ($(build),debug)
	CFLAGS += -c -g -Wall
else
	CFLAGS += -c -O3 -w
	LDFLAGS += -s
endif

ifeq ($(target),raspberry)
	CFLAGS += -DRASPBERRY
	LDFLAGS += -lwiringPi -lpthread
endif

SOURCES=main.cpp \
	BlynkDebug.cpp \
	../BlynkParam.cpp \
	../BlynkHandlers.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=blynk

all: $(SOURCES) $(EXECUTABLE)

clean:
	-rm $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
