#include "precompiled.h"
#include "Sleeper.h"
bool Sleeper::frameEnded(const Ogre::FrameEvent& evt)
{
    double ttW = 1000.0 / 50.0 - 1000.0 * evt.timeSinceLastFrame;
    if (ttW > 0) 
    #if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        Sleep(ttW);
    #else
        usleep(ttW * 1000);
    #endif
    return true;
}
