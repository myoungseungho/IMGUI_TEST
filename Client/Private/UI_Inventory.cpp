#include "stdafx.h"
#include "..\Public\UI_Inventory.h"

#include "GameInstance.h"

CUI_Inventory::CUI_Inventory(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CUI_Inventory::CUI_Inventory(const CUI_Inventory& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CUI_Inventory::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUI_Inventory::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.0f, 1.f);


	LEVELID currentLevel = (LEVELID)m_pGameInstance->GetLoadingLevelIndex();

	auto AddUIObject = [&](const TCHAR* prototypeTag, const TCHAR* layerTag, void* pArg = nullptr) -> HRESULT {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, prototypeTag, layerTag, pArg)))
			return E_FAIL;
		CUIObject* pUIObject = static_cast<CUIObject*>(m_pGameInstance->Get_GameObject(currentLevel, layerTag));
		if (!pUIObject)
			return E_FAIL;
		Safe_AddRef(pUIObject);
		m_vecUIObject.push_back(pUIObject);
		return S_OK;
		};

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_FadeInOut"), TEXT("Layer_UI_FadeInOut"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Cursor"), TEXT("Layer_UI_Cursor"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_BackGround"), TEXT("Layer_UI_Inventory_BackGround"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_DotLine"), TEXT("Layer_UI_Inventory_DotLine"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_Slot"), TEXT("Layer_UI_Inventory_Slot"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_VerticalDotLine"), TEXT("Layer_UI_Inventory_VerticalDotLine"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_ItemTabIcon_Caution"), TEXT("Layer_UI_ItemTabIcon_Caution"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_ItemTabIcon_Food"), TEXT("Layer_UI_ItemTabIcon_Food"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_ItemTabIcon_Hat"), TEXT("Layer_UI_ItemTabIcon_Hat"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_ItemTabIcon_Leaf"), TEXT("Layer_UI_ItemTabIcon_Leaf"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Shop_ItemInfo_TextBox"), TEXT("Layer_UI_Shop_ItemInfo_TextBox"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Shop_PlayerCoin"), TEXT("Layer_UI_Shop_PlayerCoin"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Shop_PriceTag"), TEXT("Layer_UI_Shop_PriceTag"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_SlotBeigeBackground"), TEXT("Layer_UI_Inventory_SlotBeigeBackground"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_Button"), TEXT("Layer_UI_Inventory_Button"))))
		return E_FAIL;

	return S_OK;
}

void CUI_Inventory::Priority_Update(_float fTimeDelta)
{
}

void CUI_Inventory::Update(_float fTimeDelta)
{

}

void CUI_Inventory::Late_Update(_float fTimeDelta)
{

}

HRESULT CUI_Inventory::Render(_float fTimeDelta)
{
	return S_OK;
}

CUI_Inventory* CUI_Inventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Inventory* pInstance = new CUI_Inventory(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Inventory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Inventory::Clone(void* pArg)
{
	CUI_Inventory* pInstance = new CUI_Inventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Inventory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory::Free()
{
	for (auto& pUIObject : m_vecUIObject)
	{
		Safe_Release(pUIObject);
	}
	m_vecUIObject.clear();

	__super::Free();
}

