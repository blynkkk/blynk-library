# 
# To build on RaspberryPi:
# 1. Install WiringPi: http://wiringpi.com/download-and-install/
# 2. Run:
#    make target=raspberry
#    sudo blynk --token=YourAuthToken
#

CC ?= gcc
CXX ?= g++
CXXFLAGS += -I ../src/ -I ./ -DLINUX
LDFLAGS += -lrt -lpthread

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif
FULLPREFIX=${DESTDIR}/${PREFIX}

ifeq ($(build),debug)
	CXXFLAGS += -c -g2 -O0 \
		-Wall -Weffc++ \
		-Wextra -Wcast-align \
		-Wchar-subscripts  -Wcomment -Wconversion \
		-Wdisabled-optimization \
		-Wfloat-equal  -Wformat  -Wformat=2 \
		-Wformat-nonliteral -Wformat-security  \
		-Wformat-y2k \
		-Wimplicit  -Wimport  -Winit-self  -Winline \
		-Winvalid-pch   \
		-Wunsafe-loop-optimizations  -Wlong-long -Wmissing-braces \
		-Wmissing-field-initializers -Wmissing-format-attribute   \
		-Wmissing-include-dirs -Wmissing-noreturn \
		-Wpacked  -Wparentheses  -Wpointer-arith \
		-Wredundant-decls -Wreturn-type \
		-Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector \
		-Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch  -Wswitch-default \
		-Wswitch-enum -Wtrigraphs  -Wuninitialized \
		-Wunknown-pragmas  -Wunreachable-code -Wunused \
		-Wunused-function  -Wunused-label  \
		-Wunused-value  -Wunused-variable \
		-Wvolatile-register-var  -Wwrite-strings

# Disable some warnings
	CXXFLAGS += -Wno-variadic-macros -Wno-unused-parameter -Wno-vla

# Uncomment this to get pedantic warnings:
#CXXFLAGS += -pedantic -Wvariadic-macros -Wunused-parameter -Waggregate-return -Wcast-qual -Wpadded
else
	CXXFLAGS += -c -O3 -w
	LDFLAGS += -s
endif

ifeq ($(target),raspberry)
	CXXFLAGS += -DRASPBERRY
	LDFLAGS += -lwiringPi
endif

LIBSOURCES=	../src/utility/BlynkDebug.cpp \
            ../src/utility/BlynkHandlers.cpp \
            ../src/utility/BlynkTimer.cpp

USRHEADERS= *.h
COREHEADERS=  ../src/Blynk/*.h
UTILHEADERS= ../src/utility/*.h

ALLSOURCES=main.cpp $(LIBSOURCES)

ALLOBJECTS=$(ALLSOURCES:.cpp=.o)
EXECUTABLE=blynk-demo

LIBOBJECTS=$(LIBSOURCES:.cpp=.o)
LIBRARY=libblynk.so

PKGCONFIG=libblynk.pc

all: $(ALLSOURCES) $(EXECUTABLE) $(LIBRARY)

library: $(LIBRARY) $(PKGCONFIG)

clean:
	-rm -f $(ALLOBJECTS) $(EXECUTABLE) $(LIBRARY) $(PKGCONFIG)

$(EXECUTABLE): $(ALLOBJECTS) $(LIBRARY)
	$(CXX) $(ALLOBJECTS) $(LDFLAGS) -o $@

$(LIBRARY): $(LIBOBJECTS)
	$(CXX) $(LIBOBJECTS) $(LDFLAGS) -shared -o $(LIBRARY)

install: $(LIBRARY) $(PKGCONFIG)
	install -d $(FULLPREFIX)/lib/libblynk/
	install -m 755 $(LIBRARY) $(FULLPREFIX)/lib/libblynk/
	install -d $(FULLPREFIX)/include/libblynk/
	install -m 644 ${USRHEADERS} $(FULLPREFIX)/include/libblynk/
	install -d $(FULLPREFIX)/include/libblynk/Blynk/
	install -m 644 ${COREHEADERS} $(FULLPREFIX)/include/libblynk/Blynk/
	install -d $(FULLPREFIX)/include/libblynk/utility/
	install -m 644 ${UTILHEADERS} $(FULLPREFIX)/include/libblynk/utility/
	install -d $(FULLPREFIX)/lib/pkgconfig/
	install -m 644 ${PKGCONFIG} $(FULLPREFIX)/lib/pkgconfig/
	echo "$(FULLPREFIX)/lib/libblynk" >/etc/ld.so.conf.d/libblynk.conf
	ldconfig ${FULLPREFIX}

uninstall:
	rm -rf ${FULLPREFIX}/lib/libblynk/
	rm -rf ${FULLPREFIX}/include/libblynk/
	rm -f ${FULLPREFIX}/lib/pkgconfig/${PKGCONFIG}
	rm -f /etc/ld.so.conf.d/libblynk.conf
	ldconfig ${FULLPREFIX}

.cpp.o:
	$(CXX) $(CXXFLAGS) -fPIC $< -o $@

$(PKGCONFIG):
	echo "Name: Blynk Library"  >>$(PKGCONFIG)
	echo "Description: C Utility Library"  >>$(PKGCONFIG)
	echo "Version: 0.0" >>$(PKGCONFIG)
	echo "Cflags: -I${FULLPREFIX}/include/libblynk/"  >>$(PKGCONFIG)
	echo "Libs: -L${FULLPREFIX}/lib/libblynk -lblynk"  >>$(PKGCONFIG)
