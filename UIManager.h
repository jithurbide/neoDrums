#pragma once
#ifndef NEOOSC_UIMANAGER_H
#define NEOOSC_UIMANAGER_H
#include "daisy_patch.h"
#include "daisysp.h"
#include "ScreenItemMenu.h"
#include "extern.h"
using namespace daisy;
using namespace daisysp;
extern MappedStringListValue    hiHatSustainList;
extern AbstractMenu::ItemConfig mainMenuItems[];
extern MappedFloatValue         hiHatAccent;
extern MappedFloatValue         hiHatTone;
extern MappedFloatValue         hiHatDecay;
extern MappedFloatValue         hiHatNoisiness;

extern MappedFloatValue snareSustain;
extern MappedFloatValue snareAccent;
extern MappedFloatValue snareTone;
extern MappedFloatValue snareDecay;
extern MappedFloatValue snareSnappy;

class UIManager
{
  private:
    /* data */
    daisy::UI      ui;
    ScreenItemMenu mainMenu;
    ScreenItemMenu hiHatEditMenu;
    ScreenItemMenu snareEditMenu;
    ScreenItemMenu kickEditMenu;
    ScreenItemMenu controlEditMenu;
    UiEventQueue   eventQueue;
    static void    exitMenu(void* context);

  public:
    UIManager(/* args */);
    ~UIManager();
    void InitPages();
    void GenerateUiEvents();
    void updateMainPage();
    void processQueue();
    void ProcessEvent(const UiEventQueue::Event& e);
    void OpenMenu();
    void ProcessUI();
};

#endif // NEOOSC_UIMANAGER_H