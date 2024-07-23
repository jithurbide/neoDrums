
#include "midihandle.h"
#include "daisy_seed.h"

#include "daisy_patch.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;


int                    midiChannel; /**< & */
uint8_t                midiNote;    /**< & */
uint8_t                midiVelocity;
uint8_t                midiType;
extern DaisyPatch      hw;
extern HiHat<>         hihat;
extern AnalogSnareDrum snare;
extern AnalogSnareDrum snare2;
extern AnalogBassDrum  Kick;


void MidiProcess()
{
    hw.midi.Listen();
    //Handle MIDI Events
    while(hw.midi.HasEvents())
    {
        HandleMidiMessage(hw.midi.PopEvent());
    }
}

// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
    midiType = m.type;
    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();
            midiChannel   = m.channel;
            midiNote      = m.data[0];
            midiVelocity  = m.data[1];

            // This is to avoid Max/MSP Note outs for now..
            if(m.data[1] != 0)
            {
                switch(midiChannel)
                {
                    case 0:
                        switch(midiNote)
                        {
                            case 60:
                                hihat.SetAccent(midiVelocity / 127);
                                //hihat.SetFreq(mtof((float)midiNote));
                                hihat.Trig();
                                break;
                            case 62: //snare
                                snare.SetAccent(midiVelocity / 127);
                                //snare.SetFreq(mtof((float)midiNote));
                                snare.Trig();
                                break;
                            case 64: //Kick
                                Kick.SetAccent(midiVelocity / 127);
                                //Kick.SetFreq(mtof((float)midiNote));
                                Kick.Trig();
                                break;
                            default: break;
                        }
                        break;
                    default: break;
                }

                p = m.AsNoteOn();
            }
        }
        break;
        default: break;
    }
}
