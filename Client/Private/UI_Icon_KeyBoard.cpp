#include "stdafx.h"
#include "..\Public\UI_Icon_KeyBoard.h"

#include "GameInstance.h"

CUI_Icon_KeyBoard::CUI_Icon_KeyBoard(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Icon_KeyBoard::CUI_Icon_KeyBoard(const CUI_Icon_KeyBoard& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Icon_KeyBoard::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CUI_Icon_KeyBoard::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	offsetX = -21.7f;
	offsetY = -1.1f;

	return S_OK;
}

void CUI_Icon_KeyBoard::Priority_Update(_float fTimeDelta)
{
}

void CUI_Icon_KeyBoard::Update(_float fTimeDelta)
{
}

void CUI_Icon_KeyBoard::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	_float3 currentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	currentPosition.x += offsetX;
	currentPosition.y += offsetY;
	currentPosition.z += offsetZ;

	if (GetAsyncKeyState('F') & 0x8000) {
		offsetX -= 0.1f;
	}
	if (GetAsyncKeyState('H') & 0x8000) {
		offsetX += 0.1f;
	}
	if (GetAsyncKeyState('T') & 0x8000) {
		offsetY += 0.1f;
	}
	if (GetAsyncKeyState('G') & 0x8000) {
		offsetY -= 0.1f;
	}
	if (GetAsyncKeyState('R') & 0x8000) {
		offsetZ -= 0.1f;
	}
	if (GetAsyncKeyState('Y') & 0x8000) {
		offsetZ += 0.1f;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &currentPosition);
	m_pTransformCom->Set_Scaled(_float3(1.2f, 1.2f, 1.f));
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Icon_KeyBoard::Render(_float fTimeDelta)
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

HRESULT CUI_Icon_KeyBoard::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Icon_KeyBoard"),
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

CUI_Icon_KeyBoard* CUI_Icon_KeyBoard::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Icon_KeyBoard* pInstance = new CUI_Icon_KeyBoard(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Icon_KeyBoard"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Icon_KeyBoard::Clone(void* pArg)
{
	CUI_Icon_KeyBoard* pInstance = new CUI_Icon_KeyBoard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Icon_KeyBoard"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Icon_KeyBoard::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
