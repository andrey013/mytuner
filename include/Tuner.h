#ifndef __Tuner_H__
#define __Tuner_H__
#include "DynamicLines.h"
#include "RtSoundInput.h"

class Tuner : public Ogre::FrameListener
{
public:
    std::vector<std::string> noteNames;
    Tuner(Ogre::SceneNode *m, Ogre::SceneManager *scn, MyGUI::Gui *gui);
    ~Tuner();
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    int note;
    double cents, currentCents;
private:
    void createDisplay();
    //MyGUI::WindowPtr window;
    //MyGUI::ComboBoxPtr deviceList;
    void onDeviceSelect(MyGUI::WidgetPtr _sender, unsigned int _index);
    void onApplyPress(MyGUI::WidgetPtr _sender);
    void onScroll(MyGUI::WidgetPtr _sender, unsigned int _index);
    void createWindow();
    unsigned int bufferFrames, numberOfFrames, fs, device, nn;
    unsigned int from;
    double sensitivity, scale;
    MyGUI::Gui *mGUI;
    MyGUI::StaticTextPtr textNote, textCents;
    Ogre::SceneNode *mainNode;
    Ogre::SceneNode *arrowNode;
    Ogre::SceneManager *mSceneMgr;
    DynamicLines *lines1;
    DynamicLines *lines2;
    DynamicLines *lineLow;
    DynamicLines *lineSens;
    RtSoundInput *snd;
};

#endif

