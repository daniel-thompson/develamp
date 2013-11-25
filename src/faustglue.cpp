/*
 * faustglue.cpp
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
// Original license (from the module.cpp architecture).
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

#include <map>
#include <list>

#include "dsp.h"
#include "dsp_factory.h"
#include "GUI.h"
#include "misc.h"

using namespace std;

/******************************************************************************
*******************************************************************************

							       VECTOR INTRINSICS

*******************************************************************************
*******************************************************************************/

<<includeIntrinsic>>

//----------------------------------------------------------------
//  Signal processor definition
//----------------------------------------------------------------

<<includeclass>>

//----------------------------------------------------------------
//  Static factory class and instance (will be added to a list during construction)
//----------------------------------------------------------------

// generate the factory class name
#define __FAUSTFACTORY(x) x ## _factory
#define _FAUSTFACTORY(x) __FAUSTFACTORY(x)
#define FAUSTFACTORY _FAUSTFACTORY(FAUSTCLASS)

class FAUSTFACTORY : public dsp_factory {
public:
	FAUSTFACTORY() {
		FAUSTCLASS::metadata(&meta);
	}

	virtual dsp* manufacture_dsp() const override {
		return new FAUSTCLASS;
	}
};

static FAUSTFACTORY staticdsp;
