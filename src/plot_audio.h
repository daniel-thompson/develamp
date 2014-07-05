/*
 * plot_audio.h
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

#ifndef DEVELAMP_PLOT_AUDIO_H_
#define DEVELAMP_PLOT_AUDIO_H_

#include <atomic>
#include <cassert>
#include <cmath>
#include <ios>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

#include "audio.h"
#include "dsp.h"

class plot_audio : public audio {
    typedef std::vector<FAUSTFLOAT> sample_buffer;

    const FAUSTFLOAT pi = acos(-1);

    dsp* fDsp;
    FAUSTFLOAT phase;

    std::vector<sample_buffer> inputs;
    std::vector<FAUSTFLOAT*> input_pointers;
    std::vector<sample_buffer> outputs;
    std::vector<FAUSTFLOAT*> output_pointers;

    std::thread worker;
    std::atomic<bool> running;

public:
    plot_audio()
        : fDsp{nullptr}
        , phase{}
        , inputs{}
        , outputs{}
        , worker{}
        , running{false}
    {
    }

    virtual ~plot_audio()
    {
        stop();
    }

    virtual bool init(const char* name, dsp* DSP)
    {
        fDsp = DSP;
	fDsp->init(44100);

	phase = 0;
	inputs.resize(fDsp->getNumInputs());
	input_pointers.resize(fDsp->getNumInputs());
	outputs.resize(fDsp->getNumOutputs());
	output_pointers.resize(fDsp->getNumOutputs());

	// set all the sample buffers to 128 samples
	for (auto& i : inputs) i.resize(128);
	for (auto& o : outputs) o.resize(128);

	// fill out the pointers
	for (auto i=0u; i<inputs.size(); ++i)
	    input_pointers[i] = inputs[i].data();
	for (auto i=0u; i<outputs.size(); ++i)
	    output_pointers[i] = outputs[i].data();

        return true;
    }

    virtual bool start()
    {
        running = true;
        worker = std::thread{[this]() { this->process(); }};
        return true;
    }

    virtual void stop()
    {
	if (worker.joinable()) {
            running = false;
            worker.join();
        }
    }

private:

    void process()
    {
        AVOIDDENORMALS;

        while (running) {
            generate_input();

            fDsp->compute(inputs[0].size(), input_pointers.data(),
                          output_pointers.data());

	    for(auto i=0u; i<outputs[0].size(); ++i) {
		for (auto& output : outputs)
                    std::cout << std::right << std::setw(16)
                              << std::setprecision(5) << output[i];
                std::cout << std::endl;
	    }

            //std::this_thread::sleep_for(std::chrono::seconds{5});
        }
    }

    /*!
     * Generate a sine wave and store it in all inputs.
     */
    void generate_input() {
        for (auto i = 0u; i < inputs[0].size(); ++i) {
            auto sample = sin(phase) / 2;
            phase += ((2 * pi) /  45.01);

            for (auto& input : inputs)
                input[i] = sample;
        }
    }
};

#endif // DEVELAMP_PLOT_AUDIO_H_
