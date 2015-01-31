#
# Makefile.am
#
# Part of develamp (the research and development amplifier)
#
# Copyright (C) 2012, 2013 Daniel Thompson <daniel@redfelineninja.org.uk> 
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#


ACLOCAL_AMFLAGS = -I m4

# __rt_data is found in code inherited from guitarix where it is
# used to mlock() all runtime data. Defining it to empty helps us
# parse the code.
DEVELAMP_CPPFLAGS = -D__rt_data= -I$(top_srcdir)/src -I$(top_srcdir)/src/guitarix

bin_PROGRAMS = \
	src/develamp

noinst_LIBRARIES =
noinst_PROGRAMS =
BUILT_SOURCES =
CLEANFILES =
TESTS =
TESTS_ENVIRONMENT=$(VALGRIND)

src_develamp_DSP = \
	src/bitshrieker.dsp \
	src/cleanamp.dsp \
	src/spksim.dsp \
	src/reverb.dsp \
	src/vumeter.dsp

src_develamp_CLEAN = \
	src/bitshrieker.cpp \
	src/cleanamp.cpp \
	src/spksim.cpp \
	src/reverb.cpp \
	src/vumeter.cpp

src_develamp_clean-local :
	$(RM) -r \
		src/bitshrieker-svg/ \
		src/cleanamp-svg/ \
		src/spksim-svg/ \
		src/reverb-svg/ \
		src/vumeter-svg/
.PHONY : src_develamp_clean-local

src_develamp_SOURCES = \
	src/main.cpp \
	src/gtk_knob.cpp \
	src/dsp_factory.cpp \
	src/dsp_wrapper.cpp \
	src/composite_dsp.cpp \
	src/GUI.cpp \
	src/GTKUI.cpp \
	$(src_develamp_DSP)
src_develamp_CPPFLAGS = $(GTK_CFLAGS) $(JACK_CFLAGS) $(DEVELAMP_CPPFLAGS)
src_develamp_LDADD = $(GTK_LIBS) $(JACK_LIBS)

CLEANFILES += $(src_develamp_CLEAN)
clean-local : src_develamp_clean-local

.dsp.cpp:
	@echo "  FAUST    $@"
	@faust -a $(top_srcdir)/src/faustglue.cpp -I $(top_srcdir)/src/guitarix -svg -cn `basename $< .dsp`dsp -o $@ $<

#
# Manual dependancies
#

FAUST_LIBFILES = \
	$(top_srcdir)/src/gui.lib \
	$(top_srcdir)/src/util.lib

$(src_develamp_DSP) : $(FAUST_LIBFILES) $(top_srcdir)/src/faustglue.cpp
