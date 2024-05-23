#include "stdafx.h"
#include "..\Public\UI_Heart_Player.h"

#include "GameInstance.h"

CUI_Heart_Player::CUI_Heart_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Heart_Player::CUI_Heart_Player(const CUI_Heart_Player& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Heart_Player::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUI_Heart_Player::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

void CUI_Heart_Player::Priority_Update(_float fTimeDelta)
{
}

void CUI_Heart_Player::Update(_float fTimeDelta)
{
}

void CUI_Heart_Player::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	_float3 currentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
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

	currentPosition.x += offsetX;
	currentPosition.y += offsetY;
	currentPosition.z += offsetZ;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &currentPosition);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Heart_Player::Render(_float fTimeDelta)
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

HRESULT CUI_Heart_Player::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Icon_HeartSymbol"),
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

CUI_Heart_Player* CUI_Heart_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Heart_Player* pInstance = new CUI_Heart_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Heart_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Heart_Player::Clone(void* pArg)
{
	CUI_Heart_Player* pInstance = new CUI_Heart_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Heart_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Heart_Player::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
