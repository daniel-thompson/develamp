/*
 * composite_dsp.cpp
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

#include "composite_dsp.h"

#include <algorithm>
#include <array>
#include <exception>
#include <cstring>

#include <assert.h>

#include "dsp_wrapper.h"


using std::begin;
using std::end;

composite_dsp::composite_dsp(const std::list<std::unique_ptr<dsp_wrapper>>& l)
{
	for (auto& p : l)
		composite_list.push_back(p->get_dsp());
}

composite_dsp::~composite_dsp()
{
	// nothing to do
}

int composite_dsp::getNumInputs()
{
	return 2;
}

int composite_dsp::getNumOutputs()
{
	return 2;
}

void composite_dsp::buildUserInterface(UI* interface)
{
	throw std::bad_function_call{};
}

void composite_dsp::init(int samplingRate)
{
	for (auto p : composite_list)
		p->init(samplingRate);
}

void composite_dsp::compute(int len, FAUSTFLOAT** rawinputs, FAUSTFLOAT** rawoutputs)
{
	const auto bufsz = len * sizeof(FAUSTFLOAT);

	for (auto i=0; i<2; i++) {
		inputs[i].resize(len); // mem. mgt. will happen here (but only once is len is constant)
		memcpy(inputs[i].data(), rawinputs[i], bufsz);
		outputs[i].resize(len); // more mem. mgt.
	}

	auto current_width = 2;

	for (auto p : composite_list) {
		auto width = p->getNumInputs();
		assert(width <= 2);

		// mono to stereo by replicating the left channel
		if (width == 2 && current_width < 2)
			std::copy(begin(inputs[0]), end(inputs[0]), begin(inputs[1]));

		// stereo to mono by discarding the right channel is implicit

		// do the processing
		FAUSTFLOAT* in[] { inputs[0].data(), inputs[1].data() };
		FAUSTFLOAT* out[] { outputs[0].data(), outputs[1].data() };
		p->compute(len, in, out);

		current_width = p->getNumOutputs();
		assert(1 <= current_width && current_width <= 2);

		for (auto i=0; i<2; i++)
			std::swap(inputs[i], outputs[i]);
	}

	// mono to stereo by replicating the left channel
	if (current_width < 2)
		std::copy(begin(inputs[0]), end(inputs[0]), begin(inputs[1]));

	for (auto i=0; i<2; i++)
		memcpy(rawoutputs[i], inputs[i].data(), bufsz);
}
