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

	__super::Initialize();

	if (FAILED(ParseInitialize()))
		return E_FAIL;


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

HRESULT CLevel_Jungle::Ready_Layer_End_Orb(const _wstring& strLayerTag)
{
	CEnd_Orb::END_ORB_DESC			ENDORBDESC{};

	ENDORBDESC.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_End_Stand"), TEXT("Com_Transform")));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_End_Orb"), strLayerTag, &ENDORBDESC)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Jungle::Ready_Layer_Rotation_Orb(const _wstring& strLayerTag)
{
	CRotation_Orb::ROTATION_ORB_DESC			ROTORBDESC{};

	ROTORBDESC.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_Rotation_Stand"), TEXT("Com_Transform")));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_Rotation_Orb"), strLayerTag, &ROTORBDESC)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Jungle::Ready_Layer_Small_Orb(const _wstring& strLayerTag)
{
	CSmall_Orb::SMALL_ORB_DESC			SAMALLORBDESC{};

	SAMALLORBDESC.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_Rotation_Stand"), TEXT("Com_Transform")));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_Small_Orb"), strLayerTag, &SAMALLORBDESC)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Jungle::Ready_Layer_UnRotation_Orb(const _wstring& strLayerTag)
{
	CUnRotation_Orb::UNROTATION_ORB_DESC			UNROTORBDESC{};

	UNROTORBDESC.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_UnRotation_Stand"), TEXT("Com_Transform")));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_UnRotation_Orb"), strLayerTag, &UNROTORBDESC)))
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
	__super::Free();
}
