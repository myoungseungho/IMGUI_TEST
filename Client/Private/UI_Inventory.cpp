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
	__super::Free();
}
