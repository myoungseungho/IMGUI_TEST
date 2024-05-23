#include "stdafx.h"
#include "..\Public\UI_HP_Green_Player.h"

#include "GameInstance.h"

CUI_HP_Green_Player::CUI_HP_Green_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_HP_Green_Player::CUI_HP_Green_Player(const CUI_HP_Green_Player& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_HP_Green_Player::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUI_HP_Green_Player::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	offsetX = -16.6f;
	offsetY = 0.1f;

	return S_OK;
}

void CUI_HP_Green_Player::Priority_Update(_float fTimeDelta)
{
}

void CUI_HP_Green_Player::Update(_float fTimeDelta)
{
}

void CUI_HP_Green_Player::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	_float3 currentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	currentPosition.x += offsetX;
	currentPosition.y += offsetY;
	currentPosition.z += offsetZ;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &currentPosition);
	m_pTransformCom->Set_Scaled(_float3(7.5f, 1.2f, 1.f));
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_HP_Green_Player::Render(_float fTimeDelta)
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

HRESULT CUI_HP_Green_Player::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_HP_Green"),
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

CUI_HP_Green_Player* CUI_HP_Green_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_HP_Green_Player* pInstance = new CUI_HP_Green_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_HP_Green_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_HP_Green_Player::Clone(void* pArg)
{
	CUI_HP_Green_Player* pInstance = new CUI_HP_Green_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_HP_Green_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_HP_Green_Player::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
