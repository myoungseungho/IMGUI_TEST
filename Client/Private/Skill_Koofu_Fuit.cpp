#include "stdafx.h"

#include "Skill_Koofu_Fuit.h"

CSkill_Koofu_Fuit::CSkill_Koofu_Fuit(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill_Monster { pGraphic_Device }
{
}

CSkill_Koofu_Fuit::CSkill_Koofu_Fuit(const CSkill_Koofu_Fuit& Prototype)
	:CSkill_Monster{ Prototype }
{
}

HRESULT CSkill_Koofu_Fuit::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Koofu_Fuit::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SKILL_FUIT_DESC* pDesc = static_cast<SKILL_FUIT_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	m_pPlayerTransform = pDesc->pPlayerTransform;

	Safe_AddRef(m_pTargetTransform);
	Safe_AddRef(m_pPlayerTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	_float vPositionX = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).x + (pDesc->iBulletCnt * 2);
	_float vPositionY = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).y;
	_float vPositionZ = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).z + (-1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vPositionX, vPositionY, vPositionZ));

	m_pTransformCom->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	return S_OK;
}

void CSkill_Koofu_Fuit::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Koofu_Fuit::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);
	m_pTransformCom->Go_Up(fTimeDelta);
	m_pTransformCom->Gravity(0.1f, 2.f, fTimeDelta);
	Bounce(2.f);
	
}

void CSkill_Koofu_Fuit::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSkill_Koofu_Fuit::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		E_FAIL;

	m_pAnimCom->Play_Animator(TEXT("SKILL_FUIT"), 0.5f, fTimeDelta, true);


	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	if (FAILED(End_RenderState()))
		E_FAIL;

	return S_OK;
}

HRESULT CSkill_Koofu_Fuit::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_FuitBounce"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Koofu_Fuit::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY,TEXT("Prototype_Component_Texture_FuitBounce"), TEXT("SKILL_FUIT"));

	return S_OK;
}

HRESULT CSkill_Koofu_Fuit::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSkill_Koofu_Fuit::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CSkill_Koofu_Fuit::Bounce(_float _LandPosY)
{
	if(m_pTransformCom->Get_State(CTransform::STATE_POSITION).y <= _LandPosY)
		m_pTransformCom->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
}

CSkill_Koofu_Fuit* CSkill_Koofu_Fuit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkill_Koofu_Fuit* pInstance = new CSkill_Koofu_Fuit(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Skill_Koofu_Fuit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Koofu_Fuit::Clone(void* pArg)
{
	CSkill_Koofu_Fuit* pInstance = new CSkill_Koofu_Fuit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Skill_Koofu_Fuit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Koofu_Fuit::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTransformCom); 
	Safe_Release(m_pPlayerTransform);
}
