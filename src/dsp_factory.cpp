/*
 * dsp_factory.cpp
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

#include "dsp_factory.h"

std::list<std::shared_ptr<dsp_factory>> dsp_factory::registry;

int dsp_factory::get_priority()
{
    std::string s = get_meta("priority");

    // place it in the middle if there is no priority
    if (s == "")
        return 50;

    // this will throw an exception is priority contains illegal characters
    return std::stoi(s);
}
