#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\Level_Edit.h"

#include "GameInstance.h"
#include "LandObject.h"

#include "Monster.h"
#include "Mon_Pocket.h"
#include "Boss_Bug.h"
#include "Boss_Koofu.h"

#include "Skill_Player.h"
#include "Skill_Bug_Bullet.h"

#include <Camera.h>

CLevel_Edit::CLevel_Edit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Edit::Initialize()
{
	m_iLevelIndex = LEVEL_EDIT;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;


	if (FAILED(Ready_LandObjects()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(ParseInitialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Edit::Update(_float fTimeDelta)
{
}

HRESULT CLevel_Edit::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨"));

	return S_OK;
}

HRESULT CLevel_Edit::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_EDIT, TEXT("Prototype_GameObject_Terrain"), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_EDIT, TEXT("Prototype_GameObject_Sky"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Edit::Ready_Layer_Camera(const _wstring& strLayerTag)
{

	CCamera::CAMERA_DESC			CameraDesc{};

	CameraDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_EDIT, TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_EDIT, TEXT("Prototype_GameObject_Camera"), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Edit::Ready_LandObjects()
{
	CLandObject::LANDOBJECT_DESC	Desc{};

	Desc.m_pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Get_Component(LEVEL_EDIT, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"), 0));
	Desc.m_pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_EDIT, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0));

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"), Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Edit::Ready_Layer_Player(const _wstring& strLayerTag, CLandObject::LANDOBJECT_DESC& Desc)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_EDIT, TEXT("Prototype_GameObject_Player"), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Edit::ParseInitialize()
{
	vector<FILEDATA>* pvecFileData = static_cast<vector<FILEDATA>*>(m_pGameInstance->LoadObjects(TEXT("../Bin/ObjectData.txt")));
	size_t totalSize = pvecFileData->size() * sizeof(FILEDATA);
	for (auto& iter : *pvecFileData)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(iter.levelIndex, iter.prototypeTag, iter.layerName, &iter)))
			return E_FAIL;
	}
	return S_OK;
}


CLevel_Edit* CLevel_Edit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Edit* pInstance = new CLevel_Edit(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Edit"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Edit::Free()
{
	__super::Free();
}
