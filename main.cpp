#include "daisy_patch.h"
#include "daisysp.h"
#include <string>
#include "midihandle.h"
#include "extern.h"
#include "UIManager.h"

using namespace daisy;
using namespace daisysp;

DaisyPatch      hw;
HiHat<>         hihat;
AnalogSnareDrum snare;
AnalogBassDrum  Kick;
eScreen         mainMachine = mainScreen;
UIManager       UiManager;
float           hat = 0.0f, snareOut = 0.0f, kickOut = 0.0f, mix = 0.0f;

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    hw.ProcessAllControls();
    UiManager.GenerateUiEvents();

    for(size_t i = 0; i < size; i++)
    {
        //hat      = hihat.Process();
        snareOut = snare.Process();
        kickOut  = Kick.Process();
        mix      = snareOut + kickOut;

        // out[0][i] = hat;
        out[1][i] = snareOut;
        out[2][i] = kickOut;
        out[3][i] = mix;
    }
}

void updateSoundValue()
{
    if(hiHatSustainList.GetString() == "True")
    {
        hihat.SetSustain(true);
    }
    else
    {
        hihat.SetSustain(false);
    }
    hihat.SetDecay(hiHatDecay.Get());
    hihat.SetTone(hiHatTone.Get());
    hihat.SetNoisiness(hiHatNoisiness.Get());
    snare.SetDecay(snareDecay.Get());
    snare.SetSnappy(snareSnappy.Get());
    snare.SetTone(snareTone.Get());
}

// Main -- Init, and Midi Handling
int main(void)
{
    // Init
    float samplerate;
    hw.Init(true);
    samplerate = hw.AudioSampleRate();

    UiManager.InitPages();


    hihat.Init(samplerate);
    snare.Init(samplerate);
    Kick.Init(samplerate);
    Kick.SetFreq(50.f);

    // Start stuff.
    hw.midi.StartReceive();
    hw.StartAdc();
    hw.StartAudio(AudioCallback);
    for(;;)
    {
        updateSoundValue();
        MidiProcess();

        switch(mainMachine)
        {
            case mainScreen:
                UiManager.updateMainPage();
                UiManager.processQueue();
                break;
            case chargeTomenu:
                mainMachine = menuScreen;
                UiManager.OpenMenu();

                break;
            case menuScreen: UiManager.ProcessUI(); break;
            case changeToMain:
                mainMachine = mainScreen;
                UiManager.updateMainPage();
                //ui.ClosePage(mainMenu);
                break;
            default: break;
        }
    }
}
