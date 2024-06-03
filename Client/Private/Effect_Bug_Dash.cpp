#include "stdafx.h"

#include "Effect_Bug_Dash.h"

CEffect_Bug_Dash::CEffect_Bug_Dash(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect_Monster{ pGraphic_Device }
{
}

CEffect_Bug_Dash::CEffect_Bug_Dash(const CEffect_Bug_Dash& Prototype)
	:CEffect_Monster{ Prototype }
{
}

HRESULT CEffect_Bug_Dash::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Bug_Dash::Initialize(void* pArg)
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

	_float3 m_vTargetTransform = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	m_vTargetTransform.x = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).x;
	m_vTargetTransform.y = 1.f;
	m_vTargetTransform.z = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).z;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_vTargetTransform);

	return S_OK;
}

void CEffect_Bug_Dash::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Bug_Dash::Update(_float fTimeDelta)
{
	m_pTransformCom->LookAt(m_pTargetTransform->Get_State(CTransform::STATE_POSITION));
	_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	m_pTransformCom->Radian_Turn(vRight, 270.f * D3DX_PI / 180.f);
	Destroy(fTimeDelta);
}

void CEffect_Bug_Dash::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CEffect_Bug_Dash::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Play_Animator(TEXT("EFFECT_BUG_DASH"), 0.5f, fTimeDelta, true)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Bug_Dash::Ready_Components()
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

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(5.f, 5.f, 1.f));


	return S_OK;
}

HRESULT CEffect_Bug_Dash::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Effect_Bug_Dash"), TEXT("EFFECT_BUG_DASH"));
	return S_OK;
}

HRESULT CEffect_Bug_Dash::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CEffect_Bug_Dash::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CEffect_Bug_Dash::Destroy(_float fTimeDelta)
{
	CEffect_Bug_Dash* pThis = this;

	if (m_pTimerCom->Time_Limit(fTimeDelta, 0.5f))
		Safe_Release(pThis);
}

CEffect_Bug_Dash* CEffect_Bug_Dash::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Bug_Dash* pInstance = new CEffect_Bug_Dash(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Effect_Bug_Dash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Bug_Dash::Clone(void* pArg)
{
	CEffect_Bug_Dash* pInstance = new CEffect_Bug_Dash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Effect_Bug_Dash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Bug_Dash::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pAnimCom);

	__super::Free();
}
