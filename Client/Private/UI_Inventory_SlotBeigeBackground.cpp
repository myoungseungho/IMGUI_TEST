#include "stdafx.h"
#include "..\Public\UI_Inventory_SlotBeigeBackground.h"

#include "GameInstance.h"

CUI_Inventory_SlotBeigeBackground::CUI_Inventory_SlotBeigeBackground(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Inventory_SlotBeigeBackground::CUI_Inventory_SlotBeigeBackground(const CUI_Inventory_SlotBeigeBackground& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Inventory_SlotBeigeBackground::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CUI_Inventory_SlotBeigeBackground::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	offsetX = -0.289000571f;
	offsetY = -0.594999611f;
	offsetZ = 1.01f;

	m_fAlpha = 255.f;
	return S_OK;
}

void CUI_Inventory_SlotBeigeBackground::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
}

void CUI_Inventory_SlotBeigeBackground::Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
}

void CUI_Inventory_SlotBeigeBackground::Late_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����

	__super::Late_Update(fTimeDelta);

	_float3 currentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	currentPosition.x += offsetX;
	currentPosition.y += offsetY;
	currentPosition.z += offsetZ;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &currentPosition);
	m_pTransformCom->Set_Scaled(_float3(2.73F , 1.62F, 1.f));
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Inventory_SlotBeigeBackground::Render(_float fTimeDelta)
{
	if (!m_bIsOn) return S_OK; // m_bIsOn�� false�̸� �������� �������� ����
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

HRESULT CUI_Inventory_SlotBeigeBackground::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Inventory_SlotBeigeBackground"),
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

CUI_Inventory_SlotBeigeBackground* CUI_Inventory_SlotBeigeBackground::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Inventory_SlotBeigeBackground* pInstance = new CUI_Inventory_SlotBeigeBackground(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Inventory_SlotBeigeBackground"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Inventory_SlotBeigeBackground::Clone(void* pArg)
{
	CUI_Inventory_SlotBeigeBackground* pInstance = new CUI_Inventory_SlotBeigeBackground(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Inventory_SlotBeigeBackground"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory_SlotBeigeBackground::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
