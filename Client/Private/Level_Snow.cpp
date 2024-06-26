#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\Level_Snow.h"

#include "GameInstance.h"
#include "LandObject.h"
#include <Camera.h>
#include "TachoShop_Tile.h"
#include "Bush.h"
#include "Monster.h"
#include "TravelNpc.h"
#include "Effect_Monster.h"

CLevel_Snow::CLevel_Snow(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel_UI{ pGraphic_Device }
{
}

HRESULT CLevel_Snow::Initialize()
{
	m_iLevelIndex = LEVEL_SNOW;
	
	__super::Initialize();

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster_Trash_Slime(TEXT("Layer_Monster_Trash_Slime"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster_Bear_Solider(TEXT("Layer_Monster_Bear_Solider"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster_Bear_Cannon(TEXT("Layer_Monster_Bear_Cannon"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_TravelNpc(TEXT("Layer_TravelNpc"))))
		return E_FAIL;

	if (FAILED(ParseInitialize()))
		return E_FAIL;
	
	// 사운드 재생
	m_pGameInstance->Play_Sound(L"BGM_SnowAreaField2", LEVEL_STATIC, true);

	// 볼륨 설정
	m_pGameInstance->Set_Volume(L"BGM_SnowAreaField2", LEVEL_STATIC, 0.2f);

	//int horizontalTiles = 14; // 예시로 가로 13 타일
	//int verticalTiles = 2; // 예시로 세로 5 타일

	//if (FAILED(Ready_Layer_Tile(TEXT("Layer_TachoGround_Tile"), horizontalTiles, verticalTiles)))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_Snow::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

HRESULT CLevel_Snow::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨"));

	return S_OK;
}

HRESULT CLevel_Snow::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_Terrain"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Snow::Ready_Layer_Sky(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"), strLayerTag)))
		return E_FAIL;
}

HRESULT CLevel_Snow::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::CAMERA_DESC			CameraDesc{};

	CameraDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_SNOW, TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_Camera"), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Snow::Ready_Layer_Monster_Trash_Slime(const _wstring& strLayerTag)
{
	CMonster::MONSTER_DESC			MonsterDesc{};
	MonsterDesc.iCurrentHp = 3;
	MonsterDesc.iAttack = 1;
	MonsterDesc.pTargetTransform= dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_SNOW, TEXT("Layer_Player"), TEXT("Com_Transform")));

	for (int i = 1; i <= 4; ++i)
	{
		MonsterDesc.iSpawnNum = i; 

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_Monster_Trash_Slime"), strLayerTag, &MonsterDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_Snow::Ready_Layer_Monster_Bear_Solider(const _wstring& strLayerTag)
{
	CMonster::MONSTER_DESC			MonsterDesc{};
	MonsterDesc.iCurrentHp = 10;
	MonsterDesc.iAttack = 1;
	MonsterDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_SNOW, TEXT("Layer_Player"), TEXT("Com_Transform")));

	for (int i = 1 ; i <= 4; ++i)
	{
		MonsterDesc.iSpawnNum = i;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_Monster_Bear_Solider"), strLayerTag, &MonsterDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_Snow::Ready_Layer_Monster_Bear_Cannon(const _wstring& strLayerTag)
{
	CMonster::MONSTER_DESC			MonsterDesc{};
	MonsterDesc.iCurrentHp = 5;
	MonsterDesc.iAttack = 1;
	MonsterDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_SNOW, TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_Monster_Bear_Cannon"), strLayerTag, &MonsterDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CLevel_Snow::Ready_Layer_Tile(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles)
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
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_TachoGround_Tile"), strLayerTag, &tileDesc)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CLevel_Snow::Ready_Layer_Bush(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles)
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
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_Bush"), strLayerTag, &bushDecs)))
				return E_FAIL;
		}
	}

	return S_OK;
}
HRESULT CLevel_Snow::Ready_Layer_TravelNpc(const _wstring& strLayerTag)
{
	CTravelNpc::NPCDESC desc{};
	desc.position = _float3(34.328f, 1.1f, 38.987f);
	desc.scale = _float3(2.f, 2.f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_TravelNpc"), strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_Snow::Ready_LandObjects()
{
	/*CLandObject::LANDOBJECT_DESC	Desc{};

	Desc.m_pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"), 0));
	Desc.m_pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0));

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"), Desc)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_Snow::Ready_Layer_Player(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_Player"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Snow::ParseInitialize()
{
	if (FAILED(__super::ParseInitialize()))
		return E_FAIL;
}


CLevel_Snow* CLevel_Snow::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Snow* pInstance = new CLevel_Snow(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Snow"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Snow::Free()
{
	__super::Free();
}
