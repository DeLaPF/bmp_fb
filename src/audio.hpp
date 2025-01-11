#include <cstdint>

#include "SDL_audio.h"

class WaveManager {
public:
    WaveManager();
    ~WaveManager();

    void playWave(int freq, int volume);
    void stopSound();

private:
    static void audioCallback(void* userdata, uint8_t* stream, int len);

private:
    SDL_AudioDeviceID m_audioDevice;
    SDL_AudioSpec m_audioSpec;

    int16_t* m_samples;
    uint32_t m_numSamples;

    int m_curFreq;
    int m_curVol;
    float m_xOff;

    bool m_isPlaying;
};
