#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\Level_Tacho.h"

#include "GameInstance.h"
#include "LandObject.h"
#include <Camera.h>
#include "TachoShop_Tile.h"
#include "Bush.h"
#include "Npc.h"
#include "UI_Npc_Question_Effect.h"
#include "UI_Npc_Talk.h"
#include "TravelNpc.h"
CLevel_Tacho::CLevel_Tacho(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel_UI{ pGraphic_Device }
{
}

HRESULT CLevel_Tacho::Initialize()
{
	m_iLevelIndex = LEVEL_TACHO;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Npc(TEXT("Layer_Npc"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Npc_Question(TEXT("Layer_Npc_Question"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_TravelNpc(TEXT("Layer_Npc_TravelNpc"))))
		return E_FAIL;
	__super::Initialize();


	//int horizontalTiles = 14; // 예시로 가로 13 타일
	//int verticalTiles = 2; // 예시로 세로 5 타일

	//if (FAILED(Ready_Layer_Tile(TEXT("Layer_TachoGround_Tile"), horizontalTiles, verticalTiles)))
	//	return E_FAIL;

	if (FAILED(ParseInitialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Tacho::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	m_fElapsedTime += fTimeDelta; // 경과 시간 증가

	// 2초가 지나고, Level_Tacho_Start1 함수가 아직 호출되지 않았다면 호출
	if (m_fElapsedTime >= 2.0f && !m_bStart1Called)
	{
		Level_Tacho_Start1();
		m_bStart1Called = true;
	}
}

HRESULT CLevel_Tacho::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨"));

	return S_OK;
}

HRESULT CLevel_Tacho::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_TACHO, TEXT("Prototype_GameObject_Terrain"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tacho::Ready_Layer_Sky(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"), strLayerTag)))
		return E_FAIL;
}

HRESULT CLevel_Tacho::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::CAMERA_DESC			CameraDesc{};

	CameraDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_TACHO, TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_TACHO, TEXT("Prototype_GameObject_Camera"), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tacho::Ready_Layer_Tile(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles)
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
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_TACHO, TEXT("Prototype_GameObject_TachoGround_Tile"), strLayerTag, &tileDesc)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CLevel_Tacho::Ready_Layer_Bush(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles)
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
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_TACHO, TEXT("Prototype_GameObject_Bush"), strLayerTag, &bushDecs)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CLevel_Tacho::Ready_Layer_Npc(const _wstring& strLayerTag)
{
	CNpc::NPCDESC desc{};
	desc.npcName = TEXT("Tacho_NPC");
	desc.position = _float3(39.872f, 0.7f, 30.239f);
	desc.scale = _float3(1.5f, 1.5f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_TACHO, TEXT("Prototype_GameObject_Npc"), strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tacho::Ready_Layer_TravelNpc(const _wstring& strLayerTag)
{
	CTravelNpc::NPCDESC desc{};
	desc.position = _float3(54.672f, 1.0f, 28.539f);
	desc.scale = _float3(2.f, 2.f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_TACHO, TEXT("Prototype_GameObject_TravelNpc"), strLayerTag,&desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tacho::Ready_Layer_Npc_Question(const _wstring& strLayerTag)
{
	CUI_Npc_Question_Effect::EFFECT_DESC desc{};
	desc.position = _float3(39.872f, 2.f, 30.239f);
	desc.scale = _float3(1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_TACHO, TEXT("Prototype_GameObject_UI_Npc_Question_Effect"), strLayerTag, &desc)))
		return E_FAIL;
}

HRESULT CLevel_Tacho::Ready_Layer_Player(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_TACHO, TEXT("Prototype_GameObject_Player"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Tacho::ParseInitialize()
{
	if (FAILED(__super::ParseInitialize()))
		return E_FAIL;
}

void CLevel_Tacho::Level_Tacho_Start1()
{
	_uint level = m_pGameInstance->GetLoadingLevelIndex();

	CGameObject* gameObjectTalk = m_pGameInstance->Get_GameObject(level, TEXT("Layer_UI_Npc_Talk"));
	CUI_Npc_Talk* npcTalkUI = static_cast<CUI_Npc_Talk*>(gameObjectTalk);

	if (npcTalkUI)
	{
		npcTalkUI->SetIsNpcTalkOn(true);
		vector<pair<wstring, wstring>> messages = {
			{TEXT("144기 요정"), TEXT("SR을 시연회를 시작해볼까?")},
			{TEXT("144기 요정"), TEXT("ㄱㄱㄱ")}
		};
		npcTalkUI->SetNpcTalkMessages(messages);
	}
}


CLevel_Tacho* CLevel_Tacho::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Tacho* pInstance = new CLevel_Tacho(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Tacho"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Tacho::Free()
{
	__super::Free();
}
