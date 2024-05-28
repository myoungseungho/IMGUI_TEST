#include "stdafx.h"
#include "..\Public\UI_Npc_Question_Effect.h"

#include "GameInstance.h"

CUI_Npc_Question_Effect::CUI_Npc_Question_Effect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CUI_Npc_Question_Effect::CUI_Npc_Question_Effect(const CUI_Npc_Question_Effect& Prototype)
	: CGameObject{ Prototype }
{
}

void CUI_Npc_Question_Effect::SetIsOn(bool _isOn)
{
	m_bIsOn = _isOn;
	m_fElapsedTime = 0.0f;

	if (_isOn)
	{
		m_fStartY = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
		m_fEndY = m_fStartY + 10.0f; // 목표 Y 위치 설정
		m_bIsAnimatingUp = true;
		m_bIsAnimatingDown = false;
	}
	else
	{
		m_bIsAnimatingUp = false;
		m_bIsAnimatingDown = true;
	}
}

HRESULT CUI_Npc_Question_Effect::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUI_Npc_Question_Effect::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	EFFECT_DESC* effectDesc = reinterpret_cast<EFFECT_DESC*>(pArg);

	m_pTransformCom->Set_Scaled(effectDesc->scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &effectDesc->position);
	m_fStartY = effectDesc->position.y;

	// 원하는 알파 값으로 초기화
	m_fAlpha = 125.f;

	return S_OK;
}

void CUI_Npc_Question_Effect::Priority_Update(_float fTimeDelta)
{
}

void CUI_Npc_Question_Effect::Update(_float fTimeDelta)
{
	m_fElapsedTime += fTimeDelta;

	if (m_bIsAnimatingUp)
	{
		float progress = m_fElapsedTime / m_fAnimationDuration;
		if (progress > 1.0f) progress = 1.0f;

		_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPosition.y = m_fStartY + (m_fEndY - m_fStartY) * progress;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPosition);

		if (m_fElapsedTime >= m_fAnimationDuration)
		{
			m_bIsAnimatingUp = false;
			m_fElapsedTime = 0.0f;
		}
	}
	else if (m_bIsAnimatingDown)
	{
		float progress = m_fElapsedTime / m_fAnimationDuration;
		if (progress > 1.0f) progress = 1.0f;

		_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPosition.y = m_fEndY - (m_fEndY - m_fStartY) * progress;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPosition);

		if (m_fElapsedTime >= m_fAnimationDuration)
		{
			m_bIsAnimatingDown = false;
			m_fElapsedTime = 0.0f;
			vPosition.y = m_fStartY;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPosition);
		}
	}

	//// Update alpha value
	//m_fElapsedTime += fTimeDelta;
	//float fAlphaStep = ((m_fMaxAlpha - m_fMinAlpha) / m_fAlphaAnimationDuration) * fTimeDelta;

	//if (m_bIncreasingAlpha)
	//{
	//	m_fAlpha += fAlphaStep;
	//	if (m_fAlpha >= m_fMaxAlpha)
	//	{
	//		m_fAlpha = m_fMaxAlpha;
	//		m_bIncreasingAlpha = false;
	//	}
	//}
	//else
	//{
	//	m_fAlpha -= fAlphaStep;
	//	if (m_fAlpha <= m_fMinAlpha)
	//	{
	//		m_fAlpha = m_fMinAlpha;
	//		m_bIncreasingAlpha = true;
	//	}
	//}
}
	

void CUI_Npc_Question_Effect::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Npc_Question_Effect::Render(_float fTimeDelta)
{
	Begin_RenderState();

	AnimState(fTimeDelta);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	End_RenderState();

	return S_OK;
}


HRESULT CUI_Npc_Question_Effect::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CUI_Npc_Question_Effect::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CUI_Npc_Question_Effect::Ready_Components()
{
	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
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

HRESULT CUI_Npc_Question_Effect::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_UI_Quest_QuestionMark"), TEXT("AnimTexture_UI_Quest_QuestionMark"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_UI_Quest_QuestionMarkBlur"), TEXT("AnimTexture_UI_Quest_QuestionMarkBlur"));

	return S_OK;
}

void CUI_Npc_Question_Effect::AnimState(_float _fTimeDelta)
{
	m_pAnimCom->Play_Animator(TEXT("AnimTexture_UI_Quest_QuestionMark"), 0.5f, _fTimeDelta, true);
	//m_pAnimCom->Play_Animator(TEXT("AnimTexture_UI_Quest_QuestionMarkBlur"), 3.f, _fTimeDelta, true);
}

CUI_Npc_Question_Effect* CUI_Npc_Question_Effect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Npc_Question_Effect* pInstance = new CUI_Npc_Question_Effect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_HP_FadeInOut"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Npc_Question_Effect::Clone(void* pArg)
{
	CUI_Npc_Question_Effect* pInstance = new CUI_Npc_Question_Effect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_HP_FadeInOut"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Npc_Question_Effect::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pAnimCom);

	__super::Free();
}
