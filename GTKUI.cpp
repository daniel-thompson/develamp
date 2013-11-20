/*
 * GTKUI.cpp
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

#include "GUI.h"
#include "GTKUI.h"

/******************************************************************************
*******************************************************************************

                                GRAPHIC USER INTERFACE
                                  gtk interface

*******************************************************************************
*******************************************************************************/
#include <string>
#include <set>
#include <iostream>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <assert.h>

#include "gtk_knob.h"

using namespace std;

// Insertion modes

#define kSingleMode 0
#define kBoxMode 1
#define kTabMode 2

//------------ calculate needed precision
static int precision(double n)
{
	if (n < 0.009999) return 3;
	else if (n < 0.099999) return 2;
	else if (n < 0.999999) return 1;
	else return 0;
}

static gtk_knob::GtkKnob myGtkKnob;

/**
 * rmWhiteSpaces(): Remove the leading and trailing white spaces of a string
 * (but not those in the middle of the string)
 */
static string rmWhiteSpaces(const string& s)
{
    size_t i = s.find_first_not_of(" \t");
    size_t j = s.find_last_not_of(" \t");

    if (i != string::npos && j != string::npos) {
        return s.substr(i, 1+j-i);
    } else {
        return "";
    }
}


/**
 * Extracts metdata from a label : 'vol [unit: dB]' -> 'vol' + metadata
 */
static void extractMetadata(const string& fulllabel, string& label, map<string, string>& metadata)
{
    enum {kLabel, kEscape1, kEscape2, kEscape3, kKey, kValue};
    int state = kLabel; int deep = 0;
    string key, value;

    for (unsigned int i=0; i < fulllabel.size(); i++) {
        char c = fulllabel[i];
        switch (state) {
            case kLabel :
                assert (deep == 0);
                switch (c) {
                    case '\\' : state = kEscape1; break;
                    case '[' : state = kKey; deep++; break;
                    default : label += c;
                }
                break;

            case kEscape1 :
                label += c;
                state = kLabel;
                break;

            case kEscape2 :
                key += c;
                state = kKey;
                break;

            case kEscape3 :
                value += c;
                state = kValue;
                break;

            case kKey :
                assert (deep > 0);
                switch (c) {
                    case '\\' :  state = kEscape2;
                                break;

                    case '[' :  deep++;
                                key += c;
                                break;

                    case ':' :  if (deep == 1) {
                                    state = kValue;
                                } else {
                                    key += c;
                                }
                                break;
                    case ']' :  deep--;
                                if (deep < 1) {
                                    metadata[rmWhiteSpaces(key)] = "";
                                    state = kLabel;
                                    key="";
                                    value="";
                                } else {
                                    key += c;
                                }
                                break;
                    default :   key += c;
                }
                break;

            case kValue :
                assert (deep > 0);
                switch (c) {
                    case '\\' : state = kEscape3;
                                break;

                    case '[' :  deep++;
                                value += c;
                                break;

                    case ']' :  deep--;
                                if (deep < 1) {
                                    metadata[rmWhiteSpaces(key)]=rmWhiteSpaces(value);
                                    state = kLabel;
                                    key="";
                                    value="";
                                } else {
                                    value += c;
                                }
                                break;
                    default :   value += c;
                }
                break;

            default :
                cerr << "ERROR unrecognized state " << state << endl;
        }
    }
    label = rmWhiteSpaces(label);
}

// global static fields

bool                        GTKUI::fInitialized = false;
map<float*, float>          GTKUI::fGuiSize;
map<float*, string>         GTKUI::fTooltip;
set<float*>                 GTKUI::fKnobSet;       // set of widget zone to be knobs

/**
* Format tooltip string by replacing some white spaces by 
* return characters so that line width doesn't exceed n.
* Limitation : long words exceeding n are not cut 
*/
static string formatTooltip(unsigned int n, const string& tt)
{
	string  ss = tt;	// ss string we are going to format
	unsigned int	lws = 0;	// last white space encountered
	unsigned int 	lri = 0;	// last return inserted
	for (unsigned int i=0; i<tt.size(); i++) {
		if (tt[i] == ' ') lws = i;
		if (((i-lri) >= n) && (lws > lri)) {
			// insert return here
			ss[lws] = '\n';
			lri = lws;
		}
	}
	cout << ss;
	return ss;
}



static gint delete_event( GtkWidget *widget, GdkEvent *event, gpointer data )
{
    return FALSE; 
}

static void destroy_event( GtkWidget *widget, gpointer data )
{
    gtk_main_quit ();
}

         
GTKUI::GTKUI(char * name, int* pargc, char*** pargv) 
{
    if (!fInitialized) {
        gtk_init(pargc, pargv);
        fInitialized = true;
    }
    
    fWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    //gtk_container_set_border_width (GTK_CONTAINER (fWindow), 10);
    gtk_window_set_title (GTK_WINDOW (fWindow), name);
    gtk_signal_connect (GTK_OBJECT (fWindow), "delete_event", GTK_SIGNAL_FUNC (delete_event), NULL);
    gtk_signal_connect (GTK_OBJECT (fWindow), "destroy", GTK_SIGNAL_FUNC (destroy_event), NULL);

    fTop = 0;
    fBox[fTop] = gtk_vbox_new (homogene, 4);
    fMode[fTop] = kBoxMode;
    gtk_container_add (GTK_CONTAINER (fWindow), fBox[fTop]);
    fStopped = false;
}

// empilement des boites

void GTKUI::pushBox(int mode, GtkWidget* w)
{
    ++fTop;
    assert(fTop < stackSize);
    fMode[fTop]     = mode;
    fBox[fTop]      = w;
}


/**
 * Remove n levels from the stack S before the top level
 * adjustStack(n): S -> S' with S' = S(0),S(n+1),S(n+2),...
 */
void GTKUI::adjustStack(int n)
{
    if (n > 0) {
        assert(fTop >= n);

        fTop -= n; 
        fMode[fTop] = fMode[fTop+n];
        fBox[fTop]  = fBox[fTop+n];
    }
}

void GTKUI::closeBox()
{
    --fTop;
    assert(fTop >= 0);
}


/**
 * Analyses the widget zone metadata declarations and takes
 * appropriate actions 
 */
void GTKUI::declare(float* zone, const char* key, const char* value)
{
	if (zone == 0) {
		// special zone 0 means group metadata
		if (strcmp(key,"tooltip")==0) {
			// only group tooltip are currently implemented
			gGroupTooltip = formatTooltip(30, value);
		}
	} else {
		if (strcmp(key,"size")==0) {
			fGuiSize[zone]=atof(value);
		}
		else if (strcmp(key,"tooltip")==0) {
			fTooltip[zone] = formatTooltip(30,value) ;
		}
		else if (strcmp(key,"style")==0) {
			if (strcmp(value,"knob") == 0) {
				fKnobSet.insert(zone);
			}
		}
	}
}
        
        

/**
 * Analyses a full label and activates the relevant options. returns a simplified
 * label (without options) and an amount of stack adjustement (in case additional
 * containers were pushed on the stack). 
 */

int GTKUI::checkLabelOptions(GtkWidget* widget, const string& fullLabel, string& simplifiedLabel)
{   
    map<string, string> metadata;
    extractMetadata(fullLabel, simplifiedLabel, metadata);

    if (metadata.count("tooltip")) {
        gtk_tooltips_set_tip (gtk_tooltips_new (), widget, metadata["tooltip"].c_str(), NULL);
    }
    if (metadata["option"] == "detachable") {
        openHandleBox(simplifiedLabel.c_str());
        return 1;
    }

	//---------------------
	if (gGroupTooltip != string()) {
		gtk_tooltips_set_tip (gtk_tooltips_new (), widget, gGroupTooltip.c_str(), NULL);
		gGroupTooltip = string();
	}
	
	//----------------------
	
    // no adjustement of the stack needed
    return 0;
}

/**
 * Check if a tooltip is associated to a zone and add it to the corresponding widget
 */
void GTKUI::checkForTooltip(float* zone, GtkWidget* widget)
{
    if (fTooltip.count(zone)) {
        gtk_tooltips_set_tip (gtk_tooltips_new (), widget, fTooltip[zone].c_str(), NULL);
    }
}


// les differentes boites

void GTKUI::openFrameBox(const char* label)
{
    GtkWidget * box = gtk_frame_new (label);
    //gtk_container_set_border_width (GTK_CONTAINER (box), 10);
            
    pushBox(kSingleMode, addWidget(label, box));
}


void GTKUI::openTabBox(const char* fullLabel)
{
    string  label;
    GtkWidget* widget = gtk_notebook_new();

    int     adjust = checkLabelOptions(widget, fullLabel, label);

    pushBox(kTabMode, addWidget(label.c_str(), widget));

    // adjust stack because otherwise Handlebox will remain open
    adjustStack(adjust);
}


void GTKUI::openHorizontalBox(const char* fullLabel)
{   
    string   label;
    GtkWidget* box = gtk_hbox_new (homogene, 4);
    int     adjust = checkLabelOptions(box, fullLabel, label);

    gtk_container_set_border_width (GTK_CONTAINER (box), 10);
            
    if (fMode[fTop] != kTabMode && label[0] != 0) {
        GtkWidget * frame = addWidget(label.c_str(), gtk_frame_new (label.c_str()));
        gtk_container_add (GTK_CONTAINER(frame), box);
        gtk_widget_show(box);
        pushBox(kBoxMode, box);
    } else {
        pushBox(kBoxMode, addWidget(label.c_str(), box));
    }

    // adjust stack because otherwise Handlebox will remain open
    adjustStack(adjust);
}


void GTKUI::openVerticalBox(const char* fullLabel)
{
    string  label;
    GtkWidget * box = gtk_vbox_new (homogene, 4);
    int      adjust = checkLabelOptions(box, fullLabel, label);

    gtk_container_set_border_width (GTK_CONTAINER (box), 10);
            
    if (fMode[fTop] != kTabMode && label[0] != 0) {
        GtkWidget * frame = addWidget(label.c_str(), gtk_frame_new (label.c_str()));
        gtk_container_add (GTK_CONTAINER(frame), box);
        gtk_widget_show(box);
        pushBox(kBoxMode, box);
    } else {
        pushBox(kBoxMode, addWidget(label.c_str(), box));
    }

    // adjust stack because otherwise Handlebox will remain open
    adjustStack(adjust);
}


void GTKUI::openHandleBox(const char* label)
{
    GtkWidget * box = gtk_hbox_new (homogene, 4);
    gtk_container_set_border_width (GTK_CONTAINER (box), 2);
    if (fMode[fTop] != kTabMode && label[0] != 0)
    {
        GtkWidget * frame = addWidget(label, gtk_handle_box_new ());
        gtk_container_add (GTK_CONTAINER(frame), box);
        gtk_widget_show(box);
        pushBox(kBoxMode, box);
    }
    else
    {
        pushBox(kBoxMode, addWidget(label, box));
    }
}


void GTKUI::openEventBox(const char* label)
{
    GtkWidget * box = gtk_hbox_new (homogene, 4);
    gtk_container_set_border_width (GTK_CONTAINER (box), 2);
    if (fMode[fTop] != kTabMode && label[0] != 0)
    {
        GtkWidget * frame = addWidget(label, gtk_event_box_new ());
        gtk_container_add (GTK_CONTAINER(frame), box);
        gtk_widget_show(box);
        pushBox(kBoxMode, box);
    }
    else
    {
        pushBox(kBoxMode, addWidget(label, box));
    }
}


struct uiExpanderBox : public uiItem
{
    GtkExpander* fButton;
    uiExpanderBox(GUI* ui, float* zone, GtkExpander* b) : uiItem(ui, zone), fButton(b) {}
    static void expanded (GtkWidget *widget, gpointer data)
    {
        float   v = gtk_expander_get_expanded  (GTK_EXPANDER(widget));
        if (v == 1.000000)
        {
            v = 0;
        }
        else v = 1;
        ((uiItem*)data)->modifyZone(v);
    }

    virtual void reflectZone()
    {
        float   v = *fZone;
        fCache = v;
        gtk_expander_set_expanded(GTK_EXPANDER(fButton), v);
    }
};

void GTKUI::openExpanderBox(const char* label, float* zone)
{
    *zone = 0.0;
    GtkWidget * box = gtk_hbox_new (homogene, 4);
    gtk_container_set_border_width (GTK_CONTAINER (box), 2);
    if (fMode[fTop] != kTabMode && label[0] != 0)
    {
        GtkWidget * frame = addWidget(label, gtk_expander_new (label));
        gtk_container_add (GTK_CONTAINER(frame), box);
        uiExpanderBox* c = new uiExpanderBox(this, zone, GTK_EXPANDER(frame));
        gtk_signal_connect (GTK_OBJECT (frame), "activate", GTK_SIGNAL_FUNC (uiExpanderBox::expanded), (gpointer)c);
        gtk_widget_show(box);
        pushBox(kBoxMode, box);
    }
    else
    {
        pushBox(kBoxMode, addWidget(label, box));
    }
}



GtkWidget* GTKUI::addWidget(const char* label, GtkWidget* w)
{ 
    switch (fMode[fTop]) {
        case kSingleMode    : gtk_container_add (GTK_CONTAINER(fBox[fTop]), w);                             break;
        case kBoxMode       : gtk_box_pack_start (GTK_BOX(fBox[fTop]), w, expand, fill, 0);                 break;
        case kTabMode       : gtk_notebook_append_page (GTK_NOTEBOOK(fBox[fTop]), w, gtk_label_new(label)); break;
    }
    gtk_widget_show (w);
    return w;
}

// --------------------------- Press button ---------------------------

struct uiButton : public uiItem
{
    GtkButton*  fButton;
    
    uiButton (GUI* ui, float* zone, GtkButton* b) : uiItem(ui, zone), fButton(b) {}
    
    static void pressed( GtkWidget *widget, gpointer   data )
    {
        uiItem* c = (uiItem*) data;
        c->modifyZone(1.0);
    }

    static void released( GtkWidget *widget, gpointer   data )
    {
        uiItem* c = (uiItem*) data;
        c->modifyZone(0.0);
    }

    virtual void reflectZone()  
    { 
        float   v = *fZone;
        fCache = v; 
        if (v > 0.0) gtk_button_pressed(fButton); else gtk_button_released(fButton);
    }
};

void GTKUI::addButton(const char* label, float* zone)
{
    *zone = 0.0;
    GtkWidget*  button = gtk_button_new_with_label (label);
    addWidget(label, button);
    
    uiButton* c = new uiButton(this, zone, GTK_BUTTON(button));
    
    gtk_signal_connect (GTK_OBJECT (button), "pressed", GTK_SIGNAL_FUNC (uiButton::pressed), (gpointer) c);
    gtk_signal_connect (GTK_OBJECT (button), "released", GTK_SIGNAL_FUNC (uiButton::released), (gpointer) c);

    checkForTooltip(zone, button);
}

// ---------------------------  Toggle Buttons ---------------------------

struct uiToggleButton : public uiItem
{
    GtkToggleButton* fButton;
    
    uiToggleButton(GUI* ui, float* zone, GtkToggleButton* b) : uiItem(ui, zone), fButton(b) {}
    
    static void toggled (GtkWidget *widget, gpointer data)
    {
        float   v = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget)) ? 1.0 : 0.0;
        ((uiItem*)data)->modifyZone(v);
    }

    virtual void reflectZone()  
    { 
        float   v = *fZone;
        fCache = v; 
        gtk_toggle_button_set_active(fButton, v > 0.0); 
    }
};

void GTKUI::addToggleButton(const char* label, float* zone)
{
    *zone = 0.0;
    GtkWidget*  button = gtk_toggle_button_new_with_label (label);
    addWidget(label, button);
    
    uiToggleButton* c = new uiToggleButton(this, zone, GTK_TOGGLE_BUTTON(button));
    gtk_signal_connect (GTK_OBJECT (button), "toggled", GTK_SIGNAL_FUNC (uiToggleButton::toggled), (gpointer) c);

    checkForTooltip(zone, button);
}



void show_dialog(GtkWidget *widget, gpointer data)
{
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)) == TRUE)
    {
        gtk_widget_show(GTK_WIDGET(data));
        gint root_x, root_y;
        gtk_window_get_position (GTK_WINDOW(data), &root_x, &root_y);
        root_y -= 120;
        gtk_window_move(GTK_WINDOW(data), root_x, root_y);
    }
    else gtk_widget_hide(GTK_WIDGET(data));
}

static gboolean deleteevent( GtkWidget *widget, gpointer   data )
{
return TRUE;
} 

void GTKUI::openDialogBox(const char* label, float* zone)
{
    // create toplevel window and set properties
    GtkWidget * dialog = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_deletable(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_gravity(GTK_WINDOW(dialog), GDK_GRAVITY_SOUTH);
    gtk_window_set_transient_for (GTK_WINDOW(dialog), GTK_WINDOW(fWindow));
    gtk_window_set_position (GTK_WINDOW(dialog), GTK_WIN_POS_MOUSE);
    gtk_window_set_keep_below (GTK_WINDOW(dialog), FALSE);
    gtk_window_set_title (GTK_WINDOW (dialog), label);
    g_signal_connect (G_OBJECT (dialog), "delete_event", G_CALLBACK (deleteevent), NULL); 
    gtk_window_set_destroy_with_parent(GTK_WINDOW(dialog), TRUE);

    GtkWidget * box = gtk_hbox_new (homogene, 4);
 
    *zone = 0.0;
    GtkWidget*  button = gtk_toggle_button_new ();
    gtk_signal_connect (GTK_OBJECT (button), "toggled", GTK_SIGNAL_FUNC (show_dialog), (gpointer) dialog);
 
    gtk_container_add (GTK_CONTAINER(fBox[fTop]), button);
    gtk_container_add (GTK_CONTAINER(dialog), box);
    gtk_widget_show (button);
    gtk_widget_show(box);
    pushBox(kBoxMode, box);
}




// ---------------------------  Check Button ---------------------------

struct uiCheckButton : public uiItem
{
    GtkToggleButton* fButton;
    
    uiCheckButton(GUI* ui, float* zone, GtkToggleButton* b) : uiItem(ui, zone), fButton(b) {}
    
    static void toggled (GtkWidget *widget, gpointer data)
    {
        float   v = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget)) ? 1.0 : 0.0;
        ((uiItem*)data)->modifyZone(v);
    }

    virtual void reflectZone()  
    { 
        float   v = *fZone;
        fCache = v; 
        gtk_toggle_button_set_active(fButton, v > 0.0); 
    }
};

void GTKUI::addCheckButton(const char* label, float* zone)
{
    *zone = 0.0;
    GtkWidget*  button = gtk_check_button_new_with_label (label);
    addWidget(label, button);
    
    uiCheckButton* c = new uiCheckButton(this, zone, GTK_TOGGLE_BUTTON(button));
    gtk_signal_connect (GTK_OBJECT (button), "toggled", GTK_SIGNAL_FUNC(uiCheckButton::toggled), (gpointer) c);

    checkForTooltip(zone, button);
}


// ---------------------------  Adjustmenty based widgets ---------------------------

struct uiAdjustment : public uiItem
{
    GtkAdjustment* fAdj;
    
    uiAdjustment(GUI* ui, float* zone, GtkAdjustment* adj) : uiItem(ui, zone), fAdj(adj) {}
    
    static void changed (GtkWidget *widget, gpointer data)
    {
        float   v = gtk_adjustment_get_value(GTK_ADJUSTMENT (widget));
        ((uiItem*)data)->modifyZone(v);
    }

    virtual void reflectZone()  
    { 
        float   v = *fZone;
        fCache = v; 
        gtk_adjustment_set_value(fAdj, v);  
    }
};

// --------------------------- format knob value display ---------------------------

struct uiValueDisplay : public uiItem
{
	GtkLabel* fLabel;
	int	fPrecision ;

	uiValueDisplay(GUI* ui, float* zone, GtkLabel* label, int precision)
		: uiItem(ui, zone), fLabel(label), fPrecision(precision) {}

	virtual void reflectZone()
		{
			float v = *fZone;
			fCache = v;
			char s[64];
			if (fPrecision <= 0)
				snprintf(s, 63, "%d", int(v));

			else if (fPrecision > 3)
				snprintf(s, 63, "%f", v);

			else if (fPrecision == 1)
			{
				const char* format[] = {"%.1f", "%.2f", "%.3f"};
				snprintf(s, 63, format[1-1], v);
			}
			else if (fPrecision == 2)
			{
				const char* format[] = {"%.1f", "%.2f", "%.3f"};
				snprintf(s, 63, format[2-1], v);
			}
			else
			{
				const char* format[] = {"%.1f", "%.2f", "%.3f"};
				snprintf(s, 63, format[3-1], v);
			}
			gtk_label_set_text(fLabel, s);
		}
};

// ------------------------------- Knob -----------------------------------------

void GTKUI::addKnob(const char* label, float* zone, float init, float min, float max, float step)
{
	*zone = init;
    GtkObject* adj = gtk_adjustment_new(init, min, max, step, 10*step, 0);
    
    uiAdjustment* c = new uiAdjustment(this, zone, GTK_ADJUSTMENT(adj));

    gtk_signal_connect (GTK_OBJECT (adj), "value-changed", GTK_SIGNAL_FUNC (uiAdjustment::changed), (gpointer) c);
    
	GtkWidget* slider = gtk_vbox_new (FALSE, 0);
	GtkWidget* fil = gtk_vbox_new (FALSE, 0);
	GtkWidget* rei = gtk_vbox_new (FALSE, 0);
	GtkWidget* re =myGtkKnob.gtk_knob_new_with_adjustment(GTK_ADJUSTMENT(adj));
	GtkWidget* lw = gtk_label_new("");
	new uiValueDisplay(this, zone, GTK_LABEL(lw),precision(step));
	gtk_container_add (GTK_CONTAINER(rei), re);
	if(fGuiSize[zone]) {
		float size = 30 * fGuiSize[zone];
		gtk_widget_set_size_request(rei, size, size );
		gtk_box_pack_start (GTK_BOX(slider), fil, TRUE, TRUE, 0);
		gtk_box_pack_start (GTK_BOX(slider), rei, FALSE, FALSE, 0);
	} else {
		gtk_container_add (GTK_CONTAINER(slider), fil);
		gtk_container_add (GTK_CONTAINER(slider), rei);
	}
	gtk_container_add (GTK_CONTAINER(slider), lw);
	gtk_widget_show_all(slider);
	
	if (label && label[0]!=0) {
        openFrameBox(label);
        addWidget(label, slider);
        closeBox();
    } else {
        addWidget(label, slider);
    }

    checkForTooltip(zone, slider);
}

// -------------------------- Vertical Slider -----------------------------------

void GTKUI::addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step)
{
	if (isKnob(zone)) { 
		addKnob(label, zone, init, min, max, step);
		return;
	} 
    *zone = init;
    GtkObject* adj = gtk_adjustment_new(init, min, max, step, 10*step, 0);
    
    uiAdjustment* c = new uiAdjustment(this, zone, GTK_ADJUSTMENT(adj));

    gtk_signal_connect (GTK_OBJECT (adj), "value-changed", GTK_SIGNAL_FUNC (uiAdjustment::changed), (gpointer) c);
    
	GtkWidget* slider = gtk_vscale_new (GTK_ADJUSTMENT(adj));
	gtk_scale_set_digits(GTK_SCALE(slider), precision(step));
	float size = 160;
	if(fGuiSize[zone]) {
		size = 160 * fGuiSize[zone];
	}
	gtk_widget_set_size_request(slider, -1, size);
	
    gtk_range_set_inverted (GTK_RANGE(slider), TRUE);
    
    if (label && label[0]!=0) {
        openFrameBox(label);
        addWidget(label, slider);
        closeBox();
    } else {
        addWidget(label, slider);
    }

    checkForTooltip(zone, slider);
}

// -------------------------- Horizontal Slider -----------------------------------

void GTKUI::addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step)
{
	if (isKnob(zone)) { 
		addKnob(label, zone, init, min, max, step);
		return;
	} 
    *zone = init;
    GtkObject* adj = gtk_adjustment_new(init, min, max, step, 10*step, 0);
    
    uiAdjustment* c = new uiAdjustment(this, zone, GTK_ADJUSTMENT(adj));

    gtk_signal_connect (GTK_OBJECT (adj), "value-changed", GTK_SIGNAL_FUNC (uiAdjustment::changed), (gpointer) c);
    
    GtkWidget* slider = gtk_hscale_new (GTK_ADJUSTMENT(adj));
	gtk_scale_set_digits(GTK_SCALE(slider), precision(step));
	float size = 160;
	if(fGuiSize[zone]) {
		size = 160 * fGuiSize[zone];
	}
	gtk_widget_set_size_request(slider, size, -1);
    
    if (label && label[0]!=0) {
        openFrameBox(label);
        addWidget(label, slider);
        closeBox();
    } else {
        addWidget(label, slider);
    }             

    checkForTooltip(zone, slider);
}


// ------------------------------ Num Entry -----------------------------------

void GTKUI::addNumEntry(const char* label, float* zone, float init, float min, float max, float step)
{
	if (isKnob(zone)) { 
		addKnob(label, zone, init, min, max, step);
		return;
	} 
    *zone = init;
    GtkObject* adj = gtk_adjustment_new(init, min, max, step, 10*step, step);
    
    uiAdjustment* c = new uiAdjustment(this, zone, GTK_ADJUSTMENT(adj));

    gtk_signal_connect (GTK_OBJECT (adj), "value-changed", GTK_SIGNAL_FUNC (uiAdjustment::changed), (gpointer) c);
    
    GtkWidget* spinner = gtk_spin_button_new (GTK_ADJUSTMENT(adj), 0.005, precision(step));

    openFrameBox(label);
    addWidget(label, spinner);
    closeBox();

    checkForTooltip(zone, spinner);
}


// ==========================   passive widgets ===============================


// ------------------------------ Progress Bar -----------------------------------

struct uiBargraph : public uiItem
{
    GtkProgressBar*     fProgressBar;
    float               fMin;
    float               fMax;
    
    uiBargraph(GUI* ui, float* zone, GtkProgressBar* pbar, float lo, float hi) 
            : uiItem(ui, zone), fProgressBar(pbar), fMin(lo), fMax(hi) {}

    float scale(float v)        { return (v-fMin)/(fMax-fMin); }
    
    virtual void reflectZone()  
    { 
        float   v = *fZone;
        fCache = v; 
        gtk_progress_bar_set_fraction(fProgressBar, scale(v));  
    }
};

    

void GTKUI::addVerticalBargraph(const char* label, float* zone, float lo, float hi)
{
    GtkWidget* pb = gtk_progress_bar_new();
    gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(pb), GTK_PROGRESS_BOTTOM_TO_TOP);
    gtk_widget_set_size_request(pb, 8, -1);
    new uiBargraph(this, zone, GTK_PROGRESS_BAR(pb), lo, hi);
    openFrameBox(label);
    addWidget(label, pb);
    closeBox();

    checkForTooltip(zone, pb);
}
    

void GTKUI::addHorizontalBargraph(const char* label, float* zone, float lo, float hi)
{
    GtkWidget* pb = gtk_progress_bar_new();
    gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(pb), GTK_PROGRESS_LEFT_TO_RIGHT);
    gtk_widget_set_size_request(pb, -1, 8);
    new uiBargraph(this, zone, GTK_PROGRESS_BAR(pb), lo, hi);
    openFrameBox(label);
    addWidget(label, pb);
    closeBox();

    checkForTooltip(zone, pb);
}


// ------------------------------ Num Display -----------------------------------

struct uiNumDisplay : public uiItem
{
    GtkLabel* fLabel;
    int fPrecision;
    
    uiNumDisplay(GUI* ui, float* zone, GtkLabel* label, int precision) 
            : uiItem(ui, zone), fLabel(label), fPrecision(precision) {}

    virtual void reflectZone()  
    { 
        float   v = *fZone;
        fCache = v;
        char s[64]; 
        if (fPrecision <= 0) { 
            snprintf(s, 63, "%d", int(v)); 
        } else if (fPrecision>3) {
            snprintf(s, 63, "%f", v);
        } else {
            const char* format[] = {"%.1f", "%.2f", "%.3f"};
            snprintf(s, 63, format[fPrecision-1], v);
        }
        gtk_label_set_text(fLabel, s);
    }
};
    

void GTKUI::addNumDisplay(const char* label, float* zone, int precision )
{
    GtkWidget* lw = gtk_label_new("");
    new uiNumDisplay(this, zone, GTK_LABEL(lw), precision);
    openFrameBox(label);
    addWidget(label, lw);
    closeBox();

    checkForTooltip(zone, lw);
}


// ------------------------------ Text Display -----------------------------------

struct uiTextDisplay : public uiItem
{
        GtkLabel*           fLabel;
        const char**    fNames;
        float               fMin;
        float               fMax;
        int                         fNum;
        
        
        uiTextDisplay (GUI* ui, float* zone, GtkLabel* label, const char* names[], float lo, float hi)
                        : uiItem(ui, zone), fLabel(label), fNames(names), fMin(lo), fMax(hi)
        {
                fNum = 0;
                while (fNames[fNum] != 0) fNum++;
    }

    virtual void reflectZone()  
    { 
        float   v = *fZone;
        fCache = v;
        
        int idx = int(fNum*(v-fMin)/(fMax-fMin));
        
        if      (idx < 0)       idx = 0; 
        else if (idx >= fNum)   idx = fNum-1;
                
        gtk_label_set_text(fLabel, fNames[idx]); 
    }
};
    

void GTKUI::addTextDisplay(const char* label, float* zone, const char* names[], float lo, float hi )
{
    GtkWidget* lw = gtk_label_new("");
    new uiTextDisplay (this, zone, GTK_LABEL(lw), names, lo, hi);
    openFrameBox(label);
    addWidget(label, lw);
    closeBox();

    checkForTooltip(zone, lw);
}



void GTKUI::show() 
{
    assert(fTop == 0);
    gtk_widget_show  (fBox[0]);
    gtk_widget_show  (fWindow);
}


/**
 * Update all user items reflecting zone z
 */
    
static gboolean callUpdateAllGuis(gpointer)
{ 
    GUI::updateAllGuis(); 
    return TRUE;
}


void GTKUI::run() 
{
    assert(fTop == 0);
    gtk_widget_show  (fBox[0]);
    gtk_widget_show  (fWindow);
    gtk_timeout_add(40, callUpdateAllGuis, 0);
    gtk_main ();
    stop();
}
