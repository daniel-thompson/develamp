/*
 * This is very small Schroeder reverb based on jcrev by John Chowning.
 *
 * Usage:
 *   _ : reverb : _,_
 *   _ : reverb : _,!
 */

declare name      "JCRev Reverb";
declare license   "GPLv3+";
declare priority  "90";

import("gui.lib");
import("util.lib");

dryreverb(bypass) = bypassmono(bypass, *(0.06) : allpass_chain <: comb_bank :> _) <: (_, bypassmono(bypass, *(-1))) with {
	allpass_chain = allpass(347,0.7) :
			allpass(113,0.7) :
			allpass( 37,0.7) with {
		allpass(d,g) = (+ <: @(d-1),*(-g)) ~ *(g) : mem,_ : + ;

		/* This is an experiment in exchanging the colourless allpass
		 * filter for one that colours the output but uses one less
		 * multiply. The reverb effect is not killed completely but it
		 * sounds like all the constants would need retuning to try
		 * and flatten the response a bit. Too hard to do now!
		 */
		//allpass(d, g) = (+ : *(g+0.15) <: (@(d-1),_)) ~ _ : (mem,_) : -;
	};
	
	comb_bank = comb(1601,.802) ,
   		    comb(1867,.773) ,
		    comb(2053,.753) ,
		    comb(2251,.733) with {
		comb(d,g) =  (+ <: @(d-1),_) ~ *(g) : !,_;
	};

	/* This is an experiment to produce a stereo reverberation that can be
	 * mixed back to mono without cancelling out. However at low
	 * levels its too subtle to worry about and at high levels it
	 * influences the frequency response too much.
	 */
	downmix = _,_,_,_ <: (*(boost),*(cut),*(boost),*(cut) :> _),(*(cut),*(boost),*(cut),*(boost) :> _) with {
		spread = vslider("Spread", 0.25, 0.0, 1.0, 0.05);
		boost = 1 + spread;
		cut = 1 - spread;
	};
};



reverb(bypass, wetness) = _ <: _,_,dryreverb(bypass) : swap :
		wetdry(wetness),wetdry(wetness) with {
	swap(a, b, c, d) = a, c, b, d;
};

reverbapp = (gui, _) : reverb with {
	bypass = bypassgui;
	wetness = wetdrygui : rerange(0, 10, 1, 0);

	gui = hgroup("[0] Reverb", bypass, wetness);
};

process = reverbapp;

