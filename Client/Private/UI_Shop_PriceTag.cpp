#include "stdafx.h"
#include "..\Public\UI_Shop_PriceTag.h"

#include "GameInstance.h"

CUI_Shop_PriceTag::CUI_Shop_PriceTag(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Shop_PriceTag::CUI_Shop_PriceTag(const CUI_Shop_PriceTag& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Shop_PriceTag::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CUI_Shop_PriceTag::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 110.f;
	m_fSizeY = 60.f;
	m_fX = 550.f;
	m_fY = -45.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 1.f));

	m_fAlpha = 255.f;
	return S_OK;
}

void CUI_Shop_PriceTag::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
}

void CUI_Shop_PriceTag::Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
	if (GetAsyncKeyState('F') & 0x8000) {
		offsetX -= 5.f;
	}
	if (GetAsyncKeyState('H') & 0x8000) {
		offsetX += 5.f;
	}
	if (GetAsyncKeyState('T') & 0x8000) {
		offsetY += 5.f;
	}
	if (GetAsyncKeyState('G') & 0x8000) {
		offsetY -= 5.f;
	}
	if (GetAsyncKeyState('R') & 0x8000) {
		offsetZ -= 5.f;
	}
	if (GetAsyncKeyState('Y') & 0x8000) {
		offsetZ += 5.f;
	}
	if (GetAsyncKeyState('J') & 0x8000) {
		offsetXScale -= 5.f;
	}
	if (GetAsyncKeyState('K') & 0x8000) {
		offsetXScale += 5.f;
	}
	if (GetAsyncKeyState('N') & 0x8000) {
		offsetYScale -= 5.f;
	}
	if (GetAsyncKeyState('M') & 0x8000) {
		offsetYScale += 5.f;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		m_fAlpha += 1.f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		m_fAlpha -= 1.f;
	}
}

void CUI_Shop_PriceTag::Late_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
	__super::Late_Update(fTimeDelta);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX + offsetXScale, m_fSizeY + offsetYScale, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX + offsetX, m_fY + offsetY, 1.f));
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Shop_PriceTag::Render(_float fTimeDelta)
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

HRESULT CUI_Shop_PriceTag::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Shop_PriceTag"),
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

CUI_Shop_PriceTag* CUI_Shop_PriceTag::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Shop_PriceTag* pInstance = new CUI_Shop_PriceTag(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Shop_PriceTag"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Shop_PriceTag::Clone(void* pArg)
{
	CUI_Shop_PriceTag* pInstance = new CUI_Shop_PriceTag(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Shop_PriceTag"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Shop_PriceTag::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
