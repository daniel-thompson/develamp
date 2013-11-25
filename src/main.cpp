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
#include "composite_dsp.h"

using namespace std;

class MapMeta : public Meta {
protected:
	map<string, string> meta;

public:

	virtual void declare(const char* key, const char* value) override {
		meta.emplace(key, value);
	}

	const string& getName() {
		// the if there is no name then it the empty string will be
		// created automatically by operator[]
		return meta["name"];
	}
};

static gint delete_event( GtkWidget *widget, GdkEvent *event, gpointer data )
{
    return FALSE;
}

static void destroy_event( GtkWidget *widget, gpointer data )
{
    gtk_main_quit ();
}

static gboolean do_update_all_guis(gpointer)
{
    GUI::updateAllGuis();
    return TRUE;
}

static void develamp_main(list<GTKUI*>& guiList)
{
	auto stack = gtk_stack_new();
	string name = "Name #1";
	for (auto gui : guiList) {
		auto panel = gui->getContainer();
		gtk_stack_add_titled(GTK_STACK(stack), panel, name.c_str(), name.c_str());
		*--end(name) += 1;
	}

	auto switcher = gtk_stack_switcher_new();
	gtk_orientable_set_orientation(GTK_ORIENTABLE(switcher), GTK_ORIENTATION_VERTICAL);
	gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher), GTK_STACK(stack));

	auto paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_paned_add1(GTK_PANED(paned), switcher);
	gtk_paned_add2(GTK_PANED(paned), stack);

	auto window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 6);
	gtk_window_set_title (GTK_WINDOW (window), "Research and Development Amplifier");
	g_signal_connect (window, "delete_event", G_CALLBACK(delete_event), NULL);
	g_signal_connect (window, "destroy", G_CALLBACK(destroy_event), NULL);
	gtk_container_add(GTK_CONTAINER(window), paned);
	gtk_widget_show_all(window);

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

	auto DSP = composite_dsp{dspList};
	//auto DSP = *begin(dspList);
	auto finterface = *begin(settingsList);
	jackaudio audio;
	audio.init(appname, &DSP);
	finterface->recallState(rcfilename);
	audio.start();

	develamp_main(guiList);

	audio.stop();
	finterface->saveState(rcfilename);
	return 0;
}
