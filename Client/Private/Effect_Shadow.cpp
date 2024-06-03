#include "stdafx.h"

#include "Effect_Shadow.h"

CEffect_Shadow::CEffect_Shadow(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect_Monster{ pGraphic_Device }
{
}

CEffect_Shadow::CEffect_Shadow(const CEffect_Shadow& Prototype)
	:CEffect_Monster{ Prototype }
{
}

HRESULT CEffect_Shadow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Shadow::Initialize(void* pArg)
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

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), 90.f * D3DX_PI / 180.f);

	return S_OK;
}

void CEffect_Shadow::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Shadow::Update(_float fTimeDelta)
{
	_float3 vPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	vPos.y = 0.01f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);
	Destroy(fTimeDelta);
}

void CEffect_Shadow::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CEffect_Shadow::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Shadow::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Shadow"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(2.f, 2.f, 1.f));


	return S_OK;
}

HRESULT CEffect_Shadow::Ready_Animation()
{
	return S_OK;
}

HRESULT CEffect_Shadow::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CEffect_Shadow::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CEffect_Shadow::Destroy(_float fTimeDelta)
{
}

CEffect_Shadow* CEffect_Shadow::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Shadow* pInstance = new CEffect_Shadow(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Effect_Shadow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

//쌤코드는 GameObject/ 내가 판단하기로는 BlendObject 일단 보류
CGameObject* CEffect_Shadow::Clone(void* pArg)
{
	CEffect_Shadow* pInstance = new CEffect_Shadow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Effect_Shadow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Shadow::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);

	__super::Free();
}
