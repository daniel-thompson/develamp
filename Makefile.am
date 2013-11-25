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
	src/reverb.dsp
src_develamp_CLEAN = \
	src/bitshrieker.cpp \
	src/reverb.cpp

src_develamp_SOURCES = \
	src/main.cpp \
	src/gtk_knob.cpp \
	src/dsp.cpp \
	src/composite_dsp.cpp \
	src/GUI.cpp \
	src/GTKUI.cpp \
	$(src_develamp_DSP)
src_develamp_CPPFLAGS = $(GTK_CFLAGS) $(JACK_CFLAGS)
src_develamp_LDADD = $(GTK_LIBS) $(JACK_LIBS)

CLEANFILES += $(src_develamp_CLEAN)

.dsp.cpp:
	@echo "  FAUST    $@"
	@faust -a $(top_srcdir)/src/faustglue.cpp -cn `basename $< .dsp` -o $@ $<

