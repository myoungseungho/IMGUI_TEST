#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\LEVEL_UI.h"
#include "UI_Inventory.h"
#include "GameInstance.h"
#include "UI_MapGuide.h"

CLevel_UI::CLevel_UI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_UI::Initialize()
{
	if (FAILED(Ready_Layer_Inventory(TEXT("Layer_ZUI_Inventory"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Npc_Talk(TEXT("Layer_UI_Npc_Talk"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Map_Guide(TEXT("Layer_UI_Map_Guide"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_UI::Update(_float fTimeDelta)
{
	//ESC를 눌렀을 때 인벤토리 사본이 이미 존재한다면 가져오고
	//없다면 사본 하나 만든다.
	if (!m_bIsAllowInventory)
		return;

	if (Key_Down(VK_ESCAPE))
	{
		CGameObject* inventoryGameObject = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_ZUI_Inventory"));
		if (inventoryGameObject == nullptr)
		{
			Ready_Layer_Inventory(TEXT("Layer_ZUI_Inventory"));
		}
		else
		{
			CInventory* inventory = static_cast<CInventory*>(inventoryGameObject);
			inventory->SetInventoryOnOff();
		}
	}
}

HRESULT CLevel_UI::Render()
{
	return S_OK;
}


_bool CLevel_UI::Key_Down(_uint _iKey)
{
	if ((!m_bKeyState[_iKey]) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}

	return false;
}

HRESULT CLevel_UI::Ready_Layer_Inventory(const _wstring& strLayerTag)
{
	//LEVELID loadingLevel = (LEVELID)m_pGameInstance->GetLoadingLevelIndex();

	CGameObject* inventoryGameObject = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_ZUI_Inventory"));
	if (inventoryGameObject == nullptr)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory"), strLayerTag)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_UI::Ready_Layer_Npc_Talk(const _wstring& strLayerTag)
{
	LEVELID loadingLevel = (LEVELID)m_pGameInstance->GetLoadingLevelIndex();

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(loadingLevel, TEXT("Prototype_GameObject_UI_Npc_Talk"), strLayerTag)))
		return E_FAIL;
}

HRESULT CLevel_UI::Ready_Layer_Map_Guide(const _wstring& strLayerTag)
{
	LEVELID loadingLevel = (LEVELID)m_pGameInstance->GetLoadingLevelIndex();

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(loadingLevel, TEXT("Prototype_GameObject_UI_MapGuide"), strLayerTag)))
		return E_FAIL;

	CGameObject* gameObjectTalk = m_pGameInstance->Get_GameObject(loadingLevel, TEXT("Layer_UI_Map_Guide"));
	CUI_MapGuide* mapGuideUI = static_cast<CUI_MapGuide*>(gameObjectTalk);

	if (mapGuideUI)
	{
		mapGuideUI->SetIsNpcTalkOn(true);

		std::wstring mapName;
		switch (loadingLevel)
		{
		case LEVEL_TACHO:
			mapName = TEXT("태초 마을");
			break;
		case LEVEL_JUNGLE:
			mapName = TEXT("정글");
			break;
		case LEVEL_SNOW:
			mapName = TEXT("설산");
			break;
		case LEVEL_KOOFU:
			mapName = TEXT("쿠푸 보스");
			break;
		case LEVEL_BUG:
			mapName = TEXT("누에 보스");
			break;
		default:
			mapName = TEXT("알 수 없음");
			break;
		}

		mapGuideUI->SetMapGuideText(mapName);
	}
}


void CLevel_UI::Free()
{
	__super::Free();
}
