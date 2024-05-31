#include "Sound.h"

FMOD_SYSTEM* CSound::g_sound_system;

CSound::CSound()
{
    
}

HRESULT CSound::Sound_Create(const char* path, bool loop)
{
    if (loop) {
        FMOD_System_CreateSound(g_sound_system, path, FMOD_LOOP_NORMAL, 0, &m_sound);
    }
    else {
        FMOD_System_CreateSound(g_sound_system, path, FMOD_DEFAULT, 0, &m_sound);
    }

    m_channel = nullptr;
    m_volume = SOUND_DEFAULT;

    return S_OK;
}

int CSound::Init() {
    FMOD_System_Create(&g_sound_system, FMOD_VERSION);
    FMOD_System_Init(g_sound_system, 32, FMOD_INIT_NORMAL, 0);

    return 0;
}

int CSound::Release() {
    FMOD_System_Close(g_sound_system);
    FMOD_System_Release(g_sound_system);

    return 0;
}


int CSound::Sound_Play() {
    FMOD_System_PlaySound(g_sound_system, m_sound, 0, false, &m_channel);

    return 0;
}

int CSound::Sound_Pause() {
    FMOD_Channel_SetPaused(m_channel, true);

    return 0;
}

int CSound::Sound_Resume() {
    FMOD_Channel_SetPaused(m_channel, false);

    return 0;
}

int CSound::Sound_Stop() {
    FMOD_Channel_Stop(m_channel);

    return 0;
}

int CSound::Sound_VolumeUp() {
    if (m_volume < SOUND_MAX) {
        m_volume += SOUND_WEIGHT;
    }

    FMOD_Channel_SetVolume(m_channel, m_volume);

    return 0;
}

int CSound::Sound_VolumeDown() {
    if (m_volume > SOUND_MIN) {
        m_volume -= SOUND_WEIGHT;
    }

    FMOD_Channel_SetVolume(m_channel, m_volume);

    return 0;
}


int CSound::Sound_Update() {
    FMOD_Channel_IsPlaying(m_channel, &m_bool);

    if (m_bool) {
        FMOD_System_Update(g_sound_system);
    }

    return 0;
}

CSound* CSound::Create()
{
    return new CSound();
}

void CSound::Free()
{
    Release();
    FMOD_Sound_Release(m_sound);

    __super::Free();
}
