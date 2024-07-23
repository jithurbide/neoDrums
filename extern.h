#pragma once
#ifndef NEOOSC_EXTERN_H
#define NEOOSC_EXTERN_H

#include "daisy_patch.h"
#include "daisysp.h"
typedef enum
{
    mainScreen = 0,
    chargeTomenu,
    menuScreen,
    changeToMain,
} eScreen;

extern eScreen           mainMachine;
extern daisy::DaisyPatch hw;
extern int               midiChannel; /**< & */
extern uint8_t           midiNote;    /**< & */
extern uint8_t           midiVelocity;
extern uint8_t           midiType;

#endif