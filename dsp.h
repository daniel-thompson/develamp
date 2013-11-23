/*
 * dsp.h
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

//
// Original license (source came from GNU GPLv3 package and had no license
// header text, however it was included by the module.cpp architecture file
// which uses this license).
//
/************************************************************************
 ************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------

	This is sample code. This file is provided as an example of minimal
	FAUST architecture file. Redistribution and use in source and binary
	forms, with or without modification, in part or in full are permitted.
	In particular you can create a derived work of this FAUST architecture
	and distribute that work under terms of your choice.

	This sample code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 ************************************************************************
 ************************************************************************/

#ifndef DEVELAMP_DSP_H_
#define DEVELAMP_DSP_H_

#include <algorithm>
#include <iterator>
#include <vector>

class UI;

//----------------------------------------------------------------
//  signal processor definition
//----------------------------------------------------------------

class dsp {
 protected:
	static std::vector<dsp*> fDspList;
	int fSamplingFreq;
 public:
	dsp() {
		fDspList.push_back(this);
	}
	virtual ~dsp() {
		fDspList.erase(std::remove(std::begin(fDspList),
				           std::end(fDspList), this), std::end(fDspList));
	}

	static int getNumDsps();
	static dsp* getDsp(int n);

	virtual int getNumInputs() = 0;
	virtual int getNumOutputs() = 0;
	virtual void buildUserInterface(UI* interface) = 0;
	virtual void init(int samplingRate) = 0;
 	virtual void compute(int len, float** inputs, float** outputs) = 0;
};

#endif // DEVELAMP_DSP_H_
