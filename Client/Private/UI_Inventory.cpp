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

	LEVELID currentLevel = (LEVELID)m_pGameInstance->GetCurrentLevelIndex();

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_FadeInOut"), TEXT("Layer_UI_FadeInOut"))))
		return E_FAIL;
	m_pUI_FadeInOut = static_cast<CUI_FadeInOut*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_FadeInOut")));
	Safe_AddRef(m_pUI_FadeInOut);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_Cursor"), TEXT("Layer_UI_Cursor"))))
		return E_FAIL;
	m_pUI_Cursor = static_cast<CUI_Cursor*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_Cursor")));
	Safe_AddRef(m_pUI_Cursor);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_Inventory_BackGround"), TEXT("Layer_UI_Inventory_BackGround"))))
		return E_FAIL;
	m_pUI_Inventory_BackGround = static_cast<CUI_Inventory_BackGround*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_Inventory_BackGround")));
	Safe_AddRef(m_pUI_Inventory_BackGround);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_Inventory_DotLine"), TEXT("Layer_UI_Inventory_DotLine"))))
		return E_FAIL;
	m_pUI_Inventory_DotLine = static_cast<CUI_Inventory_DotLine*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_Inventory_DotLine")));
	Safe_AddRef(m_pUI_Inventory_DotLine);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_Inventory_Slot"), TEXT("Layer_UI_Inventory_Slot"))))
		return E_FAIL;
	m_pUI_Inventory_Slot = static_cast<CUI_Inventory_Slot*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_Inventory_Slot")));
	Safe_AddRef(m_pUI_Inventory_Slot);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_Inventory_VerticalDotLine"), TEXT("Layer_UI_Inventory_VerticalDotLine"))))
		return E_FAIL;
	m_pUI_Inventory_VerticalDotLine = static_cast<CUI_Inventory_VerticalDotLine*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_Inventory_VerticalDotLine")));
	Safe_AddRef(m_pUI_Inventory_VerticalDotLine);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_ItemTabIcon_Caution"), TEXT("Layer_UI_ItemTabIcon_Caution"))))
		return E_FAIL;
	m_pUI_ItemTabIcon_Caution = static_cast<CUI_ItemTabIcon_Caution*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_ItemTabIcon_Caution")));
	Safe_AddRef(m_pUI_ItemTabIcon_Caution);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_ItemTabIcon_Food"), TEXT("Layer_UI_ItemTabIcon_Food"))))
		return E_FAIL;
	m_pUI_ItemTabIcon_Food = static_cast<CUI_ItemTabIcon_Food*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_ItemTabIcon_Food")));
	Safe_AddRef(m_pUI_ItemTabIcon_Food);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_ItemTabIcon_Hat"), TEXT("Layer_UI_ItemTabIcon_Hat"))))
		return E_FAIL;
	m_pUI_ItemTabIcon_Hat = static_cast<CUI_ItemTabIcon_Hat*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_ItemTabIcon_Hat")));
	Safe_AddRef(m_pUI_ItemTabIcon_Hat);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_ItemTabIcon_Leaf"), TEXT("Layer_UI_ItemTabIcon_Leaf"))))
		return E_FAIL;
	m_pUI_ItemTabIcon_Leaf = static_cast<CUI_ItemTabIcon_Leaf*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_ItemTabIcon_Leaf")));
	Safe_AddRef(m_pUI_ItemTabIcon_Leaf);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_Shop_ItemInfo_TextBox"), TEXT("Layer_UI_Shop_ItemInfo_TextBox"))))
		return E_FAIL;
	m_pUI_Shop_ItemInfo_TextBox = static_cast<CUI_Shop_ItemInfo_TextBox*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_Shop_ItemInfo_TextBox")));
	Safe_AddRef(m_pUI_Shop_ItemInfo_TextBox);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_Shop_PlayerCoin"), TEXT("Layer_UI_Shop_PlayerCoin"))))
		return E_FAIL;
	m_pUI_Shop_PlayerCoin = static_cast<CUI_Shop_PlayerCoin*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_Shop_PlayerCoin")));
	Safe_AddRef(m_pUI_Shop_PlayerCoin);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_Shop_PriceTag"), TEXT("Layer_UI_Shop_PriceTag"))))
		return E_FAIL;
	m_pUI_Shop_PriceTag = static_cast<CUI_Shop_PriceTag*>(m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_UI_Shop_PriceTag")));
	Safe_AddRef(m_pUI_Shop_PriceTag);

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
	Safe_Release(m_pUI_FadeInOut);
	Safe_Release(m_pUI_Cursor);
	Safe_Release(m_pUI_Inventory_BackGround);
	Safe_Release(m_pUI_Inventory_DotLine);
	Safe_Release(m_pUI_Inventory_Slot);
	Safe_Release(m_pUI_Inventory_VerticalDotLine);
	Safe_Release(m_pUI_ItemTabIcon_Caution);
	Safe_Release(m_pUI_ItemTabIcon_Food);
	Safe_Release(m_pUI_ItemTabIcon_Hat);
	Safe_Release(m_pUI_ItemTabIcon_Leaf);
	Safe_Release(m_pUI_Shop_ItemInfo_TextBox);
	Safe_Release(m_pUI_Shop_PlayerCoin);
	Safe_Release(m_pUI_Shop_PriceTag);

	__super::Free();
}

