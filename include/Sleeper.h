#ifndef _SLEEPER_H_
#define _SLEEPER_H_

class Sleeper : public Ogre::FrameListener
{
    bool frameEnded(const Ogre::FrameEvent& evt);
};

#endif

