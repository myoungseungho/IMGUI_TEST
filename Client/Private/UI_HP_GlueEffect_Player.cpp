#include "stdafx.h"
#include "..\Public\UI_HP_GlueEffect_Player.h"

#include "GameInstance.h"

CUI_HP_GlueEffect_Player::CUI_HP_GlueEffect_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_HP_GlueEffect_Player::CUI_HP_GlueEffect_Player(const CUI_HP_GlueEffect_Player& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_HP_GlueEffect_Player::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUI_HP_GlueEffect_Player::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	offsetX = -0.654999554f;
	offsetY = -0.00499999989f;
	offsetZ = 0.990000010f;

	return S_OK;
}

void CUI_HP_GlueEffect_Player::Priority_Update(_float fTimeDelta)
{
}

void CUI_HP_GlueEffect_Player::Update(_float fTimeDelta)
{
	

	// Update alpha value
	m_fElapsedTime += fTimeDelta;
	float fAlphaStep = ((m_fMaxAlpha - m_fMinAlpha) / m_fAlphaAnimationDuration) * fTimeDelta;

	if (m_bIncreasingAlpha)
	{
		m_fAlpha += fAlphaStep;
		if (m_fAlpha >= m_fMaxAlpha)
		{
			m_fAlpha = m_fMaxAlpha;
			m_bIncreasingAlpha = false;
		}
	}
	else
	{
		m_fAlpha -= fAlphaStep;
		if (m_fAlpha <= m_fMinAlpha)
		{
			m_fAlpha = m_fMinAlpha;
			m_bIncreasingAlpha = true;
		}
	}
}

void CUI_HP_GlueEffect_Player::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	_float3 currentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	currentPosition.x += offsetX;
	currentPosition.y += offsetY;
	currentPosition.z += offsetZ;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &currentPosition);
	m_pTransformCom->Set_Scaled(_float3(0.68f, 0.1f, 1.f));
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_HP_GlueEffect_Player::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(static_cast<DWORD>(m_fAlpha), 255, 255, 255));
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	// 알파 블렌딩 원래 상태로 복원
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return S_OK;
}

HRESULT CUI_HP_GlueEffect_Player::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_HP_GlueEffect_Player"),
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

CUI_HP_GlueEffect_Player* CUI_HP_GlueEffect_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_HP_GlueEffect_Player* pInstance = new CUI_HP_GlueEffect_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_HP_GlueEffect_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_HP_GlueEffect_Player::Clone(void* pArg)
{
	CUI_HP_GlueEffect_Player* pInstance = new CUI_HP_GlueEffect_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_HP_GlueEffect_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_HP_GlueEffect_Player::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
