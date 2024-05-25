#include "stdafx.h"
#include "..\Public\UI_PlayerEquipSlot.h"

#include "GameInstance.h"

CUI_PlayerEquipSlot::CUI_PlayerEquipSlot(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_PlayerEquipSlot::CUI_PlayerEquipSlot(const CUI_PlayerEquipSlot& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_PlayerEquipSlot::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUI_PlayerEquipSlot::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 50.f;
	m_fSizeY = 50.f;
	m_fX = -530.f;
	m_fY = 250.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));


	m_fAlpha = 255.f;

	return S_OK;
}

void CUI_PlayerEquipSlot::Priority_Update(_float fTimeDelta)
{
}

void CUI_PlayerEquipSlot::Update(_float fTimeDelta)
{
	
}

void CUI_PlayerEquipSlot::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_PlayerEquipSlot::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

HRESULT CUI_PlayerEquipSlot::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_PlayerEquipSlot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CUI_PlayerEquipSlot* CUI_PlayerEquipSlot::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_PlayerEquipSlot* pInstance = new CUI_PlayerEquipSlot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_PlayerEquipSlot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_PlayerEquipSlot::Clone(void* pArg)
{
	CUI_PlayerEquipSlot* pInstance = new CUI_PlayerEquipSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_PlayerEquipSlot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerEquipSlot::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
