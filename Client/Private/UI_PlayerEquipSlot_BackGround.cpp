#include "stdafx.h"
#include "..\Public\UI_PlayerEquipSlot_BackGround.h"

#include "GameInstance.h"

CUI_PlayerEquipSlot_BackGround::CUI_PlayerEquipSlot_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_PlayerEquipSlot_BackGround::CUI_PlayerEquipSlot_BackGround(const CUI_PlayerEquipSlot_BackGround& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_PlayerEquipSlot_BackGround::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CUI_PlayerEquipSlot_BackGround::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	UIDATA* uiData = reinterpret_cast<UIDATA*>(pArg);

	m_fSizeX = uiData->scale.x;
	m_fSizeY = uiData->scale.y;
	m_fX = uiData->position.x;
	m_fY = uiData->position.y;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));


	m_fAlpha = uiData->alpha;

	return S_OK;
}

void CUI_PlayerEquipSlot_BackGround::Priority_Update(_float fTimeDelta)
{
}

void CUI_PlayerEquipSlot_BackGround::Update(_float fTimeDelta)
{
	
}

void CUI_PlayerEquipSlot_BackGround::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_PlayerEquipSlot_BackGround::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	/* �簢������ �ø��� ���� �ױ��ĸ� �̸� ��ġ�� ���ε��Ѵ�.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

HRESULT CUI_PlayerEquipSlot_BackGround::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_PlayerEquipSlot_BackGround"),
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

CUI_PlayerEquipSlot_BackGround* CUI_PlayerEquipSlot_BackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_PlayerEquipSlot_BackGround* pInstance = new CUI_PlayerEquipSlot_BackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_PlayerEquipSlot_BackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_PlayerEquipSlot_BackGround::Clone(void* pArg)
{
	CUI_PlayerEquipSlot_BackGround* pInstance = new CUI_PlayerEquipSlot_BackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_PlayerEquipSlot_BackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerEquipSlot_BackGround::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
