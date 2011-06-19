#ifndef __RtSoundInput_H__
#define __RtSoundInput_H__

#include "RtAudio.h"

typedef double MY_TYPE;
const RtAudioFormat FORMAT = RTAUDIO_FLOAT64;

class RtSoundInput
{
public:
    void FFT();
    std::string Info();
    RtSoundInput();
    ~RtSoundInput();
    void startStream(int d, int o, int b, int n, int f);
    unsigned int getDeviceCount();
    RtAudio::DeviceInfo getDeviceInfo(unsigned int i);
    MY_TYPE *soundBuffer;
    double *fftBuffer;
    double soundTime;
private:
    int bufferFrames;
    int numberOfFrames;
    int fs;
    int nn;
    double *data;
    static int inout( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *data );
    unsigned int iDevice, iOffset;
    RtAudio *adac;
};

#endif

