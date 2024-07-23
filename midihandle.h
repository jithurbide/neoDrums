#pragma once
#ifndef NEOOSC_MIDI_H
#define NEOOSC_MIDI_H
#include "daisy_patch.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;


extern int     midiChannel; /**< & */
extern uint8_t midiNote;    /**< & */
extern uint8_t midiVelocity;
extern uint8_t midiType;

void HandleMidiMessage(MidiEvent m);

void MidiProcess();

#endif