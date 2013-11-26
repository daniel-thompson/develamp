/*
 * null-dsp.h
 *
 * Part of develamp (the research and development amplifier)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
 * Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef DEVELAMP_NULL_DSP_H_
#define DEVELAMP_NULL_DSP_H_

#include <stdio.h>
#include "audio.h"
#include "dsp.h"

static int		_srate(jack_nframes_t nframes, void *);
static int		_process (jack_nframes_t nframes, void *client);

/******************************************************************************
*******************************************************************************

							JACK AUDIO INTERFACE

*******************************************************************************
*******************************************************************************/

class nullaudio : public audio {
	dsp*			fDsp;
	int			fNumInChans;			// 	number of input channels
	int			fNumOutChans;			// 	number of output channels

 public:
	nullaudio() : fDsp(nullptr), fNumInChans(0), fNumOutChans(0) {}
	virtual ~nullaudio() { stop(); }

	virtual bool init(const char*name, dsp* DSP) {
		fDsp = DSP;
		fNumInChans  = fDsp->getNumInputs();
		fNumOutChans = fDsp->getNumOutputs();
		fDsp->init(44100);
		return true;
	}

	virtual bool start() {
		return true;
	}

	virtual void stop() {
	}
};

#endif // DEVELAMP_NULL_DSP_H_
