//-----------------------------------------------------
// name: "12ax7"
//
// Code generated with Faust 0.9.46 (http://faust.grame.fr)
//-----------------------------------------------------
/* link with : "" */
#include "valve.h"
#include <math.h>
#include <cmath>
template <int N> inline float faustpower(float x) 		{ return powf(x,N); } 
template <int N> inline double faustpower(double x) 	{ return pow(x,N); }
template <int N> inline int faustpower(int x) 			{ return faustpower<N/2>(x) * faustpower<N-N/2>(x); } 
template <> 	 inline int faustpower<0>(int x) 		{ return 1; }
template <> 	 inline int faustpower<1>(int x) 		{ return x; }
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


//----------------------------------------------------------------
//  Signal processor definition
//----------------------------------------------------------------

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  

typedef long double quad;

#define FAUSTCLASS gxamp

class gxamp : public dsp {
  private:
	FAUSTFLOAT 	fslider0;
	float 	fRec0[2];
	int 	iConst0;
	float 	fConst1;
	float 	fConst2;
	float 	fConst3;
	float 	fConst4;
	float 	fConst5;
	float 	fConst6;
	FAUSTFLOAT 	fslider1;
	float 	fRec7[2];
	float 	fConst7;
	float 	fConst8;
	float 	fConst9;
	float 	fConst10;
	float 	fConst11;
	float 	fConst12;
	float 	fConst13;
	float 	fConst14;
	float 	fConst15;
	float 	fConst16;
	float 	fConst17;
	float 	fConst18;
	float 	fConst19;
	float 	fConst20;
	float 	fConst21;
	float 	fConst22;
	float 	fConst23;
	float 	fConst24;
	float 	fConst25;
	float 	fConst26;
	float 	fConst27;
	float 	fConst28;
	float 	fConst29;
	float 	fConst30;
	float 	fConst31;
	float 	fConst32;
	float 	fConst33;
	float 	fConst34;
	float 	fConst35;
	float 	fConst36;
	FAUSTFLOAT 	fslider2;
	float 	fRec18[2];
	float 	fRec17[3];
	float 	fVec0[2];
	float 	fConst37;
	float 	fConst38;
	float 	fRec16[2];
	float 	fConst39;
	float 	fRec15[2];
	float 	fRec14[3];
	float 	fVec1[2];
	float 	fConst40;
	float 	fConst41;
	float 	fRec13[2];
	float 	fRec12[3];
	float 	fConst42;
	float 	fRec11[3];
	FAUSTFLOAT 	fslider3;
	float 	fConst43;
	float 	fConst44;
	float 	fConst45;
	float 	fConst46;
	float 	fRec22[2];
	float 	fRec21[3];
	float 	fVec2[2];
	float 	fConst47;
	float 	fConst48;
	float 	fRec20[2];
	float 	fRec19[3];
	float 	fConst49;
	float 	fConst50;
	float 	fConst51;
	float 	fConst52;
	float 	fRec26[2];
	float 	fRec25[3];
	float 	fConst53;
	float 	fRec24[3];
	float 	fRec23[3];
	float 	fRec28[2];
	float 	fRec27[3];
	float 	fVec3[2];
	float 	fConst54;
	float 	fRec10[2];
	float 	fConst55;
	float 	fConst56;
	float 	fConst57;
	float 	fConst58;
	float 	fRec29[2];
	float 	fRec9[3];
	float 	fConst59;
	float 	fRec8[2];
	float 	fVec4[2];
	float 	fRec6[2];
	float 	fConst60;
	float 	fConst61;
	float 	fConst62;
	float 	fConst63;
	float 	fRec30[2];
	float 	fRec5[3];
	float 	fRec4[2];
	float 	fVec5[2];
	float 	fRec3[2];
	float 	fConst64;
	float 	fConst65;
	float 	fConst66;
	float 	fConst67;
	float 	fRec31[2];
	float 	fRec2[3];
	float 	fRec1[2];
  public:
	static void metadata(Meta* m) 	{ 
		m->declare("id", "12ax7");
		m->declare("name", "12ax7");
		m->declare("music.lib/name", "Music Library");
		m->declare("music.lib/author", "GRAME");
		m->declare("music.lib/copyright", "GRAME");
		m->declare("music.lib/version", "1.0");
		m->declare("music.lib/license", "LGPL");
		m->declare("math.lib/name", "Math Library");
		m->declare("math.lib/author", "GRAME");
		m->declare("math.lib/copyright", "GRAME");
		m->declare("math.lib/version", "1.0");
		m->declare("math.lib/license", "LGPL");
		m->declare("filter.lib/name", "Faust Filter Library");
		m->declare("filter.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("filter.lib/copyright", "Julius O. Smith III");
		m->declare("filter.lib/version", "1.29");
		m->declare("filter.lib/license", "STK-4.3");
		m->declare("filter.lib/reference", "https://ccrma.stanford.edu/~jos/filters/");
		m->declare("effect.lib/name", "Faust Audio Effect Library");
		m->declare("effect.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("effect.lib/copyright", "Julius O. Smith III");
		m->declare("effect.lib/version", "1.33");
		m->declare("effect.lib/license", "STK-4.3");
		m->declare("effect.lib/reference", "https://ccrma.stanford.edu/realsimple/faust_strings/");
		m->declare("gxdistortion.dsp/id", "gxdistortion");
		m->declare("gxdistortion.dsp/version", "0.01");
		m->declare("gxdistortion.dsp/author", "brummer");
		m->declare("gxdistortion.dsp/license", "BSD");
		m->declare("gxdistortion.dsp/copyright", "(c)brummer 2008");
		m->declare("maxmsp.lib/name", "MaxMSP compatibility Library");
		m->declare("maxmsp.lib/author", "GRAME");
		m->declare("maxmsp.lib/copyright", "GRAME");
		m->declare("maxmsp.lib/version", "1.0");
		m->declare("maxmsp.lib/license", "LGPL");
	}

	virtual int getNumInputs() 	{ return 1; }
	virtual int getNumOutputs() 	{ return 1; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceInit(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fslider0 = -6.0f;
		for (int i=0; i<2; i++) fRec0[i] = 0;
		iConst0 = min(192000, max(1, fSamplingFreq));
		fConst1 = (1.0f / tanf((97.38937226128358f / iConst0)));
		fConst2 = (1 + fConst1);
		fConst3 = (0 - ((1 - fConst1) / fConst2));
		fConst4 = (1.0f / tanf((20517.741620594938f / iConst0)));
		fConst5 = (1 + fConst4);
		fConst6 = (0 - ((1 - fConst4) / fConst5));
		fslider1 = -6.0f;
		for (int i=0; i<2; i++) fRec7[i] = 0;
		fConst7 = tanf((942.4777960769379f / iConst0));
		fConst8 = (1.0f / faustpower<2>(fConst7));
		fConst9 = (2 * (1 - fConst8));
		fConst10 = (1.0f / fConst7);
		fConst11 = (1 + ((fConst10 - 1.0f) / fConst7));
		fConst12 = (1.0f / (1 + ((1.0f + fConst10) / fConst7)));
		fConst13 = tanf((3769.9111843077517f / iConst0));
		fConst14 = (1.0f / faustpower<2>(fConst13));
		fConst15 = (2 * (1 - fConst14));
		fConst16 = (1.0f / fConst13);
		fConst17 = (1 + ((fConst16 - 1.0000000000000004f) / fConst13));
		fConst18 = (1 + ((fConst16 + 1.0000000000000004f) / fConst13));
		fConst19 = (1.0f / fConst18);
		fConst20 = (1 + fConst16);
		fConst21 = (0 - ((1 - fConst16) / fConst20));
		fConst22 = tanf((10053.096491487338f / iConst0));
		fConst23 = (1.0f / faustpower<2>(fConst22));
		fConst24 = (2 * (1 - fConst23));
		fConst25 = (1.0f / fConst22);
		fConst26 = (1 + ((fConst25 - 1.0000000000000004f) / fConst22));
		fConst27 = (1 + ((1.0000000000000004f + fConst25) / fConst22));
		fConst28 = (1.0f / fConst27);
		fConst29 = (1 + fConst25);
		fConst30 = (0 - ((1 - fConst25) / fConst29));
		fConst31 = tanf((47123.8898038469f / iConst0));
		fConst32 = (2 * (1 - (1.0f / faustpower<2>(fConst31))));
		fConst33 = (1.0f / fConst31);
		fConst34 = (1 + ((fConst33 - 1.414213562373095f) / fConst31));
		fConst35 = (1 + ((1.414213562373095f + fConst33) / fConst31));
		fConst36 = (1.0f / fConst35);
		fslider2 = 1e+02f;
		for (int i=0; i<2; i++) fRec18[i] = 0;
		for (int i=0; i<3; i++) fRec17[i] = 0;
		for (int i=0; i<2; i++) fVec0[i] = 0;
		fConst37 = (0 - fConst1);
		fConst38 = (1.0f / (fConst2 * fConst35));
		for (int i=0; i<2; i++) fRec16[i] = 0;
		fConst39 = (1.0f / fConst29);
		for (int i=0; i<2; i++) fRec15[i] = 0;
		for (int i=0; i<3; i++) fRec14[i] = 0;
		for (int i=0; i<2; i++) fVec1[i] = 0;
		fConst40 = (0 - fConst16);
		fConst41 = (1.0f / (fConst20 * fConst27));
		for (int i=0; i<2; i++) fRec13[i] = 0;
		for (int i=0; i<3; i++) fRec12[i] = 0;
		fConst42 = (2 * (0 - fConst14));
		for (int i=0; i<3; i++) fRec11[i] = 0;
		fslider3 = 0.35f;
		fConst43 = (1 + ((fConst10 - 1.0000000000000004f) / fConst7));
		fConst44 = (1.0f / (1 + ((fConst10 + 1.0000000000000004f) / fConst7)));
		fConst45 = (1 + fConst10);
		fConst46 = (0 - ((1 - fConst10) / fConst45));
		for (int i=0; i<2; i++) fRec22[i] = 0;
		for (int i=0; i<3; i++) fRec21[i] = 0;
		for (int i=0; i<2; i++) fVec2[i] = 0;
		fConst47 = (0 - fConst10);
		fConst48 = (1.0f / (fConst45 * fConst18));
		for (int i=0; i<2; i++) fRec20[i] = 0;
		for (int i=0; i<3; i++) fRec19[i] = 0;
		fConst49 = (2 * (0 - fConst8));
		fConst50 = (1 + ((fConst16 - 1.0f) / fConst13));
		fConst51 = (1.0f / (1 + ((1.0f + fConst16) / fConst13)));
		fConst52 = (0 - fConst25);
		for (int i=0; i<2; i++) fRec26[i] = 0;
		for (int i=0; i<3; i++) fRec25[i] = 0;
		fConst53 = (2 * (0 - fConst23));
		for (int i=0; i<3; i++) fRec24[i] = 0;
		for (int i=0; i<3; i++) fRec23[i] = 0;
		for (int i=0; i<2; i++) fRec28[i] = 0;
		for (int i=0; i<3; i++) fRec27[i] = 0;
		for (int i=0; i<2; i++) fVec3[i] = 0;
		fConst54 = (1.0f / fConst5);
		for (int i=0; i<2; i++) fRec10[i] = 0;
		fConst55 = (1.0f / tanf((270.1769682087222f / iConst0)));
		fConst56 = (1 + fConst55);
		fConst57 = (0 - ((1 - fConst55) / fConst56));
		fConst58 = (0.027f / fConst56);
		for (int i=0; i<2; i++) fRec29[i] = 0;
		for (int i=0; i<3; i++) fRec9[i] = 0;
		fConst59 = (0.025f / fConst2);
		for (int i=0; i<2; i++) fRec8[i] = 0;
		for (int i=0; i<2; i++) fVec4[i] = 0;
		for (int i=0; i<2; i++) fRec6[i] = 0;
		fConst60 = (1.0f / tanf((414.6902302738527f / iConst0)));
		fConst61 = (1 + fConst60);
		fConst62 = (0 - ((1 - fConst60) / fConst61));
		fConst63 = (0.015f / fConst61);
		for (int i=0; i<2; i++) fRec30[i] = 0;
		for (int i=0; i<3; i++) fRec5[i] = 0;
		for (int i=0; i<2; i++) fRec4[i] = 0;
		for (int i=0; i<2; i++) fVec5[i] = 0;
		for (int i=0; i<2; i++) fRec3[i] = 0;
		fConst64 = (1.0f / tanf((609.4689747964198f / iConst0)));
		fConst65 = (1 + fConst64);
		fConst66 = (0 - ((1 - fConst64) / fConst65));
		fConst67 = (0.0082f / fConst65);
		for (int i=0; i<2; i++) fRec31[i] = 0;
		for (int i=0; i<3; i++) fRec2[i] = 0;
		for (int i=0; i<2; i++) fRec1[i] = 0;
	}
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void buildUserInterface(UI* interface) {
		interface->openVerticalBox("gxamp");
		interface->declare(&fslider3, "alias", "");
		interface->addVerticalSlider(".gxdistortion.drive", &fslider3, 0.35f, 0.0f, 1.0f, 0.01f);
		interface->declare(&fslider2, "alias", "");
		interface->addVerticalSlider(".gxdistortion.wet_dry", &fslider2, 1e+02f, 0.0f, 1e+02f, 1.0f);
		interface->openHorizontalBox("stage1");
		interface->declare(&fslider1, "alias", "");
		interface->addVerticalSlider(".amp2.stage1.Pregain", &fslider1, -6.0f, -2e+01f, 2e+01f, 0.1f);
		interface->closeBox();
		interface->openHorizontalBox("stage2");
		interface->declare(&fslider0, "alias", "");
		interface->addVerticalSlider(".amp2.stage2.gain1", &fslider0, -6.0f, -2e+01f, 2e+01f, 0.1f);
		interface->closeBox();
		interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		float 	fSlow0 = (0.0010000000000000009f * powf(10,(0.05f * fslider0)));
		float 	fSlow1 = (0.0010000000000000009f * powf(10,(0.05f * fslider1)));
		float 	fSlow2 = (1.000000000000001e-05f * fslider2);
		float 	fSlow3 = fslider3;
		float 	fSlow4 = powf(1e+01f,(1.2f * fSlow3));
		float 	fSlow5 = (fConst44 * powf(1e+01f,(2 * fSlow3)));
		float 	fSlow6 = powf(1e+01f,(0.8f * fSlow3));
		float 	fSlow7 = (fConst44 * powf(1e+01f,(0.9f * fSlow3)));
		float 	fSlow8 = (2 * (fSlow3 - 0.5f));
		float 	fSlow9 = (1 - max(0, (0 - fSlow8)));
		float 	fSlow10 = (1.25f * fSlow3);
		float 	fSlow11 = (1 - max(0, fSlow8));
		FAUSTFLOAT* input0 = input[0];
		FAUSTFLOAT* output0 = output[0];
		for (int i=0; i<count; i++) {
			fRec0[0] = (fSlow0 + (0.999f * fRec0[1]));
			fRec7[0] = (fSlow1 + (0.999f * fRec7[1]));
			float fTemp0 = (fConst9 * fRec11[1]);
			fRec18[0] = (fSlow2 + (0.999f * fRec18[1]));
			float fTemp1 = (float)input0[i];
			fRec17[0] = ((fTemp1 * fRec18[0]) - (fConst36 * ((fConst34 * fRec17[2]) + (fConst32 * fRec17[1]))));
			float fTemp2 = (fRec17[2] + (fRec17[0] + (2 * fRec17[1])));
			fVec0[0] = fTemp2;
			fRec16[0] = ((fConst38 * ((fConst37 * fVec0[1]) + (fConst1 * fVec0[0]))) + (fConst3 * fRec16[1]));
			fRec15[0] = ((fConst39 * (fRec16[0] + fRec16[1])) + (fConst30 * fRec15[1]));
			fRec14[0] = (fRec15[0] - (fConst28 * ((fConst26 * fRec14[2]) + (fConst24 * fRec14[1]))));
			float fTemp3 = (fRec14[2] + (fRec14[0] + (2 * fRec14[1])));
			fVec1[0] = fTemp3;
			fRec13[0] = ((fConst41 * ((fConst40 * fVec1[1]) + (fConst16 * fVec1[0]))) + (fConst21 * fRec13[1]));
			fRec12[0] = (fRec13[0] - (fConst19 * ((fConst17 * fRec12[2]) + (fConst15 * fRec12[1]))));
			fRec11[0] = ((fConst19 * (((fConst14 * fRec12[0]) + (fConst42 * fRec12[1])) + (fConst14 * fRec12[2]))) - (fConst12 * ((fConst11 * fRec11[2]) + fTemp0)));
			float fTemp4 = max(-1, min(1, (fSlow4 * (fRec11[2] + (fConst12 * (fTemp0 + (fConst11 * fRec11[0])))))));
			fRec22[0] = ((fConst41 * (fVec1[0] + fVec1[1])) + (fConst21 * fRec22[1]));
			fRec21[0] = (fRec22[0] - (fConst19 * ((fConst17 * fRec21[2]) + (fConst15 * fRec21[1]))));
			float fTemp5 = (fRec21[2] + (fRec21[0] + (2 * fRec21[1])));
			fVec2[0] = fTemp5;
			fRec20[0] = ((fConst48 * ((fConst47 * fVec2[1]) + (fConst10 * fVec2[0]))) + (fConst46 * fRec20[1]));
			fRec19[0] = (fRec20[0] - (fConst44 * ((fConst43 * fRec19[2]) + (fConst9 * fRec19[1]))));
			float fTemp6 = max(-1, min(1, (fSlow5 * (((fConst8 * fRec19[0]) + (fConst49 * fRec19[1])) + (fConst8 * fRec19[2])))));
			float fTemp7 = (fConst9 * fRec23[1]);
			float fTemp8 = (fConst15 * fRec24[1]);
			fRec26[0] = ((fConst39 * ((fConst52 * fRec16[1]) + (fConst25 * fRec16[0]))) + (fConst30 * fRec26[1]));
			fRec25[0] = (fRec26[0] - (fConst28 * ((fConst26 * fRec25[2]) + (fConst24 * fRec25[1]))));
			fRec24[0] = ((fConst28 * (((fConst23 * fRec25[0]) + (fConst53 * fRec25[1])) + (fConst23 * fRec25[2]))) - (fConst51 * ((fConst50 * fRec24[2]) + fTemp8)));
			fRec23[0] = ((fRec24[2] + (fConst51 * (fTemp8 + (fConst50 * fRec24[0])))) - (fConst12 * ((fConst11 * fRec23[2]) + fTemp7)));
			float fTemp9 = max(-1, min(1, (fSlow6 * (fRec23[2] + (fConst12 * (fTemp7 + (fConst11 * fRec23[0])))))));
			fRec28[0] = ((fConst48 * (fVec2[0] + fVec2[1])) + (fConst46 * fRec28[1]));
			fRec27[0] = (fRec28[0] - (fConst44 * ((fConst43 * fRec27[2]) + (fConst9 * fRec27[1]))));
			float fTemp10 = max(-1, min(1, (fSlow7 * (fRec27[2] + (fRec27[0] + (2 * fRec27[1]))))));
			float fTemp11 = ((1.2589412f * (fTemp10 * (1 - (0.3333333333333333f * faustpower<2>(fTemp10))))) + (1.584893192f * ((fTemp9 * (1 - (0.3333333333333333f * faustpower<2>(fTemp9)))) + ((fTemp6 * (1 - (0.3333333333333333f * faustpower<2>(fTemp6)))) + (0.8413951417869425f * (fTemp4 * (1 - (0.3333333333333333f * faustpower<2>(fTemp4)))))))));
			fVec3[0] = fTemp11;
			fRec10[0] = ((fConst54 * (fVec3[0] + fVec3[1])) + (fConst6 * fRec10[1]));
			fRec29[0] = ((fConst58 * (fRec9[1] + fRec9[2])) + (fConst57 * fRec29[1]));
			float fTemp12 = (1 - fRec18[0]);
			fRec9[0] = (Ftube(TUBE_TABLE_12AX7_68k, (((fTemp1 * ((fSlow11 * fRec18[0]) + (fTemp12 * (1 + (fSlow10 * ((4 - (4 * fabsf((fTemp1 * fTemp12)))) - 1)))))) + (fRec29[0] + (fSlow9 * fRec10[0]))) - 1.581656f)) - 191.42014814814814f);
			fRec8[0] = ((fConst59 * ((fConst37 * fRec9[1]) + (fConst1 * fRec9[0]))) + (fConst3 * fRec8[1]));
			float fTemp13 = (fRec8[0] * fRec7[0]);
			fVec4[0] = fTemp13;
			fRec6[0] = ((fConst54 * (fVec4[0] + fVec4[1])) + (fConst6 * fRec6[1]));
			fRec30[0] = ((fConst63 * (fRec5[1] + fRec5[2])) + (fConst62 * fRec30[1]));
			fRec5[0] = (Ftube(TUBE_TABLE_12AX7_250k, ((fRec30[0] + fRec6[0]) - 1.204285f)) - 169.71433333333334f);
			fRec4[0] = ((fConst59 * ((fConst37 * fRec5[1]) + (fConst1 * fRec5[0]))) + (fConst3 * fRec4[1]));
			float fTemp14 = (fRec7[0] * fRec4[0]);
			fVec5[0] = fTemp14;
			fRec3[0] = ((fConst54 * (fVec5[0] + fVec5[1])) + (fConst6 * fRec3[1]));
			fRec31[0] = ((fConst67 * (fRec2[1] + fRec2[2])) + (fConst66 * fRec31[1]));
			fRec2[0] = (Ftube(TUBE_TABLE_12AX7_250k, ((fRec31[0] + fRec3[0]) - 0.840703f)) - 147.47524390243905f);
			fRec1[0] = ((fConst59 * ((fConst37 * fRec2[1]) + (fConst1 * fRec2[0]))) + (fConst3 * fRec1[1]));
			output0[i] = (FAUSTFLOAT)(fRec1[0] * fRec0[0]);
			// post processing
			fRec1[1] = fRec1[0];
			fRec2[2] = fRec2[1]; fRec2[1] = fRec2[0];
			fRec31[1] = fRec31[0];
			fRec3[1] = fRec3[0];
			fVec5[1] = fVec5[0];
			fRec4[1] = fRec4[0];
			fRec5[2] = fRec5[1]; fRec5[1] = fRec5[0];
			fRec30[1] = fRec30[0];
			fRec6[1] = fRec6[0];
			fVec4[1] = fVec4[0];
			fRec8[1] = fRec8[0];
			fRec9[2] = fRec9[1]; fRec9[1] = fRec9[0];
			fRec29[1] = fRec29[0];
			fRec10[1] = fRec10[0];
			fVec3[1] = fVec3[0];
			fRec27[2] = fRec27[1]; fRec27[1] = fRec27[0];
			fRec28[1] = fRec28[0];
			fRec23[2] = fRec23[1]; fRec23[1] = fRec23[0];
			fRec24[2] = fRec24[1]; fRec24[1] = fRec24[0];
			fRec25[2] = fRec25[1]; fRec25[1] = fRec25[0];
			fRec26[1] = fRec26[0];
			fRec19[2] = fRec19[1]; fRec19[1] = fRec19[0];
			fRec20[1] = fRec20[0];
			fVec2[1] = fVec2[0];
			fRec21[2] = fRec21[1]; fRec21[1] = fRec21[0];
			fRec22[1] = fRec22[0];
			fRec11[2] = fRec11[1]; fRec11[1] = fRec11[0];
			fRec12[2] = fRec12[1]; fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fVec1[1] = fVec1[0];
			fRec14[2] = fRec14[1]; fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fVec0[1] = fVec0[0];
			fRec17[2] = fRec17[1]; fRec17[1] = fRec17[0];
			fRec18[1] = fRec18[0];
			fRec7[1] = fRec7[0];
			fRec0[1] = fRec0[0];
		}
	}
};



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

	virtual unique_ptr<dsp> manufacture_dsp() const override {
		return unique_ptr<dsp>{new FAUSTCLASS};
	}
};

static bool unused = dsp_factory::make_factory<FAUSTFACTORY>();
