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
eScreen         mainMachine = mainScreen;
UIManager       UiManager;


void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    float hat = 0.0f, snareOut = 0.0f;
    hw.ProcessAllControls();
    UiManager.GenerateUiEvents();

    for(size_t i = 0; i < size; i++)
    {
        hat       = hihat.Process();
        snareOut  = snare.Process();
        out[0][i] = snareOut;
        out[1][i] = hat;
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
