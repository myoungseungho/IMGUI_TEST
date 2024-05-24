#include "stdafx.h"
#include "..\Public\UI_Inventory_BackGround.h"

#include "GameInstance.h"

CUI_Inventory_BackGround::CUI_Inventory_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Inventory_BackGround::CUI_Inventory_BackGround(const CUI_Inventory_BackGround& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Inventory_BackGround::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CUI_Inventory_BackGround::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	offsetX = -0.864f;
	offsetY = 0.f;
	offsetZ = 1.f;

	return S_OK;
}

void CUI_Inventory_BackGround::Priority_Update(_float fTimeDelta)
{
}

void CUI_Inventory_BackGround::Update(_float fTimeDelta)
{

}

void CUI_Inventory_BackGround::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	_float3 currentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	currentPosition.x += offsetX;
	currentPosition.y += offsetY;
	currentPosition.z += offsetZ;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &currentPosition);
	m_pTransformCom->Set_Scaled(_float3(0.1f, 0.1f, 1.f));
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Inventory_BackGround::Render(_float fTimeDelta)
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

HRESULT CUI_Inventory_BackGround::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Inventory_BackGround"),
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

CUI_Inventory_BackGround* CUI_Inventory_BackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Inventory_BackGround* pInstance = new CUI_Inventory_BackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Inventory_BackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Inventory_BackGround::Clone(void* pArg)
{
	CUI_Inventory_BackGround* pInstance = new CUI_Inventory_BackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Inventory_BackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory_BackGround::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}