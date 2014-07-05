declare name      "Simple speaker simulator";
declare author    "Daniel Thompson";
declare copyright "2013";
declare license   "GPLv3+";
declare priority  "80";

bq = library("maxmsp.lib");
filter = library("filter.lib");

g = library("gui.lib");
u = library("util.lib");

spksim(bypass) = u.cutoutmono(bypass, notch : shelf : hpf : lpf : lpf) with {
	//notch = bq.peakingEQ(_, 400, -16, 1.0);
	notch = _;
	//shelf = bq.highShelf(_, 400, 6, 1.0);
	shelf = _;

	//hpf = bq.HPF(_, 60, 0, 1.0);
	hpf = filter.resonhp(60, 0.7, 0.999);
	//lpf = bq.LPF(_, 4000, 0, 1.0);
	lpf = filter.resonlp(4000, 0.7, 0.999);
};


spksimapp = (gui, _) : spksim with {
	bypass = g.bypassgui;

	gui = hgroup("[0] Simple speaker simulator", bypass);

};

process = spksimapp;


