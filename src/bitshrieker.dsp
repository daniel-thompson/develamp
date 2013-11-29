declare name      "Bitshieker Overdrive";
declare author    "Daniel Thompson";
declare copyright "2013";
declare version   "0.1";
declare license   "GPLv3+";
declare priority  "20";

import("effect.lib");
import("filter.lib");
import("maxmsp.lib");
import("music.lib");

import("util.lib");

vumeter = component("vumeter.dsp");

// Tuneable value
aa_cutoff = hslider("[1] Buffer AA cutoff [unit:Hz]", 2560, 320, 10240, 10);
aa_Q = hslider("[2] Buffer AA Q", 0.7, 0.3, 8, 0.1);
clean_cutoff = hslider("[3] Buffer clean cutoff [unit:Hz]", 2560, 320, 10240, 10);
clean_Q = hslider("[4] Buffer clean Q", 0.7, 0.3, 8, 0.1);
feedback_cutoff = hslider("[3] Feedback cutoff [unit:Hz]", 160, 20, 640, 1);
feedback_Q = hslider("[4] Buffer clean Q", 0.7, 0.3, 8, 0.1);



buffer(gain) = boost : antialias <: (>(0), positive, negative) : if : cleanup with {
	boost = *(gain) : *(db2linear(48));
	antialias = LPF(_, aa_cutoff, 1, aa_Q);
	cleanup = LPF(_, clean_cutoff, 1, clean_Q);
	
	pnonlin(x) = ((4.0*x/3.0) - (2.0*(x*x)/3.0) + (1.0/3.0));
	nnonlin(x) = ((4.0*x/3.0) - (4.0*(x*x*x)/3.0) - (4.0*(x*x)/3.0) + (1.0/3.0));

	positive = _ <: (>(1), 1.0, pnonlin) : if;
	negative = _ <: (<(-1), -1.0, nnonlin) : if;
};

feedbackstage(gain) = (+ : buffer(gain)) ~ feedback with {
	feedback = *(0 - db2linear(-24)) : LPF(_, feedback_cutoff, 1, feedback_Q);
};

// TODO: Add a shelving filter to boost frequencies below the feedback cutoff
bitshrieker(gain, volume) =
	buffer(db2linear(-42)) : feedbackstage(gain) : buffer(volume) : dcblocker;

bitshriekerapp = (gui, _) : bitshrieker with {
	gain = hslider("[0] Gain [style: knob]", 5, 0, 10, 0.1)
		: rerange(0, 10, -96, 0) : db2linear : smoothctl;
	// TODO: add a tone control (probably based on the feedback cutoff
	volume = hslider("[1] Volume [style: knob]", 5, 0, 10, 0.1)
		: rerange(0, 10, -96, 0) : db2linear : smoothctl : vumeter;
		
	gui = hgroup("[0] Bitshrieker", gain, volume);
};

process = bitshriekerapp;


