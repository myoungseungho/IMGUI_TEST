#include "stdafx.h"

#include "Skill_Koofu_Rolling.h"

CSkill_Koofu_Rolling::CSkill_Koofu_Rolling(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill_Monster { pGraphic_Device }
{
}

CSkill_Koofu_Rolling::CSkill_Koofu_Rolling(const CSkill_Koofu_Rolling& pPrototype)
	:CSkill_Monster{ pPrototype }
{
}

HRESULT CSkill_Koofu_Rolling::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Koofu_Rolling::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SKILL_MONSTER__DESC* pDesc = static_cast<SKILL_MONSTER__DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	_float vPositionX = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).x + (pDesc->iBulletCnt * 5); 
	_float vPositionY = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).y;
	_float vPositionZ = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).z + (- 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3 (vPositionX , vPositionY , vPositionZ));

	return S_OK;
}

void CSkill_Koofu_Rolling::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Koofu_Rolling::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);
}

void CSkill_Koofu_Rolling::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	Destroy(fTimeDelta);
}

HRESULT CSkill_Koofu_Rolling::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		E_FAIL;

	m_pAnimCom->Play_Animator(TEXT("SKILL_ROLLING"), 0.5f, fTimeDelta, true);

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
		

	if (FAILED(End_RenderState()))
		E_FAIL;

	return S_OK;
}

HRESULT CSkill_Koofu_Rolling::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_RollingIce"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 10.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(5.f, 5.f, 1.f));

	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 0.5f;
	ColliderDesc.MineGameObject = this;

	//�ݶ��̴� �纻�� ���鶧 Cube ���� �߰������ ��.
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//�ݶ��̴�������Ʈ �߰�
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_MONSTER, this);

	return S_OK;
}

HRESULT CSkill_Koofu_Rolling::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_RollingIce"), TEXT("SKILL_ROLLING"));

	return S_OK;
}

void CSkill_Koofu_Rolling::Destroy(_float fTimeDelta)
{
	CSkill_Koofu_Rolling* pRolling = this;

	if (m_pTimerCom->Time_Limit(fTimeDelta  ,5.f))
	{
		Safe_Release(pRolling);
	}
}

HRESULT CSkill_Koofu_Rolling::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSkill_Koofu_Rolling::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CSkill_Koofu_Rolling* CSkill_Koofu_Rolling::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkill_Koofu_Rolling* pInstance = new CSkill_Koofu_Rolling(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Skill_Koofu_Rolling"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Koofu_Rolling::Clone(void* pArg)
{
	CSkill_Koofu_Rolling* pInstance = new CSkill_Koofu_Rolling(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Skill_Koofu_Rolling"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Koofu_Rolling::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
