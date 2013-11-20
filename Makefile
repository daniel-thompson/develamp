default : all

APP = develamp
OBJS = main.o gtk_knob.o dsp.o GUI.o GTKUI.o $(DSP_OBJS)
DSP_OBJS = bitshrieker.o reverb.o

CXX ?= g++
CXXFLAGS = $(GTK_CFLAGS) $(JACK_CFLAGS) -std=gnu++11 -Wall -O3
LD = $(CXX)
LDFLAGS = $(GTK_LIBS) $(JACK_LIBS) $(CXXFLAGS)

GSEAL_CFLAGS ?= -DGSEAL_ENABLE
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-2.0) $(GSEAL_CFLAGS)
GTK_LIBS = $(shell pkg-config --libs gtk+-2.0)
JACK_CFLAGS = $(shell pkg-config --cflags jack)
JACK_LIBS = $(shell pkg-config --libs jack)

all : $(APP)

clean :
	$(RM) $(OBJS) $(DSP_OBJS:.o=.cpp)

$(APP) : $(OBJS)
	$(LD) -o $(APP) $(OBJS) $(LDFLAGS)

%.cpp : %.dsp
	faust -a faustglue.cpp -o $@ $<
