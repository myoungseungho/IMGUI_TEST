#include "stdafx.h"
#include "..\Public\UI_Shop_ItemInfo_TextBox.h"

#include "GameInstance.h"

CUI_Shop_ItemInfo_TextBox::CUI_Shop_ItemInfo_TextBox(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Shop_ItemInfo_TextBox::CUI_Shop_ItemInfo_TextBox(const CUI_Shop_ItemInfo_TextBox& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Shop_ItemInfo_TextBox::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CUI_Shop_ItemInfo_TextBox::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 340.f;
	m_fSizeY = 265.f;
	m_fX = 450.f;
	m_fY = -140.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));


	m_fAlpha = 255.000000f;
	return S_OK;
}

void CUI_Shop_ItemInfo_TextBox::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
}

void CUI_Shop_ItemInfo_TextBox::Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
	
}

void CUI_Shop_ItemInfo_TextBox::Late_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
	__super::Late_Update(fTimeDelta);

	
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Shop_ItemInfo_TextBox::Render(_float fTimeDelta)
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

HRESULT CUI_Shop_ItemInfo_TextBox::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Shop_ItemInfo_TextBox"),
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

CUI_Shop_ItemInfo_TextBox* CUI_Shop_ItemInfo_TextBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Shop_ItemInfo_TextBox* pInstance = new CUI_Shop_ItemInfo_TextBox(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Shop_ItemInfo_TextBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Shop_ItemInfo_TextBox::Clone(void* pArg)
{
	CUI_Shop_ItemInfo_TextBox* pInstance = new CUI_Shop_ItemInfo_TextBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Shop_ItemInfo_TextBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Shop_ItemInfo_TextBox::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
