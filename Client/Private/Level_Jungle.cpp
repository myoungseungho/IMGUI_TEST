#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\LEVEL_JUNGLE.h"

#include "GameInstance.h"
#include "LandObject.h"

#include <Camera.h>
#include <End_Orb.h>
#include <Rotation_Orb.h>
#include <Small_Orb.h>
#include <UnRotation_Orb.h>
#include <QuizMgr.h>
#include "TravelNpc.h"
CLevel_Jungle::CLevel_Jungle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel_UI{ pGraphic_Device }
{
}

HRESULT CLevel_Jungle::Initialize()
{
	m_iLevelIndex = LEVEL_JUNGLE;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_LandObjects()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_TravelNpc(TEXT("Layer_Npc_TravelNpc"))))
		return E_FAIL;

	__super::Initialize();

	if (FAILED(ParseInitialize()))
		return E_FAIL;

	// 사운드 재생
	m_pGameInstance->Play_Sound(L"BGM_JungleAreaField2", LEVEL_STATIC, true);
	// 볼륨 설정
	m_pGameInstance->Set_Volume(L"BGM_JungleAreaField2", LEVEL_STATIC, 0.2f);

	return S_OK;
}

void CLevel_Jungle::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

HRESULT CLevel_Jungle::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨"));

	return S_OK;
}

HRESULT CLevel_Jungle::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_Terrain"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Jungle::Ready_Layer_Sky(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_Sky"), strLayerTag)))
		return E_FAIL;
}

HRESULT CLevel_Jungle::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::CAMERA_DESC			CameraDesc{};

	CameraDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_Camera"), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_Jungle::Ready_Layer_TravelNpc(const _wstring& strLayerTag)
{
	CTravelNpc::NPCDESC desc{};
	desc.position = _float3(60.172f, 1.0f, 25.181f);
	desc.scale = _float3(2.f, 2.f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_TravelNpc"), strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_Jungle::Ready_LandObjects()
{
	CLandObject::LANDOBJECT_DESC	Desc{};

	Desc.m_pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"), 0));
	Desc.m_pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0));

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"), Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Jungle::Ready_Layer_Player(const _wstring& strLayerTag, CLandObject::LANDOBJECT_DESC& Desc)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_Player"), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Jungle::ParseInitialize()
{
	if (FAILED(__super::ParseInitialize()))
		return E_FAIL;
}


CLevel_Jungle* CLevel_Jungle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Jungle* pInstance = new CLevel_Jungle(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLEVEL_JUNGLE"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Jungle::Free()
{
	CQuizMgr::Get_QuizInstance()->Destroy_QuizInstance();

	__super::Free();
}
