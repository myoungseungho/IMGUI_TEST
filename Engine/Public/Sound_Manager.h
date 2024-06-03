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

class CSound_Manager final : public CBase
{
private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	void Register_Sound(const std::wstring& filePath, const std::wstring& alias, int channel, _uint levelId, _uint soundType);
	void Play_Sound(const std::wstring& alias, _uint levelId, bool loop);
	void Stop_Sound(const std::wstring& alias, _uint levelId);
	void Set_Volume(const std::wstring& alias, _uint levelId, float volume);

private:
	FMOD_SYSTEM* m_pSoundSystem;
	map<wstring, FMOD_SOUND*>* m_soundMap;
	map<wstring, FMOD_CHANNEL*>* m_channelMap;
	_uint m_iNumLevels;

public:
	static CSound_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};


#endif

END