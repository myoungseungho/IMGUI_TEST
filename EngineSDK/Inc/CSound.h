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

class  CSound :public CComponent
{
private:
    static FMOD_SYSTEM* g_sound_system;

    FMOD_SOUND* m_sound;
    FMOD_CHANNEL* m_channel;

    float m_volume;
    FMOD_BOOL m_bool;
private:
    CSound(LPDIRECT3DDEVICE9 pGraphic_Device );
    CSound(const CSound& pPrototype);
    virtual ~CSound() = default;

public:
    virtual HRESULT Initialize_Prototype(const char* path, bool loop);
    virtual HRESULT Initialize(void* pArg);

    static int Init();
    static int Release();

public:
    int play();
    int pause();
    int resume();
    int stop();
    int volumeUp();
    int volumeDown();

    int Update();


public:
    static CSound* Create(LPDIRECT3DDEVICE9 pGraphic_Device , const char* path, bool loop);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

#endif

END
