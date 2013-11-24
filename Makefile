#
# Makefile
#
# Part of develamp (the research and development amplifier)
#
# Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#

default : all

APP = develamp
OBJS = main.o gtk_knob.o dsp.o GUI.o GTKUI.o $(DSP_OBJS)
DSP_OBJS = bitshrieker.o reverb.o

OPTFLAGS = -g -O0
CC ?= gcc
CFLAGS = $(GTK_CFLAGS) -Wall -Werror $(OPTFLAGS)
CXX ?= g++
CXXFLAGS = $(GTK_CFLAGS) $(JACK_CFLAGS) -std=gnu++11 -Wall -Werror $(OPTFLAGS) $(PRIVATE_CFLAGS)
LD = $(CXX)
LDFLAGS = $(GTK_LIBS) $(JACK_LIBS) $(CXXFLAGS)

MIGRATION_CFLAGS ?= -DGTK_DISABLE_SINGLE_INCLUDES -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED -DGSEAL_ENABLE
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)
JACK_CFLAGS = $(shell pkg-config --cflags jack)
JACK_LIBS = $(shell pkg-config --libs jack)

all : $(APP)

clean :
	$(RM) $(OBJS) $(DSP_OBJS:.o=.cpp)

$(APP) : $(OBJS)
	$(LD) -o $(APP) $(OBJS) $(LDFLAGS)

%.cpp : %.dsp
	faust -a faustglue.cpp -cn $(<:.dsp=)dsp -o $@ $<
