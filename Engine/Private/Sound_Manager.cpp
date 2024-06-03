#include "..\Public\Sound_Manager.h"

#include "Layer.h"
#include "GameObject.h"
#include <locale>
#include <codecvt>

CSound_Manager::CSound_Manager()
{
}

HRESULT CSound_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	// ���� �迭 �Ҵ�
	m_soundMap = new map<wstring, FMOD_SOUND*>[iNumLevels];
	m_channelMap = new map<wstring, FMOD_CHANNEL*>[iNumLevels];

	// FMOD �ý��� �ʱ�ȭ
	FMOD_System_Create(&m_pSoundSystem, FMOD_VERSION);
	FMOD_System_Init(m_pSoundSystem, 32, FMOD_INIT_NORMAL, nullptr);

	return S_OK;
}

void CSound_Manager::Priority_Update(_float fTimeDelta)
{
	
}

void CSound_Manager::Update(_float fTimeDelta)
{
	
}

void CSound_Manager::Late_Update(_float fTimeDelta)
{
	
}

void CSound_Manager::Register_Sound(const std::wstring& filePath, const std::wstring& alias, _uint levelId, _uint soundType)
{
    if (levelId >= m_iNumLevels) return;

    // ���尡 �̹� ��ϵǾ� �ִ��� Ȯ��
    if (m_soundMap[levelId].find(alias) != m_soundMap[levelId].end())
    {
        return; // �̹� �����ϹǷ� ������� �ʰ� �������� ��ȯ
    }

    FMOD_SOUND* sound;

    // wstring�� string���� ��ȯ
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(filePath);

    if (FMOD_System_CreateSound(m_pSoundSystem, filePathStr.c_str(), FMOD_DEFAULT, nullptr, &sound) != FMOD_OK)
    {
        return;
    }

    m_soundMap[levelId][alias] = sound;
}

void CSound_Manager::Play_Sound(const std::wstring& alias, _uint levelId, bool loop)
{
    if (levelId >= m_iNumLevels) return;

    auto it = m_soundMap[levelId].find(alias);
    if (it == m_soundMap[levelId].end()) return;

    FMOD_CHANNEL* channel = nullptr;

    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    FMOD_Sound_SetMode(it->second, mode);

    if (FMOD_System_PlaySound(m_pSoundSystem, it->second, nullptr, false, &channel) != FMOD_OK)
    {
        return;
    }

    m_channelMap[levelId][alias] = channel;
}

void CSound_Manager::Stop_Sound(const std::wstring& alias, _uint levelId)
{
    if (levelId >= m_iNumLevels) return;

    auto it = m_channelMap[levelId].find(alias);
    if (it == m_channelMap[levelId].end()) return;

    if (FMOD_Channel_Stop(it->second) != FMOD_OK)
    {
        return;
    }

    m_channelMap[levelId].erase(it);
}

void CSound_Manager::Set_Volume(const std::wstring& alias, _uint levelId, float volume)
{
    if (levelId >= m_iNumLevels) return;

    auto it = m_channelMap[levelId].find(alias);
    if (it == m_channelMap[levelId].end()) return;

    if (FMOD_Channel_SetVolume(it->second, volume) != FMOD_OK)
    {
        return;
    }
}

void CSound_Manager::Stop_All_Sounds(_uint levelId)
{
    if (levelId >= m_iNumLevels) return;

    for (auto& pair : m_channelMap[levelId])
    {
        FMOD_Channel_Stop(pair.second);
    }
}


CSound_Manager* CSound_Manager::Create(_uint iNumLevels)
{
	CSound_Manager* pInstance = new CSound_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX(TEXT("Failed to Created : CSound_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSound_Manager::Free()
{
    for (_uint i = 0; i < m_iNumLevels; ++i)
    {
        for (auto& pair : m_soundMap[i])
        {
            FMOD_Sound_Release(pair.second);
        }
        m_soundMap[i].clear();
        m_channelMap[i].clear();
    }

    Safe_Delete_Array(m_soundMap);
    Safe_Delete_Array(m_channelMap);

    if (m_pSoundSystem)
    {
        FMOD_System_Close(m_pSoundSystem);
        FMOD_System_Release(m_pSoundSystem);
        m_pSoundSystem = nullptr;
    }

    __super::Free();
}
