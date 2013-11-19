/*
 * gtk_knob.h
 *
 * Part of develamp (the research and development amplifier)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
 * Copyright (C) 2003-2011 Thomas Charbonnel and GRAME
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef DEVELAMP_GTK_KNOB_H_
#define DEVELAMP_GTK_KNOB_H_

#include <string>
#include <set>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

namespace gtk_knob
{

class GtkKnob
{
private:
	double start_x, start_y, max_value;
public:
	GtkRange parent;
	int last_quadrant;
	GtkKnob();
	~GtkKnob();
	GtkWidget *gtk_knob_new_with_adjustment(GtkAdjustment *_adjustment);
	
};

#define GTK_TYPE_KNOB          (gtk_knob_get_type())
#define GTK_KNOB(obj)          (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_KNOB, GtkKnob))
#define GTK_IS_KNOB(obj)       (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_KNOB))
#define GTK_KNOB_CLASS(klass)  (G_TYPE_CHECK_CLASS_CAST ((klass),  GTK_TYPE_KNOB, GtkKnobClass))
#define GTK_IS_KNOB_CLASS(obj) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GTK_TYPE_KNOB))

struct GtkKnobClass {
	GtkRangeClass parent_class;
	int knob_x;
	int knob_y;
	int knob_step;
	int button_is;

};

//------forward declaration
GType gtk_knob_get_type ();

}/* end of gtk_knob namespace */

#endif // DEVELAMP_GTK_KNOB_H_
