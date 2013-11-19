/*
 * main.cpp
 *
 * Part of develamp (the research and development amplifier)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
 * Copyright (C) 2003-2011 Thomas Charbonnel and GRAME
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#include <libgen.h>
#include <stdlib.h>
#include <iostream>

#include "dsp.h"

#include "GTKUI.h"
#include "FUI.h"
#include "misc.h"
#include "jack-dsp.h"

using namespace std;

/*!
 * Entry point.
 *
 * Most of this function was copied from a GNU GPLv3 architecture file.
 */
int main(int argc, char *argv[])
{
	char	appname[256];
	char  	rcfilename[256];
	char* 	home = getenv("HOME");

	snprintf(appname, 255, "%s", basename(argv[0]));
	snprintf(rcfilename, 255, "%s/.%src", home, appname);

	dsp* DSP = dsp::getDsp(0);
	if (DSP==0) {
		cerr << "Unable to allocate Faust DSP object" << endl;
		exit(1);
	}
	GUI* interface 	= new GTKUI (appname, &argc, &argv);
	FUI* finterface	= new FUI();
	DSP->buildUserInterface(interface);
	DSP->buildUserInterface(finterface);

	jackaudio audio;
	audio.init(appname, DSP);
	finterface->recallState(rcfilename);
	audio.start();

	interface->run();

	audio.stop();
	finterface->saveState(rcfilename);
	return 0;
}
