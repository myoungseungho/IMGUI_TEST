#include "stdafx.h"

#include "Effect_Stun.h"

CEffect_Stun::CEffect_Stun(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect_Monster{ pGraphic_Device }
{
}

CEffect_Stun::CEffect_Stun(const CEffect_Stun& Prototype)
	:CEffect_Monster{ Prototype }
{
}

HRESULT CEffect_Stun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Stun::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	EFFECT_MONSTER__DESC* pDesc = static_cast<EFFECT_MONSTER__DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	_float3 vPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	vPos.y = 2.f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);


	return S_OK;
}

void CEffect_Stun::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Stun::Update(_float fTimeDelta)
{
}

void CEffect_Stun::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
	Destroy(fTimeDelta);
}

HRESULT CEffect_Stun::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Play_Animator(TEXT("EFFECT_STUN"), 0.75f, fTimeDelta, true)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Stun::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f));

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Stun::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Koofu_Stun"), TEXT("EFFECT_STUN"));
	return S_OK;
}

HRESULT CEffect_Stun::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CEffect_Stun::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CEffect_Stun::Destroy(_float fTimeDelta)
{
	CEffect_Stun* pThis = this;

	if (m_pTimerCom->Time_Limit(fTimeDelta, 4.0f))
		Safe_Release(pThis);
}

CEffect_Stun* CEffect_Stun::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Stun* pInstance = new CEffect_Stun(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Effect_Stun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

//쌤코드는 GameObject/ 내가 판단하기로는 BlendObject 일단 보류
CGameObject* CEffect_Stun::Clone(void* pArg)
{
	CEffect_Stun* pInstance = new CEffect_Stun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Effect_Stun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Stun::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pAnimCom);

	__super::Free();
}
