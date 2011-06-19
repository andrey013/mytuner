#include "precompiled.h"
#include "Application.h"


Application::Application():
    mRoot(0), mInputManager(0), mGUI(0), mPlatform(0),
    mSceneMgr(0), mWindow(0), mCamera(0), m_bQuit(false)
{
}
Application::~Application()
{
    mGUI->shutdown();
    delete mGUI;
    mPlatform->shutdown();
    delete mPlatform;
    delete mInputManager;
    delete mRoot;
}

void Application::go(void)
{
    if (!setup())return;
    createScene();
    /*while(!m_bQuit)
    {
        Ogre::WindowEventUtilities::messagePump();
        if (!mRoot->renderOneFrame())break;
        SDL_GL_SwapBuffers();
    }*/
    mRoot->startRendering();
    mWindow->setVisible(false);
    destroyScene();
}

bool Application::setup(void)
{
    Ogre::String pluginsPath = mResourcePath + "plugins.cfg";
    mRoot = new Ogre::Root(pluginsPath, mConfigPath + "ogre.cfg", mResourcePath + "Ogre.log");
    setupResources();
    if (!configure())
        return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    mPlatform = new MyGUI::OgrePlatform();
    mPlatform->initialise(mWindow, mSceneMgr);
    mGUI = new MyGUI::Gui();
    mGUI->initialise("");
    MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::RTTLayer>("Layer");
    MyGUI::ResourceManager::getInstance().load("core.xml");
    
    GuiListener* guiL = new GuiListener(mGUI);
    mInputManager->regKeyboardListener(guiL);
    mInputManager->regMouseListener(guiL);

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    createResourceListener();
    loadResources();
    return true;
}

bool Application::configure(void)
{
    if(mRoot->restoreConfig()||mRoot->showConfigDialog())
    {
/*
        SDL_Init(SDL_INIT_VIDEO);
        SDL_EnableUNICODE(true);
        SDL_putenv((char*)"SDL_VIDEO_CENTERED=true");
        SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
        SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
        SDL_SetVideoMode(1024, 768, 32, SDL_OPENGL);
        SDL_WM_SetCaption("Demo window", NULL);

        mRoot->initialise(false);
        Ogre::NameValuePairList params;
#if OGRE_PLATFORM==OGRE_PLATFORM_WIN32
        SDL_SysWMinfo info;
        SDL_VERSION(&info.version);
        SDL_GetWMInfo(&info);
        params["externalWindowHandle"] = Ogre::StringConverter::toString(reinterpret_cast<size_t>(info.window));
        params["externalGLContent"] = Ogre::StringConverter::toString(reinterpret_cast<size_t>(info.hglrc));
        params["externalGLControl"] = Ogre::String("True");
#else
        params["currentGLContext"] = Ogre::String("True");
#endif
        mWindow = mRoot->createRenderWindow("Main window", 800, 600, false, &params);
        mWindow->setVisible(true);*/
        mWindow = mRoot->initialise(true);
        mInputManager= new InputManager(mWindow, true, true, true);
        mRoot->addFrameListener(mInputManager);
        return true;
    }
    else
    {
        return false;
    }
}

void Application::chooseSceneManager(void)
{
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "ExampleSMInstance");
}

void Application::createCamera(void)
{
    mCamera = mSceneMgr->createCamera("PlayerCam");

    mCamera->setPosition(Ogre::Vector3(0,0,300));
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);
}

void Application::destroyScene(void){}

void Application::createViewports(void)
{
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void Application::setupResources(void)
{
    Ogre::ConfigFile cf;
    cf.load(mResourcePath + "resources.cfg");
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
}

void Application::createResourceListener(void){}

void Application::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
