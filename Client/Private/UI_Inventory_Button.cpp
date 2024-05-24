#include "stdafx.h"
#include "..\Public\UI_Inventory_Button.h"

#include "GameInstance.h"

CUI_Inventory_Button::CUI_Inventory_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Inventory_Button::CUI_Inventory_Button(const CUI_Inventory_Button& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Inventory_Button::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CUI_Inventory_Button::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 180.f;
	m_fSizeY = 90.f;
	m_fX = 150.f;
	m_fY = 210.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 1.f));

	m_fAlpha = 255.f;
	return S_OK;
}

void CUI_Inventory_Button::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
}

void CUI_Inventory_Button::Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
	
}

void CUI_Inventory_Button::Late_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����

	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Inventory_Button::Render(_float fTimeDelta)
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

HRESULT CUI_Inventory_Button::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Inventory_Button"),
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

CUI_Inventory_Button* CUI_Inventory_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Inventory_Button* pInstance = new CUI_Inventory_Button(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Inventory_Button"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Inventory_Button::Clone(void* pArg)
{
	CUI_Inventory_Button* pInstance = new CUI_Inventory_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Inventory_Button"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory_Button::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
