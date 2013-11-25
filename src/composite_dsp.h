/*
 * composite_dsp.h
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

#ifndef DEVELAMP_COMPOSITE_DSP_H_
#define DEVELAMP_COMPOSITE_DSP_H_

#include <list>

#include "dsp.h"

class composite_dsp : public dsp {
protected:
	std::list<dsp*> composite_list;

	std::vector<FAUSTFLOAT> inputs[2];
	std::vector<FAUSTFLOAT> outputs[2];

public:
	composite_dsp(const std::list<dsp*>& l);
	virtual ~composite_dsp() override;

	virtual int getNumInputs() override;
	virtual int getNumOutputs() override;
	virtual void buildUserInterface(UI* interface) override;
	virtual void init(int samplingRate) override;
 	virtual void compute(int len, FAUSTFLOAT** rawinputs, FAUSTFLOAT** rawoutputs) override;
};

#endif // DEVELAMP_COMPOSITE_DSP_H_
