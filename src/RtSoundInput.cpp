#include "precompiled.h"
#include "RtSoundInput.h"

void RtSoundInput::FFT()
{
    static int i, j, n, m, mmax, istep;
    static double tempr, tempi, wtemp, theta, wpr, wpi, wr, wi;

    static const int isign = 1;
    

    for( i = 0; i < nn; i++ )
    {
        data[ i * 2 ] = 0;
        data[ i * 2 + 1 ] = soundBuffer[ i ];
    }

    n = nn << 1;
    j = 1;
    i = 1;
    while( i < n )
    {
        if( j > i )
        {
            tempr = data[ i ]; data[ i ] = data[ j ]; data[ j ] = tempr;
            tempr = data[ i - 1 ]; data[ i - 1 ] = data[ j - 1 ]; data[ j - 1 ] = tempr;
        }
        m = n >> 1;
        while( ( m >= 2 ) && ( j > m ) )
        {
            j = j - m;
            m = m >> 1;
        }
        j = j + m;
        i = i + 2;
    }
    mmax = 2;
    while( n > mmax )
    {
        istep = mmax << 1;
        theta = 2.0 * M_PI / ( isign * mmax );
        wtemp = sin( 0.5 * theta );
        wpr = -2.0 * wtemp * wtemp;
        wpi = sin( theta );
        wr = 1.0;
        wi = 0.0;
        m = 1;
        while( m < mmax )
        {
            i = m;
            while( i < n )
            {
                j = i + mmax;
                tempr = wr * data[ j - 1 ] - wi * data[ j ];
                tempi = wr * data[ j ] + wi * data[ j - 1 ];
                data[ j - 1 ] = data[ i - 1 ] - tempr;
                data[ j ] = data[ i ] - tempi;
                data[ i - 1 ] = data[ i - 1 ] + tempr;
                data[ i ] = data[ i ] + tempi;
                i = i + istep;
            }
            wtemp = wr;
            wr = wtemp * wpr - wi * wpi + wr;
            wi = wi * wpr + wtemp * wpi + wi;
            m += 2;
        }
        mmax = istep;
    }
    

    for( i = 0; i < ( nn / 2 ); i++ )
    {
        fftBuffer[ i ] = sqrt( data[ i * 2 ] * data[ i * 2 ] + data[ i * 2 + 1 ] * data[ i * 2 + 1 ] );
    }
}

RtSoundInput::RtSoundInput() :
    soundBuffer(0), fftBuffer(0), data(0)
{
    adac = new RtAudio();
}

void RtSoundInput::startStream(int d, int o, int b, int n, int f)
{
    iDevice = d;
    iOffset = o;
    bufferFrames = b;
    numberOfFrames = n;
    fs = f;
    nn = bufferFrames * numberOfFrames;
    //if ( adac->isStreamRunning() ) adac->stopStream();
    if ( adac->isStreamOpen() ) adac->closeStream();
    delete []soundBuffer;
    delete []fftBuffer;
    delete []data;
    
    soundBuffer = new MY_TYPE[nn];
    fftBuffer = new double [ nn / 2 ];
    for(int i = 0; i < nn; i++)
    {
        soundBuffer[i] = 0;
    }
    data = new double [ nn * 2 + 1 ];
    RtAudio::StreamParameters iParams, oParams;
    iParams.deviceId = iDevice;
    iParams.nChannels = 1;
    iParams.firstChannel = iOffset;
    RtAudio::StreamOptions options;
    //options.flags |= RTAUDIO_NONINTERLEAVED;

    try {
        unsigned int b = bufferFrames;
        adac->openStream( 0, &iParams, FORMAT, fs, &b, &inout, (void *)this, &options );
        adac->startStream();
    }
    catch ( RtError& e ) {
        std::cout << '\n' << e.getMessage() << '\n' << std::endl;
    }
}

RtSoundInput::~RtSoundInput()
{
    //if ( adac->isStreamRunning() ) adac->stopStream();
    if ( adac->isStreamOpen() ) adac->closeStream();
    delete adac;
    delete []soundBuffer;
    delete []fftBuffer;
    delete []data;
}

int RtSoundInput::inout( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
    double streamTime, RtAudioStreamStatus status, void *data )
{
    RtSoundInput *s = (RtSoundInput *)data;
    s->soundTime = streamTime;
    for(int i = 1; i < s->numberOfFrames; i++)
    {
        memcpy(s->soundBuffer + (i - 1) * s->bufferFrames, s->soundBuffer + i * s->bufferFrames, s->bufferFrames * sizeof(MY_TYPE));
    }
    memcpy(s->soundBuffer + (s->numberOfFrames - 1) * s->bufferFrames, inputBuffer, s->bufferFrames * sizeof(MY_TYPE));

    return 0;
}

std::string RtSoundInput::Info()
{
  // Create an api map.
  std::string out;
  std::map<int, std::string> apiMap;
  apiMap[RtAudio::MACOSX_CORE] = "OS-X Core Audio";
  apiMap[RtAudio::WINDOWS_ASIO] = "Windows ASIO";
  apiMap[RtAudio::WINDOWS_DS] = "Windows Direct Sound";
  apiMap[RtAudio::UNIX_JACK] = "Jack Client";
  apiMap[RtAudio::LINUX_ALSA] = "Linux ALSA";
  apiMap[RtAudio::LINUX_OSS] = "Linux OSS";
  apiMap[RtAudio::RTAUDIO_DUMMY] = "RtAudio Dummy";

  std::vector< RtAudio::Api > apis;
  RtAudio :: getCompiledApi( apis );

  out += "\nCompiled APIs:\n";
  for ( unsigned int i=0; i<apis.size(); i++ )
    out += "  " + apiMap[ apis[i] ] + "\n";

  RtAudio::DeviceInfo info;

  out += "\nCurrent API: " + apiMap[ adac->getCurrentApi() ] + "\n";

  unsigned int devices = adac->getDeviceCount();
  out += "\nFound " + Ogre::StringConverter::toString ( devices ) + " device(s) ...\n";

  for (unsigned int i=0; i<devices; i++) {
    info = adac->getDeviceInfo(i);
    out += "\nDevice Name = " + info.name + '\n';
    if ( info.probed == false )
      out += "Probe Status = UNsuccessful\n";
    else {
      out += "Probe Status = Successful\n";
      out += "Output Channels = " + Ogre::StringConverter::toString ( info.outputChannels ) + '\n';
      out += "Input Channels = " + Ogre::StringConverter::toString ( info.inputChannels ) + '\n';
      out += "Duplex Channels = " + Ogre::StringConverter::toString ( info.duplexChannels ) + '\n';
      if ( info.isDefaultOutput ) out += "This is the default output device.\n";
      else out += "This is NOT the default output device.\n";
      if ( info.isDefaultInput ) out += "This is the default input device.\n";
      else out += "This is NOT the default input device.\n";
      if ( info.nativeFormats == 0 )
        out += "No natively supported data formats(?)!";
      else {
        out += "Natively supported data formats:\n";
        if ( info.nativeFormats & RTAUDIO_SINT8 )
          out += "  8-bit int\n";
        if ( info.nativeFormats & RTAUDIO_SINT16 )
          out += "  16-bit int\n";
        if ( info.nativeFormats & RTAUDIO_SINT24 )
          out += "  24-bit int\n";
        if ( info.nativeFormats & RTAUDIO_SINT32 )
          out += "  32-bit int\n";
        if ( info.nativeFormats & RTAUDIO_FLOAT32 )
          out += "  32-bit float\n";
        if ( info.nativeFormats & RTAUDIO_FLOAT64 )
          out += "  64-bit float\n";
      }
      if ( info.sampleRates.size() < 1 )
        out += "No supported sample rates found!";
      else {
        out += "Supported sample rates = ";
        for (unsigned int j=0; j<info.sampleRates.size(); j++)
          out += Ogre::StringConverter::toString ( info.sampleRates[j] ) + " ";
      }
      out += "\n";
    }
  }
  out += "\n";
  return out;
}

unsigned int RtSoundInput::getDeviceCount()
{
    return adac->getDeviceCount();
}

RtAudio::DeviceInfo RtSoundInput::getDeviceInfo(unsigned int i)
{
    return adac->getDeviceInfo(i);
}
