/*
 * dsp_wrapper.cpp
 *
 * Part of develamp (the research and development amplifier)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#include "dsp_wrapper.h"

#include <cctype>

dsp_wrapper::dsp_wrapper(
		char* name, int* pargc, char*** pargv, dsp_factory* f)
: signal_processor{nullptr}, appname{name}, factory{f}, gui{name, pargc, pargv}, fui{}
{
}

dsp_wrapper::~dsp_wrapper()
{
	if (nullptr != signal_processor)
		delete signal_processor;
}

dsp* dsp_wrapper::get_dsp()
{
	if (nullptr == signal_processor) {
		signal_processor = factory->create_dsp();
		signal_processor->buildUserInterface(&gui);
		signal_processor->buildUserInterface(&fui);
	}

	return signal_processor;
}

GtkWidget* dsp_wrapper::get_panel()
{
	(void) get_dsp(); // ensure dsp is instanciated
	return gui.getContainer();
}

static std::string generate_filename(
		const std::string& appname, std::string dspname)
{
	const char* home = getenv("HOME");
	if (NULL == home)
		home = ".";

	for (auto& c : dspname)
		if (!isalnum(c))
			c = '-';

	return std::string{home} + "/." + appname + "rc-" + dspname;
}

void dsp_wrapper::recall_state()
{
	std::string s {generate_filename(appname, get_name())};
	fui.recallState(s.c_str());
}

void dsp_wrapper::save_state()
{
	std::string s {generate_filename(appname, get_name())};
	fui.saveState(s.c_str());
}
