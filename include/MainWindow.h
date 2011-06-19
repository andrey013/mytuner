#include "Application.h"
#include "Tuner.h"
#include "Sleeper.h"
//#include <algorithm>

class MainWindow: public Application, public Ogre::FrameListener, public IKeyboardListener
{
protected:
    Ogre::SceneNode* tunerNode;
    Tuner *tuner;
    Sleeper *sleeper;
    MyGUI::WindowPtr window;
    virtual void createScene(void)
    {
        Ogre::Root::getSingleton().setFrameSmoothingPeriod(0);

        //Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
        //Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        //headNode->attachObject(ogreHead);
        mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

        mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

        Ogre::Light* l = mSceneMgr->createLight("MainLight");
        l->setPosition(-200,80,200);

        MyGUI::ButtonPtr button = mGUI->createWidget<MyGUI::Button>("Button", 10, 10, 200, 30, MyGUI::Align::Default, "Back");
        button->setCaption("Exit");
        //window = mGUI->createWidget<MyGUI::Window>("Window", 50, 50, 410, 410, MyGUI::Align::Center, "Overlapped");

        //MyGUI::EditPtr edit = window->createWidget<MyGUI::Edit>("Edit", 0, 0, 400, 400, MyGUI::Align::Default, "Back");
        //
        //window->setCaption(Ogre::String("каапвлопв"));
        //edit->setEditWordWrap(true);
	    //edit->setEditMultiLine(true);
	    //edit->addText(RtSoundInput::Info());
        //Ogre::LogManager::getSingletonPtr()->logMessage(RtSoundInput::Info());

        button->eventMouseButtonClick = MyGUI::newDelegate(this, &MainWindow::onExitButtonPressed);
        mRoot->addFrameListener(this);
        mInputManager->regKeyboardListener(this);

        tunerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        tuner = new Tuner(tunerNode, mSceneMgr, mGUI);
        mRoot->addFrameListener(tuner);

        sleeper = new Sleeper();
        mRoot->addFrameListener(sleeper);
    }

    virtual void destroyScene()
    {
        delete tuner;
    }

    virtual bool keyPressed(const Ogre::FrameEvent &evt, const OIS::KeyEvent &arg )
    {
        if (arg.key == OIS::KC_LEFT)
        {
            tunerNode->yaw(Ogre::Angle(9));
        }
        if (arg.key == OIS::KC_RIGHT)
        {
            tunerNode->yaw(Ogre::Angle(-9));
        }
        if (arg.key == OIS::KC_ESCAPE)
        {
            mRoot->queueEndRendering();
        }
        return true;
    }
    virtual bool keyReleased(const Ogre::FrameEvent &evt, const OIS::KeyEvent &arg )
    {
        return true;
    }

    void onExitButtonPressed(MyGUI::WidgetPtr _sender)
    {
        mRoot->queueEndRendering();
    }

    bool frameRenderingQueued(const Ogre::FrameEvent& evt)
    {
        //window->setCaption(Ogre::StringConverter::toString((Ogre::Real)tuner->note) + " - " + Ogre::StringConverter::toString((Ogre::Real)tuner->cents));
        return true;
    }
};

