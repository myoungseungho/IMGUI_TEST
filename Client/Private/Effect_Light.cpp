#include "stdafx.h"

#include "Effect_Light.h"

CEffect_Light::CEffect_Light(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect_Monster{ pGraphic_Device }
{
}

CEffect_Light::CEffect_Light(const CEffect_Light& Prototype)
	:CEffect_Monster{ Prototype }
{
}

HRESULT CEffect_Light::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Light::Initialize(void* pArg)
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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(10.f, 10.f, 10.f));


	D3DXCOLOR Color = D3DCOLOR_XRGB(255, 255, 255);

	pLight.Type = D3DLIGHT_DIRECTIONAL;
	pLight.Diffuse = Color;
	pLight.Specular = Color * 0.3f;
	pLight.Ambient = Color * 0.6f;
	pLight.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	return S_OK;
}

void CEffect_Light::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Light::Update(_float fTimeDelta)
{
}

void CEffect_Light::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CEffect_Light::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Light::Ready_Components()
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

HRESULT CEffect_Light::Ready_Animation()
{
	return S_OK;
}

HRESULT CEffect_Light::Begin_RenderState()
{
	m_pGraphic_Device->SetLight(0, &pLight);
	m_pGraphic_Device->LightEnable(0, true);


	m_pGraphic_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pGraphic_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	return S_OK;
}

HRESULT CEffect_Light::End_RenderState()
{


	return S_OK;
}


void CEffect_Light::Destroy(_float fTimeDelta)
{
}

CEffect_Light* CEffect_Light::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Light* pInstance = new CEffect_Light(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Effect_Light"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Light::Clone(void* pArg)
{
	CEffect_Light* pInstance = new CEffect_Light(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Effect_Light"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Light::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);

	__super::Free();
}
