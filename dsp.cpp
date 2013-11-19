/*
 * dsp.cpp
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

#include "dsp.h"

std::vector<dsp*> dsp::fDspList;

int dsp::getNumDsps()
{
	return fDspList.size();
}

dsp* dsp::getDsp(int n)
{
	return fDspList.at(n);
}
