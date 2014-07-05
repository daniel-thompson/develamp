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

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

#include "GTKUI.h"
#include "FUI.h"
#include "misc.h"
#include "composite_dsp.h"
#include "jack-dsp.h"
#include "null-dsp.h"
#include "plot_audio.h"
#include "dsp_factory.h"
#include "dsp_wrapper.h"

using namespace std;

struct appctx {
    gboolean enable_null_audio;
    gboolean enable_plot_audio;
    string isolate;
};

static gint delete_event(GtkWidget* widget, GdkEvent* event, gpointer data)
{
    return FALSE;
}

static void destroy_event(GtkWidget* widget, gpointer data)
{
    gtk_main_quit();
}

static gboolean do_update_all_guis(gpointer unused)
{
    GUI::updateAllGuis();
    return TRUE;
}

/*!
 * Discover the geometry of the monitor that a window might appear on.
 *
 * This function uses a heuristic that will trivially work for single monitors
 * but relies on window manager behaviour for multi-monitor setups. Specifically
 * we assume that new windows will appear on the same desktop as the currently
 * active window.
 */
static void gtk_window_get_monitor_geometry(GtkWindow* window,
                                            GdkRectangle* dest)
{
    auto screen = gtk_window_get_screen(window);
    auto raw_window = gdk_screen_get_active_window(screen);
    auto monitor_id = gdk_screen_get_monitor_at_window(screen, raw_window);

    gdk_screen_get_monitor_geometry(screen, monitor_id, dest);
}

static void develamp_init(int *argc, char ***argv, appctx &ctx)
{
    gchar *isolate_cstr = nullptr;

    GOptionEntry entries[] = {
        {"null-audio", 'N', 0, G_OPTION_ARG_NONE,
	 &ctx.enable_null_audio, "Enable NULL audio output", nullptr},
        {"plot-audio", 'p', 0, G_OPTION_ARG_NONE,
	 &ctx.enable_plot_audio, "Enable NULL audio output", nullptr},
        {"isolate", 'i', 0, G_OPTION_ARG_STRING,
	 &isolate_cstr, "Isolate a single signal processor", nullptr},
        {nullptr}
    };

    auto res
        = gtk_init_with_args(argc, argv, nullptr, entries, nullptr, nullptr);

    if (!res) {
        cerr << "Cannot initialize GTK+ (DISPLAY not set?)" << endl;
	exit(1);
    }

    if (isolate_cstr)
        ctx.isolate = isolate_cstr;
    g_free(isolate_cstr);
}

static void develamp_main(list<unique_ptr<dsp_wrapper>>& wrapperList)
{
    auto stack = gtk_stack_new();

    for (auto& w : wrapperList) {
        auto panel = w->get_panel();
        auto scroller = gtk_scrolled_window_new(nullptr, nullptr);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroller),
                                       GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
        gtk_container_add(GTK_CONTAINER(scroller), panel);
        gtk_stack_add_titled(GTK_STACK(stack), scroller, w->get_name().c_str(),
                             w->get_name().c_str());
    }

    auto switcher = gtk_stack_switcher_new();
    gtk_orientable_set_orientation(GTK_ORIENTABLE(switcher),
                                   GTK_ORIENTATION_VERTICAL);
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher),
                                 GTK_STACK(stack));

    auto paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_paned_add1(GTK_PANED(paned), switcher);
    gtk_paned_add2(GTK_PANED(paned), stack);

    auto window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 6);
    gtk_window_set_title(GTK_WINDOW(window),
                         "Research and Development Amplifier");
    g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy_event), NULL);
    gtk_container_add(GTK_CONTAINER(window), paned);

    auto sz = GdkRectangle{};
    gtk_window_get_monitor_geometry(GTK_WINDOW(window), &sz);
    gtk_window_set_default_size(GTK_WINDOW(window), -1, sz.height - 100);

    gtk_widget_show_all(window);

    do_update_all_guis(nullptr);
    g_timeout_add(40, do_update_all_guis, 0);
    gtk_main();

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
    auto ctx = appctx{};
    develamp_init(&argc, &argv, ctx);

    // sort the instances into priority order (the lambda can be made more
    // beautiful when C++14 rolls around).
    dsp_factory::registry.sort([](shared_ptr<dsp_factory>& x,
                                  shared_ptr<dsp_factory>& y) {
        return x->get_priority() < y->get_priority();
    });

    auto appname = std::string{basename(argv[0])};

    list<unique_ptr<dsp_wrapper>> wrapperList;
    for (auto& p : dsp_factory::registry) {
        wrapperList.push_back(unique_ptr<dsp_wrapper>{
            new dsp_wrapper{appname.c_str(), p}});
    }

    if (ctx.isolate != "") {
        wrapperList.erase(remove_if(begin(wrapperList), end(wrapperList),
                                    [&ctx](const unique_ptr<dsp_wrapper>& w) {
                              return w->get_name() != ctx.isolate;
                          }),
                          end(wrapperList));
        if (wrapperList.size() == 0) {
	    cerr << "ERROR: Cannot isolate '" << ctx.isolate << "'" << endl;
	    return 2;
        }
    }

    auto DSP = composite_dsp{wrapperList};
    auto aud = unique_ptr<audio>{nullptr};
    if (ctx.enable_plot_audio) {
        aud.reset(new plot_audio{});
    } else if (ctx.enable_null_audio) {
        aud.reset(new nullaudio{});
    } else {
        aud.reset(new jackaudio{});
    }
    aud->init(appname.c_str(), &DSP);
    aud->start();

    // the dsp classes are init'ed during audio.start() (which is the first
    // point we know the sampling frequency. However because this causes all
    // the values to be reset to their defaults we cannot recall the state until
    // after that happens.
    for (auto& p : wrapperList)
        p->recall_state();

    develamp_main(wrapperList);
    aud->stop();

    for (auto& p : wrapperList)
        p->save_state();

    return 0;
}
