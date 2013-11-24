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

#include "GTKUI.h"
#include "FUI.h"
#include "misc.h"
#include "jack-dsp.h"

using namespace std;

static gboolean do_update_all_guis(gpointer)
{
    GUI::updateAllGuis();
    return TRUE;
}

static void develamp_main(list<GTKUI*>& guiList)
{
	for (auto gui : guiList) {
		GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(window), "Temporary");

		GtkWidget* panel = gui->getContainer();
		gtk_container_add(GTK_CONTAINER(window), panel);
		gtk_widget_show(panel);
		gtk_widget_show(window);
	}

	g_timeout_add(40, do_update_all_guis, 0);
	gtk_main ();

	for (auto& gui : guiList) {
		gui->stop();
	}
}

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

	auto& dspList = dsp::getDspList();

	list<GTKUI*> guiList;
	list<FUI*> settingsList;

	for (auto& d : dspList) {
		auto gui = new GTKUI{appname, &argc, &argv};
		guiList.push_back(gui);
		d->buildUserInterface(gui);

		auto settings = new FUI{};
		settingsList.push_back(settings);
		d->buildUserInterface(settings);
	}

	auto DSP = *begin(dspList);
	auto finterface = *begin(settingsList);
	jackaudio audio;
	audio.init(appname, DSP);
	finterface->recallState(rcfilename);
	audio.start();

	develamp_main(guiList);

	audio.stop();
	finterface->saveState(rcfilename);
	return 0;
}
