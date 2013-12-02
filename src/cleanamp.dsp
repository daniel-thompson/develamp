declare id "12ax7"; // in amp tube selector
declare name "Clean amplifier";

import("music.lib");
import("filter.lib");
import("effect.lib"); 

import("gui.lib");

gxamp = library("gxamp.dsp");
gxdistortion = library("gxdistortion.dsp");

cleanamp(drive, wet_dry, preamp, gain1) =
	gxdistortion.dist(drive,wet_dry) : gxamp.tubeax(preamp,gain1);

cleanampapp = (gui, _) : cleanamp with {
    drive = drivegui : rerange(0, 10, 0, 1);
    wet_dry = wetdrygui : rerange(0, 10, 0, 1);
    preamp = volumegui : rerange(0, 10, -20, 20) : db2linear;
    gain1 = mastergui : rerange(0, 10, -20, 20) : db2linear;

    gui = hgroup("[0] Clean amplifier", drive, wet_dry, preamp, gain1);
};

process = cleanampapp;
