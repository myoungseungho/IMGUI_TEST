#include "stdafx.h"

#include "Effect_Shield.h"

CEffect_Shield::CEffect_Shield(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect_Monster{ pGraphic_Device }
{
}

CEffect_Shield::CEffect_Shield(const CEffect_Shield& Prototype)
	:CEffect_Monster{ Prototype }
{
}

HRESULT CEffect_Shield::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Shield::Initialize(void* pArg)
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

	return S_OK;
}

void CEffect_Shield::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Shield::Update(_float fTimeDelta)
{
	Destroy(fTimeDelta);
}

void CEffect_Shield::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CEffect_Shield::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Play_Animator(TEXT("EFFECT_MON_DESTROY"), 0.5f, fTimeDelta, true)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Shield::Ready_Components()
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


	return S_OK;
}

HRESULT CEffect_Shield::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_Texture_Texture_Shield"), TEXT("EFFECT_SHIELD"));
	return S_OK;
}

HRESULT CEffect_Shield::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CEffect_Shield::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CEffect_Shield::Destroy(_float fTimeDelta)
{
}

CEffect_Shield* CEffect_Shield::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Shield* pInstance = new CEffect_Shield(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Effect_Shield"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

//쌤코드는 GameObject/ 내가 판단하기로는 BlendObject 일단 보류
CGameObject* CEffect_Shield::Clone(void* pArg)
{
	CEffect_Shield* pInstance = new CEffect_Shield(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Effect_Shield"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Shield::Free()
{
	__super::Free();
}
