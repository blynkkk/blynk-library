
#for raspberry:
#CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s

CC=g++
CFLAGS=-I ../ -I ./ -DLINUX
CFLAGS+=-c -Wall -O3
LDFLAGS=-s
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
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
