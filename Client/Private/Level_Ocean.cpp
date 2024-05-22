#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\Level_Ocean.h"

#include "GameInstance.h"
#include "LandObject.h"
#include <Camera.h>
#include "TachoShop_Tile.h"
#include "Bush.h"
CLevel_Ocean::CLevel_Ocean(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Ocean::Initialize()
{
	m_iLevelIndex = LEVEL_OCEAN;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_LandObjects()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	//int horizontalTiles = 14; // 예시로 가로 13 타일
	//int verticalTiles = 2; // 예시로 세로 5 타일

	//if (FAILED(Ready_Layer_Tile(TEXT("Layer_TachoGround_Tile"), horizontalTiles, verticalTiles)))
	//	return E_FAIL;

	if (FAILED(ParseInitialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Ocean::Update(_float fTimeDelta)
{
}

HRESULT CLevel_Ocean::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨"));

	return S_OK;
}

HRESULT CLevel_Ocean::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_OCEAN, TEXT("Prototype_GameObject_Terrain"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Ocean::Ready_Layer_Sky(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"), strLayerTag)))
		return E_FAIL;
}

HRESULT CLevel_Ocean::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::CAMERA_DESC			CameraDesc{};

	CameraDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_OCEAN, TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_OCEAN, TEXT("Prototype_GameObject_Camera"), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Ocean::Ready_Layer_Tile(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles)
{
	CTachoShop_Tile::TILEDESC tileDesc{};

	float startX = 17.64f;
	float startY = 0.006f;
	float startZ = 29.510f;
	float spacing = 1.0f; // 각 타일 사이의 간격

	for (int z = 0; z < verticalTiles; ++z)
	{
		for (int x = 0; x < horizontalTiles; ++x)
		{
			tileDesc.startPosition = _float3(startX + x * spacing, startY, startZ + z * spacing);
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_OCEAN, TEXT("Prototype_GameObject_TachoGround_Tile"), strLayerTag, &tileDesc)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CLevel_Ocean::Ready_Layer_Bush(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles)
{
	CBush::BUSHDESC bushDecs{};

	float startX = 23.944f;
	float startY = 0.600f;
	float startZ = 25.468f;
	float spacing = 0.8f; // 각 타일 사이의 간격

	for (int z = 0; z < verticalTiles; ++z)
	{
		for (int x = 0; x < horizontalTiles; ++x)
		{
			bushDecs.startPosition = _float3(startX + x * spacing, startY, startZ + z * spacing);
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_OCEAN, TEXT("Prototype_GameObject_Bush"), strLayerTag, &bushDecs)))
				return E_FAIL;
		}
	}

	return S_OK;
}


HRESULT CLevel_Ocean::Ready_LandObjects()
{
	/*CLandObject::LANDOBJECT_DESC	Desc{};

	Desc.m_pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Get_Component(LEVEL_EDIT, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"), 0));
	Desc.m_pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_EDIT, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0));

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"), Desc)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_Ocean::Ready_Layer_Player(const _wstring& strLayerTag, CLandObject::LANDOBJECT_DESC& Desc)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_OCEAN, TEXT("Prototype_GameObject_Player"), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Ocean::ParseInitialize()
{
	if (FAILED(__super::ParseInitialize()))
		return E_FAIL;
}


CLevel_Ocean* CLevel_Ocean::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Ocean* pInstance = new CLevel_Ocean(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Ocean"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Ocean::Free()
{
	__super::Free();
}
