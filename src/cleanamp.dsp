declare name "Clean amplifier";
declare license "GPLv3+";

import("music.lib");
import("filter.lib");
import("effect.lib"); 

import("gui.lib");
import("util.lib");

gxamp = library("gxamp.dsp");
gxdistortion = library("gxdistortion.dsp");

cleanamp(bypass, drive, wet_dry, preamp, gain1) =
	cutoutmono(bypass, gxdistortion.dist(drive,wet_dry) : gxamp.tubeax(preamp,gain1));

cleanampapp = (gui, _) : cleanamp with {
    bypass = bypassgui;
    drive = drivegui : rerange(0, 10, 0, 1);
    wet_dry = wetdrygui : rerange(0, 10, 0, 1);
    preamp = volumegui : rerange(0, 10, -20, 20) : db2linear;
    gain1 = mastergui : rerange(0, 10, -20, 20) : db2linear;

    gui = hgroup("[0] Clean amplifier", bypass, drive, wet_dry, preamp, gain1);
};

process = cleanampapp;
