#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\Level_Bug.h"

#include "GameInstance.h"
#include "LandObject.h"
#include <Camera.h>
#include "TachoShop_Tile.h"
#include "Bush.h"
#include "Boss_Bug.h"
#include "Effect_Monster.h"
#include "TravelNpc.h"
CLevel_Bug::CLevel_Bug(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel_UI{ pGraphic_Device }
{
}

HRESULT CLevel_Bug::Initialize()
{
	m_iLevelIndex = LEVEL_BUG;

	Font_Initialize();

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;


	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	__super::Initialize();

	if (FAILED(ParseInitialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Boss_Bug(TEXT("Layer_Boss_Bug"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect_Light(TEXT("Layer_Effect_Light"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_TravelNpc(TEXT("Layer_TravelNpc"))))
		return E_FAIL;

	// 사운드 재생
	m_pGameInstance->Set_Volume(L"BGM_MoonPowerPlant", LEVEL_STATIC, 0.2f);
	m_pGameInstance->Play_Sound(L"BGM_MoonPowerPlant", LEVEL_STATIC, true);

	return S_OK;
}

void CLevel_Bug::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	// 시간 경과를 추적
	m_ElapsedTime += fTimeDelta;

	// 조건을 만족하고 아직 호출되지 않았다면, 메서드를 호출하고 플래그를 설정합니다.
	if (m_ElapsedTime >= m_DisplayDelay && !m_bBossHpUILayerReady)
	{
		if (FAILED(Ready_Layer_Boss_Bug_Hp_UI(TEXT("Layer_Boss_Bug_Hp_UI"))))
		{
		}
		m_bBossHpUILayerReady = true; // 한 번 호출 후 플래그 설정
	}

}

HRESULT CLevel_Bug::Render()
{
	// 일정 시간이 경과하지 않았으면 즉시 반환
	if (m_ElapsedTime < m_DisplayDelay)
		return S_OK;

	// 알파값을 계산 (경과 시간 기준으로 0에서 255로 증가)
	float alphaTime = m_ElapsedTime - m_DisplayDelay; // 알파값 증가 시간
	float alphaDuration = 1.0f; // 알파값이 0에서 255로 증가하는데 걸리는 시간 (예: 3초)
	int alpha = static_cast<int>((alphaTime / alphaDuration) * 255);
	if (alpha > 255) alpha = 255; // 최대값 255로 제한

	// 텍스트 형식화 및 렌더링
	wchar_t text[256];

	swprintf_s(text, L"%s", L"누에");
	RECT rect;
	SetRect(&rect, static_cast<int>(g_iWinSizeX * 0.5f + m_TextPosX), static_cast<int>(100.f + m_TextPosY), 0, 0); // 텍스트를 출력할 위치
	m_pBoss_Font->DrawText(
		NULL,
		text,
		-1,
		&rect,
		DT_NOCLIP,
		D3DCOLOR_ARGB(alpha, 255, 255, 255)
	);

	return S_OK;
}

HRESULT CLevel_Bug::Ready_Layer_Boss_Bug_Hp_UI(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_UI_HP_Green_Enemy"), strLayerTag)))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_UI_HP_Enemy"), strLayerTag)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Bug::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Terrain"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Bug::Ready_Layer_Sky(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"), strLayerTag)))
		return E_FAIL;
}

HRESULT CLevel_Bug::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::CAMERA_DESC			CameraDesc{};

	CameraDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_BUG, TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Camera"), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Bug::Ready_Layer_Tile(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles)
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
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_TachoGround_Tile"), strLayerTag, &tileDesc)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CLevel_Bug::Ready_Layer_Bush(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles)
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
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Bush"), strLayerTag, &bushDecs)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CLevel_Bug::Ready_Layer_Boss_Bug(const _wstring& strLayerTag)
{
	CBoss_Bug::BOSS_BUG_DESC			BossBug{};

	BossBug.iCurrentHp = 10;
	BossBug.iMaxHp = 10;
	BossBug.iAttack = 1;
	BossBug.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_BUG, TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Boss_Bug"), strLayerTag, &BossBug)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Bug::Ready_Layer_Effect_Light(const _wstring& strLayerTag)
{
	CEffect_Monster::EFFECT_MONSTER__DESC			Desc{};
	Desc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_BUG, TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Light"), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Bug::Ready_LandObjects()
{
	/*CLandObject::LANDOBJECT_DESC	Desc{};

	Desc.m_pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Get_Component(LEVEL_BUG, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"), 0));
	Desc.m_pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_BUG, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0));

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"), Desc)))
		return E_FAIL;*/

	return S_OK;
}

void CLevel_Bug::Font_Initialize()
{
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	// 폰트 설정 - CurrentPlayerMoney
	tFontInfo.Height = 40;
	tFontInfo.Width = 30;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	wcscpy_s(tFontInfo.FaceName, LF_FACESIZE, TEXT("Cafe24 Ssurround air OTF Light"));

	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pBoss_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentPlayerMoney_Font Failed");
		return;
	}
}

HRESULT CLevel_Bug::Ready_Layer_Player(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Player"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Bug::Ready_Layer_TravelNpc(const _wstring& strLayerTag)
{
	CTravelNpc::NPCDESC desc{};
	desc.position = _float3(45.149f, 1.0f, 23.688f);
	desc.scale = _float3(2.f, 2.f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_TravelNpc"), strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Bug::ParseInitialize()
{
	if (FAILED(__super::ParseInitialize()))
		return E_FAIL;
}


CLevel_Bug* CLevel_Bug::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Bug* pInstance = new CLevel_Bug(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Bug"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Bug::Free()
{
	Safe_Release(m_pBoss_Font);
	__super::Free();
}
