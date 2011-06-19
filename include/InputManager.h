#ifndef __InputManager_H__
#define __InputManager_H__

class IKeyboardListener
{
public:
    virtual ~IKeyboardListener() {}
    virtual bool keyPressed(const Ogre::FrameEvent &evt, const OIS::KeyEvent &arg ) = 0;
    virtual bool keyReleased(const Ogre::FrameEvent &evt, const OIS::KeyEvent &arg ) = 0;
};

class IMouseListener
{
public:
    virtual ~IMouseListener() {}
    virtual bool mouseMoved(const Ogre::FrameEvent &evt, const OIS::MouseEvent &arg ) = 0;
    virtual bool mousePressed(const Ogre::FrameEvent &evt,const OIS::MouseEvent &arg, OIS::MouseButtonID id ) = 0;
    virtual bool mouseReleased(const Ogre::FrameEvent &evt, const OIS::MouseEvent &arg, OIS::MouseButtonID id ) = 0;
};

class InputManager:  public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::MouseListener, public OIS::KeyListener//, public OIS::JoyStickListener
{
public:
	InputManager(Ogre::RenderWindow* win, bool bufferedKeys = false, bool bufferedMouse = false,
			     bool bufferedJoy = false );
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);
	virtual ~InputManager();
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

protected:
    Ogre::FrameEvent mEvt;
	Ogre::RenderWindow* mWindow;
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::JoyStick* mJoy;

private:
    typedef std::list<IKeyboardListener*> KeyboardListenersList;
    typedef std::list<IMouseListener*> MouseListenersList;

    KeyboardListenersList kb_listeners;
    MouseListenersList mouse_listeners;
public:

    void regKeyboardListener(IKeyboardListener *l);
    void unregKeyboardListener(IKeyboardListener *l);
    void regMouseListener(IMouseListener *l);
    void unregMouseListener(IMouseListener *l);

    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
};

#endif
