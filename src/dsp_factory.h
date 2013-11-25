/*
 * dsp_factory.h
 *
 * Part of develamp (the research and development amplifier)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef DEVELAMP_DSP_FACTORY_H_
#define DEVELAMP_DSP_FACTORY_H_

#include <algorithm>
#include <iterator>
#include <list>
#include <map>

#include "dsp.h"
#include "meta.h"

class UI;

class dsp_factory;

/*!
 * Trivial class to capture a DSP's meta data.
 */
class map_meta : public Meta {
public:
	std::map<std::string, std::string> data;

	virtual void declare(const char* key, const char* value) override {
		data.emplace(key, value);
	}
};

class dsp_factory {
protected:
	map_meta meta;

	virtual dsp* manufacture_dsp() const = 0;

public:
	static std::list<dsp_factory*> instances;

	dsp_factory() {
		instances.push_back(this);
	}

	virtual ~dsp_factory() {
		instances.erase(std::remove(std::begin(instances),
				           std::end(instances), this), std::end(instances));
	}

	// this is a ham-fisted way to start us on the road to the NVI design pattern... it seemed like
	// a good idea at the time.
	dsp* create_dsp() const {
		return manufacture_dsp();
	}

	const std::string& get_meta(const std::string& key) {
		// operator[] automatically populates key with the default constructor
		// (the empty string) if the key is not found.
		return meta.data[key];
	}

	const std::string& get_name() {
		return get_meta("name");
	}

	int get_priority() {
		return std::stoi(get_meta("priority"));
	}
};

#endif // DEVELAMP_DSP_FACTORY_H_
