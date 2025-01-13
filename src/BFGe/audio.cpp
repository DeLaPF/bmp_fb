#include "audio.hpp"

#include <cmath>
#include <iostream>

static const float tau = 6.28318f;

static void genWaveSamples(int16_t* samples, uint32_t len, float& x, int freq, int volume)
{
    const float domain = tau/freq;
    const float sampleStepSize = 0.1f / freq;
    for (uint32_t i = 0; i < len; i++) {
        samples[i] = (int16_t)(sin(x * freq) * volume);
        x += sampleStepSize;
        if (x > domain) { x -= domain; }
    }
}

WaveManager::WaveManager()
{
    SDL_zero(this->m_audioSpec);
    this->m_audioSpec.freq = 44100;
    this->m_audioSpec.format = AUDIO_S16SYS;
    this->m_audioSpec.channels = 1;
    this->m_audioSpec.samples = 1024;
    this->m_audioSpec.userdata = this;
    this->m_audioSpec.callback = audioCallback;

    this->m_audioDevice = SDL_OpenAudioDevice(NULL, 0, &this->m_audioSpec, NULL, 0);
    if (!this->m_audioDevice) {
        std::cout << "Error [SDL_AUDIO]: could not open device" << std::endl;
    }

    this->m_numSamples = this->m_audioSpec.samples;
    this->m_samples = (int16_t*)malloc(this->m_numSamples * sizeof(int16_t));
    this->m_isPlaying = false;
    this->m_xOff = 0.0f;
}

WaveManager::~WaveManager() {
    SDL_CloseAudioDevice(this->m_audioDevice);
    free(this->m_samples);
}

void WaveManager::audioCallback(void* userdata, uint8_t* stream, int len)
{
    WaveManager* wM = static_cast<WaveManager*>(userdata);
    genWaveSamples((int16_t*)stream, len/2, wM->m_xOff, wM->m_curFreq, wM->m_curVol);
}

void WaveManager::playWave(int freq, int volume)
{
    if (this->m_isPlaying) { return; }
    this->m_isPlaying = true;
    this->m_curFreq = freq;
    this->m_curVol = volume;

    genWaveSamples(this->m_samples, this->m_numSamples, this->m_xOff, freq, volume);

    SDL_QueueAudio(this->m_audioDevice, this->m_samples, this->m_numSamples * sizeof(int16_t));
    SDL_PauseAudioDevice(this->m_audioDevice, 0);
}

void WaveManager::stopSound()
{
    if (!this->m_isPlaying) { return; }
    this->m_isPlaying = false;

    SDL_PauseAudioDevice(this->m_audioDevice, 1);
    SDL_ClearQueuedAudio(this->m_audioDevice);
}
