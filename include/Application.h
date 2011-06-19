#ifndef __Application_H__
#define __Application_H__

//#include <SDL.h>
//#include <Ogre.h>
//#include <MyGUI.h>
//#include <MyGUI_OgrePlatform.h>
//#include "precompiled.h"
#include "InputManager.h"
#include "GuiListener.h"
#include "MyGUI_RTTLayer.h"

class Application
{
public:
    Application();
    virtual ~Application();

    virtual void go(void);

protected:
    bool m_bQuit;
    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    InputManager* mInputManager;
    Ogre::RenderWindow* mWindow;
	Ogre::String mResourcePath;
	Ogre::String mConfigPath;
	MyGUI::Gui* mGUI;
	MyGUI::OgrePlatform* mPlatform;
    virtual bool setup(void);

    virtual bool configure(void);

    virtual void chooseSceneManager(void);
    virtual void createCamera(void);

    virtual void createScene(void) = 0;

    virtual void destroyScene(void);

    virtual void createViewports(void);

    virtual void setupResources(void);

	virtual void createResourceListener(void);

	virtual void loadResources(void);

};

#endif
