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

#include <cstdlib>

#include "GTKUI.h"
#include "FUI.h"
#include "misc.h"
#include "jack-dsp.h"
#include "composite_dsp.h"
#include "dsp_factory.h"
#include "dsp_wrapper.h"

using namespace std;

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

static void develamp_main(list<unique_ptr<dsp_wrapper>>& wrapperList)
{
	auto stack = gtk_stack_new();
	for (auto& w : wrapperList) {
		auto panel = w->get_panel();
		gtk_stack_add_titled(GTK_STACK(stack), panel, w->get_name().c_str(), w->get_name().c_str());
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

	do_update_all_guis(nullptr);
	g_timeout_add(40, do_update_all_guis, 0);
	gtk_main ();

#if 0
	for (auto& gui : guiList) {
		gui->stop();
	}
#endif
}

/*!
 * Entry point.
 *
 * Most of this function was copied from a GNU GPLv3 architecture file.
 */
int main(int argc, char *argv[])
{
	auto appname = std::string{ basename(argv[0]) };

	// sort the instances into priority order (the lambda can be made more
	// beautiful when C++14 rolls around).
	dsp_factory::registry.sort([](std::shared_ptr<dsp_factory>& x, std::shared_ptr<dsp_factory>& y) {
		return x->get_priority() < y->get_priority();
	});

	list<unique_ptr<dsp_wrapper>> wrapperList;
	for (auto& p : dsp_factory::registry)
		wrapperList.push_back(unique_ptr<dsp_wrapper>{new dsp_wrapper{ appname.c_str(), &argc, &argv, p }});

	auto DSP = composite_dsp{ wrapperList };
	jackaudio audio;
	audio.init(appname.c_str(), &DSP);
	audio.start();

	// the dsp classes are init'ed during audio.start() (which is the first
	// point we know the sampling frequency. However because this causes all
	// the values to be reset to their defaults we cannot recall the state until
	// after that happens.
	for (auto& p : wrapperList)
		p->recall_state();

	develamp_main(wrapperList);
	audio.stop();
	for (auto& p : wrapperList)
		p->save_state();

	return 0;
}
