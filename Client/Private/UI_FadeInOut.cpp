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
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

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

	// ���ϴ� ���� ������ �ʱ�ȭ
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
				m_fFadeElapsedTime = 0.f;  // �ð� �ʱ�ȭ
			}
		}
		else
		{
			m_fAlpha -= fAlphaStep;
			if (m_fAlpha <= m_fStartAlpha)
			{
				m_fAlpha = m_fStartAlpha;
				m_fCurrentRepeatTime += m_fFadeDuration * 2;  // �����·� ���ƿ��� �ð� �߰�
				if (m_bLoop)
				{
					if (m_fCurrentRepeatTime >= m_fRepeatTime)
					{
						m_bIsFading = false;
						m_bIsOn = false;  // �ݺ� ���� �� ��Ȱ��ȭ
					}
					else
					{
						m_bIncreasingAlpha = true;
						m_fFadeElapsedTime = 0.f;  // �ð� �ʱ�ȭ
					}
				}
				else
				{
					m_bIsFading = false;
					m_bIncreasingAlpha = true;
					m_bIsOn = false;  // ���̵� ���� �� ��Ȱ��ȭ
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
			m_bIsOn = false;  // ���̵� ���� �� ��Ȱ��ȭ
		}
	}
	else
	{
		m_fAlpha = m_fDefaultAlpha;
	}
}

void CUI_FadeInOut::Late_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� �������� �������� ����
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_FadeInOut::Render(_float fTimeDelta)
{
	if (!m_bIsOn) return S_OK; // m_bIsOn�� false�̸� �������� �������� ����
	__super::Begin_RenderState();

	/* �簢������ �ø��� ���� �ױ��ĸ� �̸� ��ġ�� ���ε��Ѵ�.  */
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
	m_bIsOn = true;  // ���̵� ���� �� Ȱ��ȭ
	m_bLoop = bLoop;  // �ݺ� ���� ����
	m_fRepeatTime = fRepeatTime;  // �ݺ� �ð� ����
	m_fCurrentRepeatTime = 0.f;  // ���� �ݺ� �ð� �ʱ�ȭ
}

void CUI_FadeInOut::StartFadingSingleDirection(_float fDuration, _float fStartAlpha, _float fEndAlpha)
{
	m_fFadeDuration = fDuration;
	m_fStartAlpha = fStartAlpha;
	m_fEndAlpha = fEndAlpha;
	m_fFadeElapsedTime = 0.f;
	m_bIsFadingSingleDirection = true;
	m_bIsOn = true;  // ���̵� ���� �� Ȱ��ȭ
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
