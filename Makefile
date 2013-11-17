default : all

APP = develamp
OBJS = main.o bitshrieker.o reverb.o

CXX = g++
CXXFLAGS = $(GTK_CFLAGS) $(JACK_CFLAGS) -Wall -O3
LD = $(CXX)
LDFLAGS = $(GTK_LIBS) $(JACK_LIBS) $(CXXFLAGS)

GTK_CFLAGS = $(shell pkg-config --cflags gtk+-2.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-2.0)
JACK_CFLAGS = $(shell pkg-config --cflags jack)
JACK_LIBS = $(shell pkg-config --libs jack)

all : $(APP)

clean :
	$(RM) $(OBJS) bitshrieker.cpp reverb.cpp

$(APP) : $(OBJS)
	$(LD) -o $(APP) $(OBJS) $(LDFLAGS)

%.cpp : %.dsp
	faust -a faustglue.cpp -o $@ $<
