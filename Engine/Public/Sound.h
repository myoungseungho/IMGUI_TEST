#pragma once
#ifndef _CSOUND_H_
#define _CSOUND_H_

#include <fmod.h>
#include "Engine_Defines.h"
#include "Component.h"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

BEGIN(Engine)

class ENGINE_DLL CSound final :public CBase
{
private:
    static FMOD_SYSTEM* g_sound_system;

    FMOD_SOUND* m_sound;
    FMOD_CHANNEL* m_channel;

    float m_volume;
    FMOD_BOOL m_bool;

  
private:
    CSound();
    virtual ~CSound();

public:
    virtual HRESULT Sound_Create(const char* path, bool loop);

    static int Init();
    static int Release();

public:
    int Sound_Play();
    int Sound_Pause();
    int Sound_Resume();
    int Sound_Stop();
    int Sound_VolumeUp();
    int Sound_VolumeDown();
    int Sound_Update();

private:
    _uint										m_iNumLevels = { 0 };

public:
    static CSound* Create();
    virtual void Free() override;
};

#endif

END
