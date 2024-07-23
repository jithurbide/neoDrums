#include "UIManager.h"
#include "UiHardware.h"

uint32_t lastEnter = 0;
uint32_t upTime    = 0;
//main Window
AbstractMenu::ItemConfig mainWindowItems[1];

// Main menu items
const int                kNumMainMenuItems = 5;
AbstractMenu::ItemConfig mainMenuItems[kNumMainMenuItems];


// hiHat menu items

const int                kNumHiHatEditMenuItems = 6;
AbstractMenu::ItemConfig hiHatEditMenuItems[kNumHiHatEditMenuItems];
MappedFloatValue         hiHatAccent(0.0f, 1.0f, 0.1f);
MappedFloatValue         hiHatTone(0.0f, 1.0f, 0.5f);
MappedFloatValue         hiHatDecay(0.0f, 1.0f, 0.8f);
MappedFloatValue         hiHatNoisiness(0.0f, 1.0f, 0.7f);
const char*              hiHatSustainValues[] = {"True", "False"};
MappedStringListValue    hiHatSustainList(hiHatSustainValues, 2, 1);


// snare menu items
const int                kNumSnareEditMenuItems = 6;
AbstractMenu::ItemConfig snareEditMenuItems[kNumSnareEditMenuItems];
MappedFloatValue         snareSustain(0.0f, 1.0f, 0.1f);
MappedFloatValue         snareAccent(0.0f, 1.0f, 0.1f);
MappedFloatValue         snareTone(0.0f, 1.0f, 0.1f);
MappedFloatValue         snareDecay(0.0f, 1.0f, 0.1f);
MappedFloatValue         snareSnappy(0.0f, 1.0f, 0.1f);


// kick menu items

const int                kNumKickEditMenuItems = 7;
AbstractMenu::ItemConfig kickEditMenuItems[kNumKickEditMenuItems];
MappedFloatValue         kickSustain(0.0f, 1.0f, 0.1f);
MappedFloatValue         kickAccent(0.0f, 1.0f, 0.1f);
MappedFloatValue         kickTone(0.0f, 1.0f, 0.1f);
MappedFloatValue         kickDecay(0.0f, 1.0f, 0.1f);
MappedFloatValue         kickAttackFmAmount(0.0f, 1.0f, 0.1f);
MappedFloatValue         kickSelfFmAmount(0.0f, 1.0f, 0.1f);


// control menu items
const int                kNumControlEditMenuItems = 17;
AbstractMenu::ItemConfig controlEditMenuItems[kNumControlEditMenuItems];
const char*              controlListValues[] = {"HiHat Sustain",
                                                "HiHat Accent",
                                                "HiHat Tone",
                                                "HiHat Decay",
                                                "HiHat Noisiness",
                                                "Snare Sustain",
                                                "Snare Accent",
                                                "Snare Tone",
                                                "Snare Decay",
                                                "Snare Snappy",
                                                "Kick Sustain",
                                                "Kick Accent",
                                                "Kick Tone",
                                                "Kick Decay",
                                                "Kick AttackFmAmount",
                                                "Kick SelfFmAmount"};
MappedStringListValue    controlListValueOne(controlListValues, 16, 0);
MappedStringListValue    controlListValueTwo(controlListValues, 16, 1);
MappedStringListValue    controlListValueThree(controlListValues, 16, 2);
MappedStringListValue    controlListValueFour(controlListValues, 16, 3);


UIManager::UIManager()
{
    UI::SpecialControlIds specialControlIds;
    specialControlIds.okBttnId
        = bttnEncoder; // Encoder button is our okay button
    specialControlIds.menuEncoderId
        = encoderMain; // Encoder is used as the main menu navigation encoder

    // This is the canvas for the OLED display.
    UiCanvasDescriptor oledDisplayDescriptor;
    oledDisplayDescriptor.id_     = canvasOledDisplay; // the unique ID
    oledDisplayDescriptor.handle_ = &hw.display;   // a pointer to the display
    oledDisplayDescriptor.updateRateMs_      = 50; // 50ms == 20Hz
    oledDisplayDescriptor.screenSaverTimeOut = 0;  // display always on
    oledDisplayDescriptor.clearFunction_     = &ClearCanvas;
    oledDisplayDescriptor.flushFunction_     = &FlushCanvas;

    ui.Init(eventQueue,
            specialControlIds,
            {oledDisplayDescriptor},
            canvasOledDisplay);
}

UIManager::~UIManager() {}
void UIManager::OpenMenu()
{
    ui.OpenPage(mainMenu);
}

void UIManager::ProcessUI()
{
    ui.Process();
}

void UIManager::ProcessEvent(const UiEventQueue::Event& e)
{
    switch(e.type)
    {
        case UiEventQueue::Event::EventType::buttonPressed:
            if(mainMachine == mainScreen)
            {
                mainMachine = chargeTomenu;
            }
            else if(mainMachine == menuScreen)
            {
                mainMachine = changeToMain;
            }
            break;
        default: break;
    }
}

void UIManager::processQueue()
{
    while(!eventQueue.IsQueueEmpty())
    {
        UiEventQueue::Event e = eventQueue.GetAndRemoveNextEvent();
        if(e.type != UiEventQueue::Event::EventType::invalid)
        {
            ProcessEvent(e);
        }
    }
}

void UIManager::updateMainPage()
{
    uint32_t    minX       = 0;
    uint32_t    minY       = 0;
    uint32_t    lineOffset = 12;
    uint8_t     lineNumber = 0;
    std::string str        = "NeoDrums";
    char*       cstr       = &str[0];
    uint32_t    now        = 0;
    now                    = System::GetNow();

    if(now - lastEnter > 1000)
    {
        lastEnter = now;
        upTime += 1;
        //Title
        hw.display.Fill(false);
        hw.display.SetCursor(minX, minY + lineNumber * lineOffset);
        hw.display.WriteString(cstr, Font_7x10, true);

        lineNumber++;
        hw.display.SetCursor(minX, minY + lineNumber * lineOffset);
        str = "HiHat : 60";
        hw.display.WriteString(cstr, Font_6x8, true);

        lineNumber++;
        hw.display.SetCursor(minX, minY + lineNumber * lineOffset);
        str = "Snare : 62";
        hw.display.WriteString(cstr, Font_6x8, true);

        lineNumber++;
        hw.display.SetCursor(minX, minY + lineNumber * lineOffset);
        str = "Kick : 64";
        hw.display.WriteString(cstr, Font_6x8, true);

        lineNumber++;
        hw.display.SetCursor(minX, minY + lineNumber * lineOffset);
        str = "Time (s) : " + std::to_string(upTime);
        hw.display.WriteString(cstr, Font_6x8, true);

        hw.display.Update();
    }
}
void UIManager::exitMenu(void* context)
{
    mainMachine = changeToMain;
}

void UIManager::GenerateUiEvents()
{
    if(hw.encoder.RisingEdge())
        eventQueue.AddButtonPressed(bttnEncoder, 1);

    const auto increments = hw.encoder.Increment();
    if(increments != 0)
        eventQueue.AddEncoderTurned(encoderMain, increments, 12);
}

void UIManager::InitPages()
{
    // ====================================================================
    // The main config menu
    // ====================================================================

    mainMenuItems[0].type = daisy::AbstractMenu::ItemType::openUiPageItem;
    mainMenuItems[0].text = "HiHat";
    mainMenuItems[0].asOpenUiPageItem.pageToOpen = &hiHatEditMenu;


    mainMenuItems[1].type = daisy::AbstractMenu::ItemType::openUiPageItem;
    mainMenuItems[1].text = "Snare";
    mainMenuItems[1].asOpenUiPageItem.pageToOpen = &snareEditMenu;

    mainMenuItems[2].type = daisy::AbstractMenu::ItemType::openUiPageItem;
    mainMenuItems[2].text = "Kick";
    mainMenuItems[2].asOpenUiPageItem.pageToOpen = &kickEditMenu;

    mainMenuItems[3].type = daisy::AbstractMenu::ItemType::openUiPageItem;
    mainMenuItems[3].text = "Config CV";
    mainMenuItems[3].asOpenUiPageItem.pageToOpen = &controlEditMenu;

    mainMenuItems[4].type = daisy::AbstractMenu::ItemType::callbackFunctionItem;
    mainMenuItems[4].text = "Exit";
    mainMenuItems[4].asCallbackFunctionItem.callbackFunction = exitMenu;


    mainMenu.Init(mainMenuItems, kNumMainMenuItems);

    // ====================================================================

    // ====================================================================
    // The "Hi Hat edit" menu     = {"Sustain", "Accent", "Tone", "Decay", "Noisiness"};

    // ====================================================================

    hiHatEditMenuItems[0].type = daisy::AbstractMenu::ItemType::valueItem;
    hiHatEditMenuItems[0].text = "Sustain";
    hiHatEditMenuItems[0].asMappedValueItem.valueToModify = &hiHatSustainList;

    hiHatEditMenuItems[1].type = daisy::AbstractMenu::ItemType::valueItem;
    hiHatEditMenuItems[1].text = "Accent";
    hiHatEditMenuItems[1].asMappedValueItem.valueToModify = &hiHatAccent;

    hiHatEditMenuItems[2].type = daisy::AbstractMenu::ItemType::valueItem;
    hiHatEditMenuItems[2].text = "Tone";
    hiHatEditMenuItems[2].asMappedValueItem.valueToModify = &hiHatTone;

    hiHatEditMenuItems[3].type = daisy::AbstractMenu::ItemType::valueItem;
    hiHatEditMenuItems[3].text = "Decay";
    hiHatEditMenuItems[3].asMappedValueItem.valueToModify = &hiHatDecay;

    hiHatEditMenuItems[4].type = daisy::AbstractMenu::ItemType::valueItem;
    hiHatEditMenuItems[4].text = "Noisiness";
    hiHatEditMenuItems[4].asMappedValueItem.valueToModify = &hiHatNoisiness;

    hiHatEditMenuItems[5].type = daisy::AbstractMenu::ItemType::closeMenuItem;
    hiHatEditMenuItems[5].text = "Back";


    hiHatEditMenu.Init(hiHatEditMenuItems, kNumHiHatEditMenuItems);

    // ====================================================================
    // The "Snare edit" menu  {"Sustain", "Accent", "Tone", "Decay", "Snappy"};
    // ====================================================================
    snareEditMenuItems[0].type = daisy::AbstractMenu::ItemType::valueItem;
    snareEditMenuItems[0].text = "Sustain";
    snareEditMenuItems[0].asMappedValueItem.valueToModify = &snareSustain;

    snareEditMenuItems[1].type = daisy::AbstractMenu::ItemType::valueItem;
    snareEditMenuItems[1].text = "Accent";
    snareEditMenuItems[1].asMappedValueItem.valueToModify = &snareAccent;

    snareEditMenuItems[2].type = daisy::AbstractMenu::ItemType::valueItem;
    snareEditMenuItems[2].text = "Tone";
    snareEditMenuItems[2].asMappedValueItem.valueToModify = &snareTone;

    snareEditMenuItems[3].type = daisy::AbstractMenu::ItemType::valueItem;
    snareEditMenuItems[3].text = "Decay";
    snareEditMenuItems[3].asMappedValueItem.valueToModify = &snareDecay;

    snareEditMenuItems[4].type = daisy::AbstractMenu::ItemType::valueItem;
    snareEditMenuItems[4].text = "Snappy";
    snareEditMenuItems[4].asMappedValueItem.valueToModify = &snareSnappy;

    snareEditMenuItems[5].type = daisy::AbstractMenu::ItemType::closeMenuItem;
    snareEditMenuItems[5].text = "Back";

    snareEditMenu.Init(snareEditMenuItems, kNumSnareEditMenuItems);


    // ====================================================================
    // The "Kick edit" menu   {"Sustain", "Accent", "Tone", "Decay", "AttackFmAmount", "SelfFmAmount"}
    // ====================================================================
    kickEditMenuItems[0].type = daisy::AbstractMenu::ItemType::valueItem;
    kickEditMenuItems[0].text = "Sustain";
    kickEditMenuItems[0].asMappedValueItem.valueToModify = &kickSustain;

    kickEditMenuItems[1].type = daisy::AbstractMenu::ItemType::valueItem;
    kickEditMenuItems[1].text = "Accent";
    kickEditMenuItems[1].asMappedValueItem.valueToModify = &kickAccent;

    kickEditMenuItems[2].type = daisy::AbstractMenu::ItemType::valueItem;
    kickEditMenuItems[2].text = "Tone";
    kickEditMenuItems[2].asMappedValueItem.valueToModify = &kickTone;

    kickEditMenuItems[3].type = daisy::AbstractMenu::ItemType::valueItem;
    kickEditMenuItems[3].text = "Decay";
    kickEditMenuItems[3].asMappedValueItem.valueToModify = &kickDecay;

    kickEditMenuItems[4].type = daisy::AbstractMenu::ItemType::valueItem;
    kickEditMenuItems[4].text = "AttackFmAmount";
    kickEditMenuItems[4].asMappedValueItem.valueToModify = &kickAttackFmAmount;

    kickEditMenuItems[5].type = daisy::AbstractMenu::ItemType::valueItem;
    kickEditMenuItems[5].text = "SelfFmAmount";
    kickEditMenuItems[5].asMappedValueItem.valueToModify = &kickSelfFmAmount;


    kickEditMenuItems[6].type = daisy::AbstractMenu::ItemType::closeMenuItem;
    kickEditMenuItems[6].text = "Back";

    snareEditMenu.Init(kickEditMenuItems, kNumKickEditMenuItems);

    // ====================================================================
    // The "CV settings" menu     = {"Sustain", "Accent", "Tone", "Decay", "Noisiness"};

    // ====================================================================

    controlEditMenuItems[0].type = daisy::AbstractMenu::ItemType::valueItem;
    controlEditMenuItems[0].text = "CTRL 1";
    controlEditMenuItems[0].asMappedValueItem.valueToModify
        = &controlListValueOne;

    controlEditMenuItems[1].type = daisy::AbstractMenu::ItemType::valueItem;
    controlEditMenuItems[1].text = "CTRL 2";
    controlEditMenuItems[1].asMappedValueItem.valueToModify
        = &controlListValueTwo;

    controlEditMenuItems[2].type = daisy::AbstractMenu::ItemType::valueItem;
    controlEditMenuItems[2].text = "CTRL 3";
    controlEditMenuItems[2].asMappedValueItem.valueToModify
        = &controlListValueThree;

    controlEditMenuItems[3].type = daisy::AbstractMenu::ItemType::valueItem;
    controlEditMenuItems[3].text = "CTRL 4";
    controlEditMenuItems[3].asMappedValueItem.valueToModify
        = &controlListValueFour;

    controlEditMenu.Init(controlEditMenuItems, kNumControlEditMenuItems);
}