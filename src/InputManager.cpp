#include "precompiled.h"
#include "InputManager.h"

InputManager::InputManager(Ogre::RenderWindow* win, bool bufferedKeys, bool bufferedMouse,
                           bool bufferedJoy) :
    mWindow(win), mInputManager(0), mMouse(0), mKeyboard(0), mJoy(0)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    win->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, bufferedKeys ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, bufferedMouse ));
    try
    {
        //mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject( OIS::OISJoyStick, bufferedJoy ));
    }
    catch(...)
    {
        mJoy = 0;
    }

    mKeyboard->setTextTranslation(OIS::Keyboard::Unicode);
    windowResized(mWindow);
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
}

void InputManager::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void InputManager::windowClosed(Ogre::RenderWindow* rw)
{
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
            mInputManager->destroyInputObject( mJoy );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

InputManager::~InputManager()
{
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
}

bool InputManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    mEvt = evt;
    if(mWindow->isClosed())	return false;

    mKeyboard->capture();
    mMouse->capture();
    if( mJoy ) mJoy->capture();
/*
    mMouse->setEventCallback()
    if( !mKeyboard->buffered() )
        if( processUnbufferedKeyInput(evt) == false )
            return false;

    if( !mMouse->buffered() )
        if( processUnbufferedMouseInput(evt) == false )
            return false;
*/
    return true;
}

void InputManager::regKeyboardListener(IKeyboardListener *l)
{
    if (l && std::find(kb_listeners.begin(), kb_listeners.end(), l) == kb_listeners.end())
        kb_listeners.push_back(l);
}

void InputManager::unregKeyboardListener(IKeyboardListener *l)
{
    if (l)
        kb_listeners.remove(l);
}

void InputManager::regMouseListener(IMouseListener *l)
{
    if (l && std::find(mouse_listeners.begin(), mouse_listeners.end(), l) == mouse_listeners.end())
        mouse_listeners.push_back(l);
}

void InputManager::unregMouseListener(IMouseListener *l)
{
    if (l)
        mouse_listeners.remove(l);
}

bool InputManager::mouseMoved( const OIS::MouseEvent &arg )
{
    bool result = true;
    MouseListenersList::const_iterator m_it = mouse_listeners.begin(), m_end = mouse_listeners.end();
    for (; m_it != m_end; ++m_it)
        result &= (*m_it)->mouseMoved(mEvt, arg);
    return result;
}
bool InputManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    bool result = true;
    MouseListenersList::const_iterator m_it = mouse_listeners.begin(), m_end = mouse_listeners.end();
    for (; m_it != m_end; ++m_it)
        result &= (*m_it)->mousePressed(mEvt, arg, id);
    return result;
}
bool InputManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    bool result = true;
    MouseListenersList::const_iterator m_it = mouse_listeners.begin(), m_end = mouse_listeners.end();
    for (; m_it != m_end; ++m_it)
        result &= (*m_it)->mouseReleased(mEvt, arg, id);
    return result;
}
bool InputManager::keyPressed( const OIS::KeyEvent &arg )
{
    bool result = true;
    KeyboardListenersList::const_iterator m_it = kb_listeners.begin(), m_end = kb_listeners.end();
    for (; m_it != m_end; ++m_it)
        result &= (*m_it)->keyPressed(mEvt, arg);
    return result;
}
bool InputManager::keyReleased( const OIS::KeyEvent &arg )
{
    bool result = true;
    KeyboardListenersList::const_iterator m_it = kb_listeners.begin(), m_end = kb_listeners.end();
    for (; m_it != m_end; ++m_it)
        result &= (*m_it)->keyReleased(mEvt, arg);
    return result;
}
