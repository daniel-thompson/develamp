/*
 * FUI.h
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

#ifndef DEVELAMP_FUI_H_
#define DEVELAMP_FUI_H_

#include "UI.h"

#include <string>
#include <map>
#include <set>
#include <vector>
#include <stack>

#include <iostream>
#include <fstream>

/*******************************************************************************
 * FUI : used to save and recall the state of the user interface
 * This class provides essentially two new methods saveState() and recallState()
 * used to save on file and recall from file the state of the user interface.
 * The file is human readble and editable
 ******************************************************************************/

class FUI  : public UI
{
	std::stack<std::string>		fGroupStack;
	std::vector<std::string>	fNameList;
	std::map<std::string, float*>	fName2Zone;

 protected:

 	// labels are normalized by replacing white spaces by underscores and by
 	// removing parenthesis
	std::string normalizeLabel(const char* label)
	{
		std::string 	s;
		char 	c;

		while ((c=*label++)) {
			if (isspace(c)) 				{ s += '_'; }
			//else if ((c == '(') | (c == ')') ) 	{ }
			else 							{ s += c; }
		}
		return s;
	}

	// add an element by relating its full name and memory zone
	virtual void addElement (const char* label, float* zone)
	{
		std::string fullname (fGroupStack.top() + '/' + normalizeLabel(label));
		fNameList.push_back(fullname);
		fName2Zone[fullname] = zone;
	}

	// keep track of full group names in a stack
	virtual void pushGroupLabel(const char* label)
	{
		if (fGroupStack.empty()) {
			fGroupStack.push(normalizeLabel(label));
		} else {
			fGroupStack.push(fGroupStack.top() + '/' + normalizeLabel(label));
		}
	}

	virtual void popGroupLabel()
	{
		fGroupStack.pop();
	};

 public:

	FUI() 			{}
	virtual ~FUI() 	{}

	// -- Save and recall methods

	// save the zones values and full names
	virtual void saveState(const char* filename)
	{
		std::ofstream f(filename);

		for (unsigned int i=0; i<fNameList.size(); i++) {
			std::string	n = fNameList[i];
			float*	z = fName2Zone[n];
			f << *z << ' ' << n << std::endl;
		}

		f << std::endl;
		f.close();
	}

	// recall the zones values and full names
	virtual void recallState(const char* filename)
	{
		std::ifstream f(filename);
		float	v;
		std::string	n;

		while (f.good()) {
			f >> v >> n;
			if (fName2Zone.count(n)>0) {
				*(fName2Zone[n]) = v;
			} else {
				std::cerr << "recallState : parameter not found : " << n << " with value : " << v << std::endl;
			}
		}
		f.close();
	}


    // -- widget's layouts (just keep track of group labels)

    virtual void openFrameBox(const char* label) 		{ pushGroupLabel(label); }
    virtual void openTabBox(const char* label) 			{ pushGroupLabel(label); }
    virtual void openHorizontalBox(const char* label) 	{ pushGroupLabel(label); }
    virtual void openVerticalBox(const char* label)  	{ pushGroupLabel(label); }
    virtual void closeBox() 							{ popGroupLabel(); };

    // -- active widgets (just add an element)

    virtual void addButton(const char* label, float* zone) 			{ addElement(label, zone); }
    virtual void addToggleButton(const char* label, float* zone) 	{ addElement(label, zone); }
    virtual void addCheckButton(const char* label, float* zone) 	{ addElement(label, zone); }
    virtual void addVerticalSlider(const char* label, float* zone, float , float , float , float )
    																{ addElement(label, zone); }
    virtual void addHorizontalSlider(const char* label, float* zone, float , float , float , float )
    																{ addElement(label, zone); }
    virtual void addNumEntry(const char* label, float* zone, float , float , float , float )
    																{ addElement(label, zone); }

    // -- passive widgets (are ignored)

    virtual void addNumDisplay(const char* , float* , int ) {};
    virtual void addTextDisplay(const char* , float* , const char*[], float , float ) {};
    virtual void addHorizontalBargraph(const char* , float* , float , float ) {};
    virtual void addVerticalBargraph(const char* , float* , float , float ) {};

	// -- metadata are not used

    virtual void declare(float* , const char* , const char* ) {}
};

#endif // DEVELAMP_FUI_H_
