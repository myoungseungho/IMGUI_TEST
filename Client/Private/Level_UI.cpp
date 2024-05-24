#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\LEVEL_UI.h"

#include "GameInstance.h"

CLevel_UI::CLevel_UI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_UI::Initialize()
{
	if (FAILED(Ready_Layer_Inventory(TEXT("Layer_Inventory"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_UI::Update(_float fTimeDelta)
{
	if (Key_Down(VK_ESCAPE))
	{

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
	LEVELID loadingLevel = (LEVELID)m_pGameInstance->GetLoadingLevelIndex();

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(loadingLevel, TEXT("Prototype_GameObject_UI_Inventory"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

void CLevel_UI::Free()
{
	__super::Free();
}
