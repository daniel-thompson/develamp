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

import("gui.lib");
import("util.lib");

// Tuneable value
aa_cutoff = 5500;
aa_Q = 2.6;
clean_cutoff = 4000;
clean_Q = 1.0;
feedback_cutoff = 80;
feedback_Q = 0.9;

buffer(gain) = boost : antialias <: (>(0), positive, negative) : if : cleanup with {
	boost = *(gain);
	antialias = LPF(_, aa_cutoff, 1, aa_Q);
	cleanup = LPF(_, clean_cutoff, 1, clean_Q);
	
	pnonlin(x) = ((4.0*x/3.0) - (2.0*(x*x)/3.0) + (1.0/3.0));
	nnonlin(x) = ((4.0*x/3.0) - (4.0*(x*x*x)/3.0) - (4.0*(x*x)/3.0) + (1.0/3.0));

	positive = _ <: (>(1), 1.0, pnonlin) : if;
	negative = _ <: (<(-1), -1.0, nnonlin) : if;
};

feedbackstage(gain) = (* : buffer(gain)) ~ feedback with {
	feedback = *(0 - db2linear(-24)) : LPF(_, feedback_cutoff, 1, feedback_Q);
};

bitshrieker(bypass, gain, volume) =
bypassmono(bypass, buffer(1) : feedbackstage(gain): dcblocker : buffer(volume) : dcblocker);

bitshriekerapp = (gui, _) : bitshrieker with {
	bypass = bypassgui;
	gain = hslider("[2] Gain [style: knob]", 5, 0, 10, 0.1)
		: rerange(0, 10, 0, 48) : db2linear : smoothctl;
	// TODO: add a tone control (probably based on the feedback cutoff)
	volume = hslider("[3] Volume [style: knob]", 5, 0, 10, 0.1)
		: rerange(0, 10, -48, 24) : db2linear : smoothctl;

	gui = hgroup("[0] Bitshrieker", bypass, gain, volume);

};

process = bitshriekerapp;


