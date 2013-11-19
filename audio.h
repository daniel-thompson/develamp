/*
 * audio.h
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

/******************************************************************************
*******************************************************************************

		An abstraction layer over audio layer

*******************************************************************************
*******************************************************************************/

#ifndef DEVELAMP_AUDIO_H_
#define DEVELAMP_AUDIO_H_
			
class dsp;
class audio {
 public:
	audio() {}
	virtual ~audio() {}
	
	virtual bool init(const char* name, dsp*) = 0;
	virtual bool start() = 0;
	virtual void stop() = 0;
};
					
#endif // DEVELAMP_AUDIO_H_
