#include "stdafx.h"
#include "..\Public\UI_FadeInOut.h"

#include "GameInstance.h"

CUI_FadeInOut::CUI_FadeInOut(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_FadeInOut::CUI_FadeInOut(const CUI_FadeInOut& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_FadeInOut::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUI_FadeInOut::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = 0.f;
	m_fY = 0.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));

	// 원하는 알파 값으로 초기화
	m_fAlpha = m_fDefaultAlpha;

	return S_OK;
}

void CUI_FadeInOut::Priority_Update(_float fTimeDelta)
{
}

void CUI_FadeInOut::Update(_float fTimeDelta)
{
	if (!m_bIsOn) return;

	if (m_bIsFading)
	{
		m_fFadeElapsedTime += fTimeDelta;
		float fAlphaStep = ((m_fEndAlpha - m_fStartAlpha) / m_fFadeDuration) * fTimeDelta;

		if (m_bIncreasingAlpha)
		{
			m_fAlpha += fAlphaStep;
			if (m_fAlpha >= m_fEndAlpha)
			{
				m_fAlpha = m_fEndAlpha;
				m_bIncreasingAlpha = false;
				m_fFadeElapsedTime = 0.f;  // 시간 초기화
			}
		}
		else
		{
			m_fAlpha -= fAlphaStep;
			if (m_fAlpha <= m_fStartAlpha)
			{
				m_fAlpha = m_fStartAlpha;
				m_fCurrentRepeatTime += m_fFadeDuration * 2;  // 원상태로 돌아오는 시간 추가
				if (m_bLoop)
				{
					if (m_fCurrentRepeatTime >= m_fRepeatTime)
					{
						m_bIsFading = false;
						m_bIsOn = false;  // 반복 종료 시 비활성화
					}
					else
					{
						m_bIncreasingAlpha = true;
						m_fFadeElapsedTime = 0.f;  // 시간 초기화
					}
				}
				else
				{
					m_bIsFading = false;
					m_bIncreasingAlpha = true;
					m_bIsOn = false;  // 페이딩 종료 시 비활성화
				}
			}
		}
	}
	else if (m_bIsFadingSingleDirection)
	{
		m_fFadeElapsedTime += fTimeDelta;
		float fAlphaStep = ((m_fEndAlpha - m_fStartAlpha) / m_fFadeDuration) * fTimeDelta;

		m_fAlpha += fAlphaStep;
		if ((fAlphaStep > 0 && m_fAlpha >= m_fEndAlpha) || (fAlphaStep < 0 && m_fAlpha <= m_fEndAlpha))
		{
			m_fAlpha = m_fEndAlpha;
			m_bIsFadingSingleDirection = false;
			m_bIsOn = false;  // 페이딩 종료 시 비활성화
		}
	}
	else
	{
		m_fAlpha = m_fDefaultAlpha;
	}
}

void CUI_FadeInOut::Late_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn이 false이면 렌더링을 수행하지 않음
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_FadeInOut::Render(_float fTimeDelta)
{
	if (!m_bIsOn) return S_OK; // m_bIsOn이 false이면 렌더링을 수행하지 않음
	__super::Begin_RenderState();

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureNum)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

HRESULT CUI_FadeInOut::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_FadeInOut"),
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

void CUI_FadeInOut::StartFading(_float fDuration, _float fStartAlpha, _float fEndAlpha, _bool bLoop, _float fRepeatTime)
{
	m_fFadeDuration = fDuration;
	m_fStartAlpha = fStartAlpha;
	m_fEndAlpha = fEndAlpha;
	m_fFadeElapsedTime = 0.f;
	m_bIsFading = true;
	m_bIncreasingAlpha = true;
	m_bIsOn = true;  // 페이딩 시작 시 활성화
	m_bLoop = bLoop;  // 반복 여부 설정
	m_fRepeatTime = fRepeatTime;  // 반복 시간 설정
	m_fCurrentRepeatTime = 0.f;  // 현재 반복 시간 초기화
}

void CUI_FadeInOut::StartFadingSingleDirection(_float fDuration, _float fStartAlpha, _float fEndAlpha)
{
	m_fFadeDuration = fDuration;
	m_fStartAlpha = fStartAlpha;
	m_fEndAlpha = fEndAlpha;
	m_fFadeElapsedTime = 0.f;
	m_bIsFadingSingleDirection = true;
	m_bIsOn = true;  // 페이딩 시작 시 활성화
}


CUI_FadeInOut* CUI_FadeInOut::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_FadeInOut* pInstance = new CUI_FadeInOut(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_HP_FadeInOut"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_FadeInOut::Clone(void* pArg)
{
	CUI_FadeInOut* pInstance = new CUI_FadeInOut(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_HP_FadeInOut"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_FadeInOut::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
