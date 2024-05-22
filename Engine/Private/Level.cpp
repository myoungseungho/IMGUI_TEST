#include "..\Public\Level.h"

#include "GameInstance.h"

CLevel::CLevel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device{ pGraphic_Device }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLevel::Initialize()
{
	return S_OK;
}

void CLevel::Update(_float fTimeDelta)
{

}

HRESULT CLevel::Render()
{
	return S_OK;
}

HRESULT CLevel::Clear_Resources()
{
	return m_pGameInstance->Clear_LevelResources(m_iLevelIndex);
}

HRESULT CLevel::ParseInitialize()
{
    // 파일 이름을 미리 배열로 정의
    const wchar_t* filePaths[] = {
        L"../Bin/LevelStaticObjects.txt",    // 0: LEVEL_STATIC
        L"../Bin/LevelLoadingObjects.txt",   // 1: LEVEL_LOADING
        L"../Bin/LevelLogoObjects.txt",      // 2: LEVEL_LOGO
        L"../Bin/LevelGameplayObjects.txt",  // 3: LEVEL_GAMEPLAY
        L"../Bin/LevelEditObjects.txt",      // 4: LEVEL_EDIT
        L"../Bin/LevelTachoObjects.txt",      // 5: LEVEL_TACHO
        L"../Bin/LevelSnowObjects.txt",      // 6: LEVEL_SNOW
        L"../Bin/LevelKoofuObjects.txt",      // 7: LEVEL_KOOFU
        L"../Bin/LevelBugObjects.txt",      // 8: LEVEL_Bug
        L"../Bin/DefaultLevelObjects.txt"    // 9: LEVEL_END (또는 기타)
    };

    // 배열 인덱스를 사용하여 파일 경로 가져오기
    wstring filePath;
    if (m_iLevelIndex >= 0 && m_iLevelIndex < _countof(filePaths)) {
        filePath = filePaths[m_iLevelIndex];
    }
    else {
        filePath = L"../Bin/DefaultLevelObjects.txt"; // 기본값
    }

    vector<FILEDATA>* pvecFileData = static_cast<vector<FILEDATA>*>(m_pGameInstance->LoadObjects(filePath.c_str()));
    if (pvecFileData == nullptr)
    {
        return E_FAIL;
    }

    size_t totalSize = pvecFileData->size() * sizeof(FILEDATA);
    for (auto& iter : *pvecFileData)
    {
        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(iter.levelIndex, iter.prototypeTag, iter.layerName, &iter)))
            return E_FAIL;
    }

    return S_OK;
}


void CLevel::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}
