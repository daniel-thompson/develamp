/*
 * misc.h
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
// Original license (source came from GNU GPLv3 package and had not license
// text but was included by the module.cpp architecture file which uses this
// license).
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

#ifndef __misc__
#define __misc__

#include <map>
#include <string.h>

// On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
// flags to avoid costly denormals
#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare (const char* key, const char* value) { (*this)[key]=value; }
};

struct Meta
{
    virtual void declare (const char* key, const char* value) = 0;
};

struct MY_Meta : Meta, std::map<const char*, const char*>
{
    void declare (const char* key, const char* value) { (*this)[key]=value; }
};

using std::max;
inline float max(int x, float y) { return max(static_cast<float>(x), y); }
inline float max(float x, int y) { return max(x, static_cast<float>(y)); }

using std::min;
inline float min(int x, float y) { return min(static_cast<float>(x), y); }
inline float min(float x, int y) { return min(x, static_cast<float>(y)); }


inline int		lsr (int x, int n)		{ return int(((unsigned int)x) >> n); }
inline int 		int2pow2 (int x)		{ int r=0; while ((1<<r)<x) r++; return r; }

inline long lopt(char *argv[], const char *name, long def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return atoi(argv[i+1]);
	return def;
}

inline char* lopts(char *argv[], const char *name, char* def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return argv[i+1];
	return def;
}
#endif

