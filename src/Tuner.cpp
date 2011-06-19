#include "precompiled.h"
#include "Tuner.h"


Tuner::Tuner(Ogre::SceneNode *m, Ogre::SceneManager *scn, MyGUI::Gui *gui) :
    mainNode(m), mSceneMgr(scn), mGUI(gui), currentCents(0)
{

    //bufferFrames = 256;
    //numberOfFrames = 8;

    snd = new RtSoundInput();//1,0,bufferFrames,numberOfFrames,fs);
    Ogre::Entity* arrow = mSceneMgr->createEntity("Arrow", "Circle.mesh");
    arrowNode = mainNode->createChildSceneNode();
    arrowNode->attachObject(arrow);
    arrowNode->setScale(6,6,6);
    arrowNode->setPosition(50,0,1);
    arrowNode->pitch(Ogre::Angle(90));

    Ogre::Entity* box = mSceneMgr->createEntity("Box", "Cube.mesh");
    Ogre::SceneNode *boxNode = mainNode->createChildSceneNode();
    boxNode->attachObject(box);
    boxNode->setScale(110,2.2,2);
    boxNode->setPosition(0,-4,-1);

    createDisplay();
    createWindow();



    //snd->startStream(device,0,bufferFrames,numberOfFrames,fs);

    noteNames.push_back("C");
    noteNames.push_back("C♯");
    noteNames.push_back("D");
    noteNames.push_back("E♭");
    noteNames.push_back("E");
    noteNames.push_back("F");
    noteNames.push_back("F♯");
    noteNames.push_back("G");
    noteNames.push_back("G♯");
    noteNames.push_back("A");
    noteNames.push_back("B♭");
    noteNames.push_back("B");
}

Tuner::~Tuner()
{
    delete snd;
}

void Tuner::createDisplay()
{
    Ogre::SceneNode *linesNode = mainNode->createChildSceneNode();
    linesNode->setPosition(-50, 0, 0);

    lines1 = new DynamicLines(Ogre::RenderOperation::OT_LINE_STRIP);
    linesNode->attachObject(lines1);

    lines2 = new DynamicLines(Ogre::RenderOperation::OT_LINE_STRIP);
    linesNode->attachObject(lines2);

    lineLow = new DynamicLines(Ogre::RenderOperation::OT_LINE_LIST);
    linesNode->attachObject(lineLow);

    lineSens = new DynamicLines(Ogre::RenderOperation::OT_LINE_LIST);
    linesNode->attachObject(lineSens);

    Ogre::TexturePtr m_texture;
    m_texture = Ogre::TextureManager::getSingleton().createManual("RTTTexture1",
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
                256, 256, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET);

    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create
                                 ("BrowserMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    material->createTechnique()->createPass();
    material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    material->getTechnique(0)->getPass(0)->createTextureUnitState("RTTTexture1");

    Ogre::Plane plane(Ogre::Vector3::UNIT_Z, 0);
    Ogre::MeshManager::getSingleton().createPlane("noteNamePlane",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
            100,100);

    Ogre::Entity *ent = mSceneMgr->createEntity("noteName", "noteNamePlane");
    ent->setMaterialName("BrowserMaterial");
    Ogre::SceneNode *node = mainNode->createChildSceneNode();
    node->setPosition(50, 50, 0);
    node->attachObject(ent);

    MyGUI::StaticImagePtr image = mGUI->createWidget<MyGUI::StaticImage>("StaticImage", 0, 0, 256, 256, MyGUI::Align::Default, "RTTLayer1");
    image->setImageTexture("arrow.png");

    textNote = mGUI->createWidget<MyGUI::StaticText>("StaticText", 0, 0, 128, 128, MyGUI::Align::Default, "RTTLayer1");
    textNote->setTextColour(MyGUI::Colour(1,1,1));
    textNote->setFontName("font_DejaVuSans.128");
    textNote->setTextAlign(MyGUI::Align::Left|MyGUI::Align::Top);

    textCents = mGUI->createWidget<MyGUI::StaticText>("StaticText", 128, 0, 128, 128, MyGUI::Align::Default, "RTTLayer1");
    textCents->setTextColour(MyGUI::Colour(1,1,1));
    textCents->setFontName("font_DejaVuSans.128");
    textCents->setFontHeight(64);
    textCents->setTextAlign(MyGUI::Align::Right|MyGUI::Align::Top);
}

void Tuner::createWindow()
{

    mGUI->load("tuner.layout");
    MyGUI::ComboBoxPtr bufferSize = mGUI->findWidget<MyGUI::ComboBox>("bufferSize");
    bufferSize->setComboModeDrop(true);
    bufferSize->addItem("128");
    bufferSize->addItem("256");
    bufferSize->addItem("512");
    bufferSize->addItem("1024");
    bufferSize->setIndexSelected(1);
    MyGUI::ComboBoxPtr nofBuffers = mGUI->findWidget<MyGUI::ComboBox>("nofBuffers");
    nofBuffers->setComboModeDrop(true);
    nofBuffers->addItem("1");
    nofBuffers->addItem("2");
    nofBuffers->addItem("4");
    nofBuffers->addItem("8");
    nofBuffers->addItem("16");
    nofBuffers->addItem("32");
    nofBuffers->addItem("64");
    nofBuffers->addItem("128");
    nofBuffers->setIndexSelected(3);
    MyGUI::ComboBoxPtr samplingRate = mGUI->findWidget<MyGUI::ComboBox>("samplingRate");
    samplingRate->setComboModeDrop(true);
    MyGUI::ComboBoxPtr deviceList = mGUI->findWidget<MyGUI::ComboBox>("deviceList");
    deviceList->eventComboChangePosition = MyGUI::newDelegate(this, &Tuner::onDeviceSelect);
    deviceList->setComboModeDrop(true);

    MyGUI::ButtonPtr applyButton = mGUI->findWidget<MyGUI::Button>("applyButton");
    applyButton->eventMouseButtonClick = MyGUI::newDelegate(this, &Tuner::onApplyPress);
    unsigned int devices = snd->getDeviceCount();
    for (unsigned int i=0; i<devices; i++)
    {
        wchar_t wchartemp[256];
        RtAudio::DeviceInfo info = snd->getDeviceInfo(i);
        if(!info.inputChannels)continue;
        mbstowcs(wchartemp, info.name.c_str(),info.name.length());
        deviceList->addItem( Ogre::StringConverter::toString(i) + " : " + MyGUI::UString(wchartemp), i);
    }
    deviceList->setIndexSelected(0);
    MyGUI::HScrollPtr scroll;
    scroll = mGUI->findWidget<MyGUI::HScroll>("scale1");
    scroll->setScrollRange(100);
    scroll->eventScrollChangePosition = MyGUI::newDelegate(this, &Tuner::onScroll);
    scroll->setScrollPosition(3);

    scroll = mGUI->findWidget<MyGUI::HScroll>("sensitivity");
    scroll->setScrollRange(100);
    scroll->eventScrollChangePosition = MyGUI::newDelegate(this, &Tuner::onScroll);
    scroll->setScrollPosition(10);

    onDeviceSelect(0, 0);
    onApplyPress(0);

}

void Tuner::onDeviceSelect(MyGUI::WidgetPtr _sender, unsigned int _index)
{
    MyGUI::ComboBoxPtr deviceList = mGUI->findWidget<MyGUI::ComboBox>("deviceList");
    device = *deviceList->getItemDataAt<unsigned int>(_index);
    MyGUI::ComboBoxPtr samplingRate = mGUI->findWidget<MyGUI::ComboBox>("samplingRate");
    samplingRate->removeAllItems();
    RtAudio::DeviceInfo info = snd->getDeviceInfo(device);
    if(!info.inputChannels)return;
    for (unsigned int i=0; i<info.sampleRates.size(); i++)
        samplingRate->addItem(Ogre::StringConverter::toString ( info.sampleRates[i] ));
    samplingRate->setIndexSelected(0);
}

void Tuner::onApplyPress(MyGUI::WidgetPtr _sender)
{
    MyGUI::ComboBoxPtr bufferSize = mGUI->findWidget<MyGUI::ComboBox>("bufferSize");
    MyGUI::ComboBoxPtr nofBuffers = mGUI->findWidget<MyGUI::ComboBox>("nofBuffers");
    MyGUI::ComboBoxPtr samplingRate = mGUI->findWidget<MyGUI::ComboBox>("samplingRate");
    bufferFrames = Ogre::StringConverter::parseUnsignedInt(bufferSize->getItemNameAt(bufferSize->getIndexSelected()));
    numberOfFrames = Ogre::StringConverter::parseUnsignedInt(nofBuffers->getItemNameAt(nofBuffers->getIndexSelected()));
    nn = bufferFrames * numberOfFrames;
    fs = Ogre::StringConverter::parseUnsignedInt(samplingRate->getItemNameAt(samplingRate->getIndexSelected()));

    lines1->clear();
    for (unsigned int i=0; i< nn/2; i++)
    {
        lines1->addPoint(0, 0, 0);
    }
    lines1->update();

    lines2->clear();
    for (unsigned int i=0; i<nn; i++)
    {
        lines2->addPoint(0, 0, 0);
    }
    lines2->update();

    snd->startStream(device,0,bufferFrames,numberOfFrames,fs);
    onScroll(0,0);
}

void Tuner::onScroll(MyGUI::WidgetPtr _sender, unsigned int _index)
{
    scale = 1.;
    MyGUI::HScrollPtr scroll;
    scroll = mGUI->findWidget<MyGUI::HScroll>("scale1");
    from = 5 + scroll->getScrollPosition() * 44100 / fs ;
    lineLow->clear();
    for(unsigned int i = 0; i<12; i++)
    {
        lineLow->addPoint(200. * from / nn - 50.,
                         i * 2 + 75,
                         0);
    }
    lineLow->update();

    scroll = mGUI->findWidget<MyGUI::HScroll>("sensitivity");
    sensitivity = (double)scroll->getScrollPosition() / 2.;
    lineSens->clear();
    for (unsigned int i=0; i<50; i++)
    {
        lineSens->addPoint((double)i * 2 - 50,
                          scale * sensitivity + 75,
                          0);
    }
    lineSens->update();
}

bool Tuner::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    //double arrowSpeed = 5.;
    currentCents += 5 * evt.timeSinceLastFrame * (cents - currentCents);
    snd->FFT();
    
    for (unsigned int i=0; i<nn/2; i++)
    {
        lines1->setPoint(i,
                         Ogre::Vector3(
                             200. * i / nn - 50.,
                             scale * snd->fftBuffer[i]+75,
                             0
                         )
                        );
    }
    
    for (unsigned int i=0; i<nn; i++)
    {
        lines2->setPoint(i,
                         Ogre::Vector3(
                             100. * i / nn - 50.,
                             70. * snd->soundBuffer[i]+35,
                             0
                         )
                        );
    }
    lines1->update();
    lines2->update();


    int maxPeakIndex = 0;// = std::max_element(dfourier,dfourier + bufferFrames * numberOfFrames/4) - dfourier;
    double realPeakIndex, pitch = 0;
    //int from = 100 * bufferFrames * numberOfFrames / fs - 1;
    for(unsigned int i=from; i < (nn >> 1) - 5; i++)
    {
        if(snd->fftBuffer[i]>sensitivity)
            if(snd->fftBuffer[i-2]<snd->fftBuffer[i-1] and snd->fftBuffer[i-1]<snd->fftBuffer[i] and snd->fftBuffer[i]>snd->fftBuffer[i+1] and snd->fftBuffer[i+1]>snd->fftBuffer[i+2])
            //if(i == std::max_element(dfourier + i -3 ,dfourier + i + 3 ) - dfourier)
            {
                maxPeakIndex = i;
                break;
            }
    }
    if(maxPeakIndex)
    {
        realPeakIndex = maxPeakIndex +
                        (snd->fftBuffer[maxPeakIndex + 1] - snd->fftBuffer[maxPeakIndex - 1]) /
                        (snd->fftBuffer[maxPeakIndex + 1] + snd->fftBuffer[maxPeakIndex - 1] + snd->fftBuffer[maxPeakIndex]);
        pitch = (double)fs * realPeakIndex / nn;
        note = 69 + floor(12 * log2( pitch / 440 ) + 0.5);
        cents = 1200. * log2( pitch / 440 ) - 100. * (note - 69);
        
        textNote->setCaption(noteNames[note % 12]);
        textCents->setCaption(Ogre::StringConverter::toString((int)cents));
        arrowNode->resetOrientation();
        arrowNode->pitch(Ogre::Angle(90));
        arrowNode->yaw(Ogre::Angle(-currentCents));
    }
    //window->setCaption(Ogre::StringConverter::toString((Ogre::Real)pitch) + "  -  " +
    //    Ogre::StringConverter::toString ( (Ogre::Real)(1200. * log2( pitch / 440 ) ) ));

    
    

    
    return true;
}

