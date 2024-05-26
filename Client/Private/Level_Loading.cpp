#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "Loader.h"
#include "Level_GamePlay.h"
#include "LEVEL_Jungle.h"
#include "Level_Tacho.h"
#include "Level_Snow.h"
#include "GameInstance.h"
#include "Level_Koofu.h"
#include "Level_Bug.h"
#include "Level_Edit.h"
CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Loading::Initialize(LEVELID eNextLevelID)
{
	m_iLevelIndex = LEVEL_LOADING;
	m_eNextLevelID = eNextLevelID;


	m_pLoader = CLoader::Create(m_pGraphic_Device, eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (FAILED(Ready_Layer_Loading(TEXT("Layer_Loading_BackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	if (true == m_pLoader->isFinished())
	{
		CLevel* pNextLevel = { nullptr };

		switch (m_eNextLevelID)
		{
		case LEVEL_TACHO:
			pNextLevel = CLevel_Tacho::Create(m_pGraphic_Device);
			break;
		case LEVEL_JUNGLE:
			pNextLevel = CLevel_Jungle::Create(m_pGraphic_Device);
			break;
		case LEVEL_SNOW:
			pNextLevel = CLevel_Snow::Create(m_pGraphic_Device);
			break;
		case LEVEL_KOOFU:
			pNextLevel = CLevel_Koofu::Create(m_pGraphic_Device);
			break;
		case LEVEL_BUG:
			pNextLevel = CLevel_Bug::Create(m_pGraphic_Device);
			break;
		case LEVEL_EDIT:
			pNextLevel = CLevel_Edit::Create(m_pGraphic_Device);
			break;
		}

		if (FAILED(m_pGameInstance->Change_Level(pNextLevel)))
			return;
	}
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Draw_Text();

	if (!m_pLoader->isFinished()) {
		
	}
	else {
		
	}

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Loading(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOADING, TEXT("Prototype_GameObject_UI_Loading_Background"), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOADING, TEXT("Prototype_GameObject_UI_Loading_Orgu_Run"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}


CLevel_Loading* CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVELID eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
