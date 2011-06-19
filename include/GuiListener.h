#ifndef __GuiListener_H__
#define __GuiListener_H__

//#include <MyGUI.h>
//#include "precompiled.h"
#include "InputManager.h"

class GuiListener: public IKeyboardListener , public IMouseListener
{
private:
    MyGUI::Gui* mGUI;
    float mouseX;
    float mouseY;
public:
    GuiListener(MyGUI::Gui* m);
    virtual bool keyPressed(const Ogre::FrameEvent &evt, const OIS::KeyEvent &arg );
    virtual bool keyReleased(const Ogre::FrameEvent &evt, const OIS::KeyEvent &arg );
    virtual bool mouseMoved(const Ogre::FrameEvent &evt, const OIS::MouseEvent &arg );
    virtual bool mousePressed(const Ogre::FrameEvent &evt,const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased(const Ogre::FrameEvent &evt, const OIS::MouseEvent &arg, OIS::MouseButtonID id );
};

#endif
