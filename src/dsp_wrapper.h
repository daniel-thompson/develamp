/*
 * dsp_wrapper.h
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

#ifndef DEVELAMP_DSP_WRAPPER_H_
#define DEVELAMP_DSP_WRAPPER_H_

#include <string>

#include "dsp.h"
#include "dsp_factory.h"
#include "GTKUI.h"
#include "FUI.h"


class dsp_wrapper {
private:
	dsp* signal_processor;
	std::string appname;
	dsp_factory* factory;
	GTKUI gui;
	FUI fui;

public:
	dsp_wrapper(char* name, int* pargc, char*** pargv, dsp_factory* f);
	virtual ~dsp_wrapper();

	dsp* get_dsp();
	GtkWidget* get_panel();

	void recall_state();
	void save_state();

	const std::string& get_meta(const std::string& key) {
		return factory->get_meta(key);
	}

	const std::string& get_name() {
		return factory->get_name();
	}

	int get_priority() {
		return factory->get_priority();
	}
};

#endif // DEVELAMP_DSP_WRAPPER_H_
