#include "stdafx.h"
#include "..\Public\UI_HP_BloodEffect.h"

#include "GameInstance.h"

CUI_HP_BloodEffect::CUI_HP_BloodEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_HP_BloodEffect::CUI_HP_BloodEffect(const CUI_HP_BloodEffect& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_HP_BloodEffect::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUI_HP_BloodEffect::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	offsetX = -0.04f;
	offsetY = -1.58f;
	offsetZ = 1.68f;

	m_pTextureCom->ChangeTextureColor(D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(255, 0, 0)); // 흰색을 빨간색으로
	//m_pTextureCom->ChangeTextureColor(D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255)); // 검정색을 빨간색으로

	return S_OK;
}

void CUI_HP_BloodEffect::Priority_Update(_float fTimeDelta)
{
}

void CUI_HP_BloodEffect::Update(_float fTimeDelta)
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

void CUI_HP_BloodEffect::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	_float3 currentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	currentPosition.x += offsetX;
	currentPosition.y += offsetY;
	currentPosition.z += offsetZ;

	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &currentPosition);
	m_pTransformCom->Set_Scaled(_float3(10.f, 5.f, 1.f));
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_HP_BloodEffect::Render(_float fTimeDelta)
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

HRESULT CUI_HP_BloodEffect::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_HP_BloodEffect"),
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


CUI_HP_BloodEffect* CUI_HP_BloodEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_HP_BloodEffect* pInstance = new CUI_HP_BloodEffect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_HP_BloodEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_HP_BloodEffect::Clone(void* pArg)
{
	CUI_HP_BloodEffect* pInstance = new CUI_HP_BloodEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_HP_BloodEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_HP_BloodEffect::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
