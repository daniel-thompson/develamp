/*
 * gtk_knob.cpp
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

#include <string>
#include <set>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <assert.h>

#include "gtk_knob.h"

namespace gtk_knob
{

GtkKnob::GtkKnob()
// GtkKnob constructor
{

}

GtkKnob::~GtkKnob()
{
	// Nothing specific to do...
}

/****************************************************************
 ** calculate the knop pointer with dead zone
 */

const double scale_zero = 20 * (M_PI/180); // defines "dead zone" for knobs

static void knob_expose(GtkWidget *widget, int knob_x, int knob_y, GdkEventExpose *event, int arc_offset)
{
	/** check resize **/
	int grow;
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);
	if(allocation.width > allocation.height) {
		grow = allocation.height;
	} else {
		grow = allocation.width;
	}
	knob_x = grow-4;
	knob_y = grow-4;
	/** get values for the knob **/
	GtkAdjustment *adj = gtk_range_get_adjustment(GTK_RANGE(widget));
	int knobx = (allocation.x+2 + (allocation.width-4 - knob_x) * 0.5);
	int knoby = (allocation.y+2 + (allocation.height-4 - knob_y) * 0.5);
	int knobx1 = (allocation.x+2 + (allocation.width-4)* 0.5);
	int knoby1 = (allocation.y+2 + (allocation.height-4) * 0.5);
	double adjvalue = gtk_adjustment_get_value(adj);
	double adjlower = gtk_adjustment_get_lower(adj);
	double adjupper = gtk_adjustment_get_upper(adj);
	double knobstate = (adjvalue - adjlower) / (adjupper - adjlower);
	double angle = scale_zero + knobstate * 2 * (M_PI - scale_zero);
	double knobstate1 = (0. - adjlower) / (adjupper - adjlower);
	double pointer_off = knob_x/6;
	double radius = MIN(knob_x-pointer_off, knob_y-pointer_off) / 2;
	double lengh_x = (knobx+radius+pointer_off/2) - radius * sin(angle);
	double lengh_y = (knoby+radius+pointer_off/2) + radius * cos(angle);
	double radius1 = MIN(knob_x, knob_y) / 2 ;

	/** get widget forground color convert to cairo **/
	GtkStyle *style = gtk_widget_get_style (widget);
	double r = MIN(0.6,style->fg[gtk_widget_get_state(widget)].red/65535.0),
		   g = MIN(0.6,style->fg[gtk_widget_get_state(widget)].green/65535.0),
		   b = MIN(0.6,style->fg[gtk_widget_get_state(widget)].blue/65535.0);

	/** paint focus **/
	if (gtk_widget_has_focus(widget)== TRUE) {
		gtk_paint_focus(gtk_widget_get_style(widget),
				gtk_widget_get_window(widget),
				GTK_STATE_NORMAL, NULL, widget, NULL,
		                knobx-2, knoby-2, knob_x+4, knob_y+4);
	}
	/** create clowing knobs with cairo **/
	cairo_t *cr = gdk_cairo_create(GDK_DRAWABLE(gtk_widget_get_window(widget)));
	GdkRegion *region;
	region = gdk_region_rectangle (&allocation);
	gdk_region_intersect (region, event->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);
	
	cairo_arc(cr,knobx1+arc_offset, knoby1+arc_offset, knob_x/2.1, 0, 2 * M_PI );
	cairo_pattern_t*pat =
		cairo_pattern_create_radial (knobx1+arc_offset-knob_x/6,knoby1+arc_offset-knob_x/6, 1,knobx1+arc_offset,knoby1+arc_offset,knob_x/2.1 );
	if(adjlower<0 && adjvalue>0.) {
		cairo_pattern_add_color_stop_rgb (pat, 0, r+0.4, g+0.4 + knobstate-knobstate1, b+0.4);
		cairo_pattern_add_color_stop_rgb (pat, 0.7, r+0.15, g+0.15 + (knobstate-knobstate1)*0.5, b+0.15);
		cairo_pattern_add_color_stop_rgb (pat, 1, r, g, b);
	} else if(adjlower<0 && adjvalue<=0.) {
		cairo_pattern_add_color_stop_rgb (pat, 0, r+0.4 +knobstate1- knobstate, g+0.4, b+0.4);
		cairo_pattern_add_color_stop_rgb (pat, 0.7, r+0.15 +(knobstate1- knobstate)*0.5, g+0.15, b+0.15);
		cairo_pattern_add_color_stop_rgb (pat, 1, r, g, b);
	} else {
		cairo_pattern_add_color_stop_rgb (pat, 0, r+0.4, g+0.4 +knobstate, b+0.4);
		cairo_pattern_add_color_stop_rgb (pat, 0.7, r+0.15, g+0.15 + knobstate*0.5, b+0.15);
		cairo_pattern_add_color_stop_rgb (pat, 1, r, g, b);
	}
	cairo_set_source (cr, pat);
	cairo_fill_preserve (cr);
	gdk_cairo_set_source_color(cr, gtk_widget_get_style (widget)->fg);
	cairo_set_line_width(cr, 2.0);
	cairo_stroke(cr);

	/** create a rotating pointer on the kob**/
	cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
	cairo_set_line_width(cr,MAX(3, MIN(7, knob_x/15)));
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND); 
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_BEVEL);
	cairo_move_to(cr, knobx+radius1, knoby+radius1);
	cairo_line_to(cr,lengh_x,lengh_y);
	cairo_stroke(cr);
	cairo_set_source_rgb(cr,  0.9, 0.9, 0.9);
	cairo_set_line_width(cr,MIN(5, MAX(1,knob_x/30)));
	cairo_move_to(cr, knobx+radius1, knoby+radius1);
	cairo_line_to(cr,lengh_x,lengh_y);
	cairo_stroke(cr);
	cairo_pattern_destroy (pat);
	gdk_region_destroy (region);
	cairo_destroy(cr);
}

/****************************************************************
 ** general expose events for all "knob" controllers
 */

//----------- draw the Knob when moved
static gboolean gtk_knob_expose (GtkWidget *widget, GdkEventExpose *event)
{
	g_assert(GTK_IS_KNOB(widget));
	GtkKnobClass *klass =  GTK_KNOB_CLASS(GTK_OBJECT_GET_CLASS(widget));
	knob_expose(widget, klass->knob_x, klass->knob_y, event, 0);
	return TRUE;
}

/****************************************************************
 ** set initial size for GdkDrawable per type
 */

static void gtk_knob_size_request (GtkWidget *widget, GtkRequisition *requisition)
{
	g_assert(GTK_IS_KNOB(widget));
	GtkKnobClass *klass =  GTK_KNOB_CLASS(GTK_OBJECT_GET_CLASS(widget));
	requisition->width = klass->knob_x;
	requisition->height = klass->knob_y;
}

/****************************************************************
 ** set value from key bindings
 */

static void gtk_knob_set_value (GtkWidget *widget, int dir_down)
{
	g_assert(GTK_IS_KNOB(widget));

	GtkAdjustment *adj = gtk_range_get_adjustment(GTK_RANGE(widget));
	double adjvalue = gtk_adjustment_get_value(adj);
	double adjupper = gtk_adjustment_get_upper(adj);
	double adjlower = gtk_adjustment_get_lower(adj);
	double adjstep = gtk_adjustment_get_step_increment(adj);

	int oldstep = (int)(0.5f + (adjvalue - adjlower) / adjstep);
	int step;
	int nsteps = (int)(0.5f + (adjupper - adjlower) / adjstep);
	if (dir_down)
		step = oldstep - 1;
	else
		step = oldstep + 1;
	double value = adjlower + step * double(adjupper - adjlower) / nsteps;
	gtk_widget_grab_focus(widget);
	gtk_range_set_value(GTK_RANGE(widget), value);
}

/****************************************************************
 ** keyboard bindings
 */

static gboolean gtk_knob_key_press (GtkWidget *widget, GdkEventKey *event)
{
	g_assert(GTK_IS_KNOB(widget));

	GtkAdjustment *adj = gtk_range_get_adjustment(GTK_RANGE(widget));
	switch (event->keyval) {
	case GDK_KEY_Home:
		gtk_range_set_value(GTK_RANGE(widget), gtk_adjustment_get_lower(adj));
		return TRUE;
	case GDK_KEY_End:
		gtk_range_set_value(GTK_RANGE(widget), gtk_adjustment_get_upper(adj));
		return TRUE;
	case GDK_KEY_Up:
		gtk_knob_set_value(widget, 0);
		return TRUE;
	case GDK_KEY_Right:
		gtk_knob_set_value(widget, 0);
		return TRUE;
	case GDK_KEY_Down:
		gtk_knob_set_value(widget, 1);
		return TRUE;
	case GDK_KEY_Left:
		gtk_knob_set_value(widget, 1);
		return TRUE;
	}

	return FALSE;
}

/****************************************************************
 ** alternative (radial) knob motion mode (ctrl + mouse pressed)
 */

static void knob_pointer_event(GtkWidget *widget, gdouble x, gdouble y, int knob_x, int knob_y,
                               bool drag, int state)
{
	static double last_y = 2e20;
	GtkKnob *knob = GTK_KNOB(widget);
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);
	GtkAdjustment *adj = gtk_range_get_adjustment(GTK_RANGE(widget));
	double adjvalue = gtk_adjustment_get_value(adj);
	double adjupper = gtk_adjustment_get_upper(adj);
	double adjlower = gtk_adjustment_get_lower(adj);
	double radius =  MIN(knob_x, knob_y) / 2;
	int  knobx = (allocation.width - knob_x) / 2;
	int  knoby = (allocation.height - knob_y) / 2;
	double posx = (knobx + radius) - x; // x axis right -> left
	double posy = (knoby + radius) - y; // y axis top -> bottom
	double value;
	if (!drag) {
		if (state & GDK_CONTROL_MASK) {
			last_y = 2e20;
			return;
		} else {
			last_y = posy;
		}
	}
	if (last_y < 1e20) { // in drag started with Control Key
		const double scaling = 0.005;
		double scal = (state & GDK_SHIFT_MASK ? scaling*0.1 : scaling);
		value = (last_y - posy) * scal;
		last_y = posy;
		gtk_range_set_value(GTK_RANGE(widget), adjvalue - value * (adjupper - adjlower));
		return;
	}

	double angle = atan2(-posx, posy) + M_PI; // clockwise, zero at 6 o'clock, 0 .. 2*M_PI
	if (drag) {
		// block "forbidden zone" and direct moves between quadrant 1 and 4
		int quadrant = 1 + int(angle/M_PI_2);
		if (knob->last_quadrant == 1 && (quadrant == 3 || quadrant == 4)) {
			angle = scale_zero;
		} else if (knob->last_quadrant == 4 && (quadrant == 1 || quadrant == 2)) {
			angle = 2*M_PI - scale_zero;
		} else {
			if (angle < scale_zero) {
				angle = scale_zero;
			} else if (angle > 2*M_PI - scale_zero) {
				angle = 2*M_PI - scale_zero;
			}
			knob->last_quadrant = quadrant;
		}
	} else {
		if (angle < scale_zero) {
			angle = scale_zero;
		} else if (angle > 2*M_PI - scale_zero) {
			angle = 2*M_PI - scale_zero;
		}
		knob->last_quadrant = 0;
	}
	angle = (angle - scale_zero) / (2 * (M_PI-scale_zero)); // normalize to 0..1
	gtk_range_set_value(GTK_RANGE(widget), adjlower + angle * (adjupper - adjlower));
}

/****************************************************************
 ** mouse button pressed set value
 */

static gboolean gtk_knob_button_press (GtkWidget *widget, GdkEventButton *event)
{
	g_assert(GTK_IS_KNOB(widget));
	
	GtkKnobClass *klass =  GTK_KNOB_CLASS(GTK_OBJECT_GET_CLASS(widget));
	

	switch (event->button) {
	case 1:  // left button
		gtk_widget_grab_focus(widget);
		gtk_widget_grab_default (widget);
		gtk_grab_add(widget);
		klass->button_is = 1;
		knob_pointer_event(widget, event->x, event->y, klass->knob_x, klass->knob_y,
						   false, event->state);
		break;
	case 2: //wheel
		klass->button_is = 2;
		break;
	case 3:  // right button 
		klass->button_is = 3;
		break;
	default: // do nothing
		break;
	}
	return TRUE;
}

/****************************************************************
 ** mouse button release
 */

static gboolean gtk_knob_button_release (GtkWidget *widget, GdkEventButton *event)
{
	g_assert(GTK_IS_KNOB(widget));
	GTK_KNOB_CLASS(GTK_OBJECT_GET_CLASS(widget))->button_is = 0;
	if (gtk_widget_has_grab(widget))
		gtk_grab_remove(widget);
	return FALSE;
}

/****************************************************************
 ** set the value from mouse movement
 */

static gboolean gtk_knob_pointer_motion (GtkWidget *widget, GdkEventMotion *event)
{
	g_assert(GTK_IS_KNOB(widget));
	GtkKnobClass *klass =  GTK_KNOB_CLASS(GTK_OBJECT_GET_CLASS(widget));
	
	gdk_event_request_motions (event);
	
	if (gtk_widget_has_grab(widget)) {
		knob_pointer_event(widget, event->x, event->y, klass->knob_x, klass->knob_y,
						   true, event->state);
	}
	return FALSE;
}

/****************************************************************
 ** set value from mouseweel
 */

static gboolean gtk_knob_scroll (GtkWidget *widget, GdkEventScroll *event)
{
	usleep(5000);
	gtk_knob_set_value(widget, event->direction);
	return FALSE;
}

/****************************************************************
 ** init the GtkKnobClass
 */

static void gtk_knob_class_init (GtkKnobClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

	/** set here the sizes and steps for the used knob **/
//--------- small knob size and steps
	
	klass->knob_x = 30;
	klass->knob_y = 30;
	klass->knob_step = 86;

//--------- event button
	klass->button_is = 0;

//--------- connect the events with funktions
	widget_class->expose_event = gtk_knob_expose;
	widget_class->size_request = gtk_knob_size_request;
	widget_class->button_press_event = gtk_knob_button_press;
	widget_class->button_release_event = gtk_knob_button_release;
	widget_class->motion_notify_event = gtk_knob_pointer_motion;
	widget_class->key_press_event = gtk_knob_key_press;
	widget_class->scroll_event = gtk_knob_scroll;
}

/****************************************************************
 ** init the Knob type/size
 */

static void gtk_knob_init (GtkKnob *knob)
{
	GtkWidget *widget = GTK_WIDGET(knob);

	gtk_widget_set_can_focus(GTK_WIDGET(knob), TRUE);
	gtk_widget_set_can_default(GTK_WIDGET(knob), TRUE);
}

/****************************************************************
 ** redraw when value changed
 */

static gboolean gtk_knob_value_changed(gpointer obj)
{
	GtkWidget *widget = (GtkWidget *)obj;
	gtk_widget_queue_draw(widget);
	return FALSE;
}

/****************************************************************
 ** create small knob
 */

GtkWidget *GtkKnob::gtk_knob_new_with_adjustment(GtkAdjustment *_adjustment)
{
	GtkWidget *widget = GTK_WIDGET( g_object_new (GTK_TYPE_KNOB, NULL ));
	GtkKnob *knob = GTK_KNOB(widget);
	knob->last_quadrant = 0;
	if (widget) {
		gtk_range_set_adjustment(GTK_RANGE(widget), _adjustment);
		g_signal_connect(GTK_OBJECT(widget), "value-changed",
		                 G_CALLBACK(gtk_knob_value_changed), widget);
	}
	return widget;
}

/****************************************************************
 ** get the Knob type
 */

GType gtk_knob_get_type (void)
{
	static GType kn_type = 0;
	if (!kn_type) {
		static const GTypeInfo kn_info = {
			sizeof(GtkKnobClass), NULL,  NULL, (GClassInitFunc)gtk_knob_class_init, NULL, NULL, sizeof (GtkKnob), 0, (GInstanceInitFunc)gtk_knob_init
		};
		kn_type = g_type_register_static(GTK_TYPE_RANGE,  "GtkKnob", &kn_info, (GTypeFlags)0);
	}
	return kn_type;
}

}/* end of gtk_knob namespace */
