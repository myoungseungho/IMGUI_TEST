#include "stdafx.h"
#include "..\Public\Effect_Orb.h"

#include "GameInstance.h"

CEffect_Orb::CEffect_Orb(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
}

CEffect_Orb::CEffect_Orb(const CEffect_Orb& Prototype)
	: CEffect{ Prototype }
{
}

HRESULT CEffect_Orb::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CEffect_Orb::Initialize(void* pArg)
{
	EFFECT_ORB_DESC* pDesc = static_cast<EFFECT_ORB_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;

	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x, vTargetPos.y, vTargetPos.z + 0.01f));
	m_pTransformCom->Set_Scaled(_float3(2.5f, 2.5f, 1.f));

	return S_OK;
}

void CEffect_Orb::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Orb::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_pTimerCom->Time_Limit(fTimeDelta, 5.f))
	{
		m_Died = true;
	}
}

void CEffect_Orb::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CEffect_Orb::Render(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	__super::Begin_Blend_RenderState();

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_Blend_RenderState();

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CEffect_Orb::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Effect_Orb"),
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

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;

	/* For.Com_Calc_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Calc_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
		return E_FAIL;

	return S_OK;
}

CEffect_Orb* CEffect_Orb::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Orb* pInstance = new CEffect_Orb(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_Orb"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CEffect_Orb::Clone(void* pArg)
{
	CEffect_Orb* pInstance = new CEffect_Orb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect_Orb"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Orb::Free()
{
	Safe_Release(m_pAnimCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}

