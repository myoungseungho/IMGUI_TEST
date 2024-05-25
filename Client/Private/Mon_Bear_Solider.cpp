#include "stdafx.h"

#include "Mon_Bear_Solider.h"

#include "GameInstance.h"

CMon_Bear_Solider::CMon_Bear_Solider(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster{ pGraphic_Device }
{
}

CMon_Bear_Solider::CMon_Bear_Solider(const CMon_Bear_Solider& Prototype)
	:CMonster{ Prototype }
{
}

HRESULT CMon_Bear_Solider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMon_Bear_Solider::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);

	m_tMonsterDesc.iHp = pDesc->iHp;
	m_tMonsterDesc.iAttack = pDesc->iAttack;
	m_pPlayerTransform = pDesc->pTargetTransform;

	Safe_AddRef(m_pPlayerTransform);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(15.f, 0.5f, 15.f));

	m_eMon_State = MON_STATE::IDLE;
	m_eAnim_State = ANIM_STATE::IDLE;

	return S_OK;
}

void CMon_Bear_Solider::Priority_Update(_float fTimeDelta)
{
	__super::Move_Dir(fTimeDelta);

	m_vTargetDistance = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_fAttackRange = D3DXVec3Length(&m_vTargetDistance);
}

void CMon_Bear_Solider::Update(_float fTimeDelta)
{
}

void CMon_Bear_Solider::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CMon_Bear_Solider::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	Anim_State(fTimeDelta);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMon_Bear_Solider::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
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

	/* For.Com_Collider */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 0.5f;
	ColliderDesc.MineGameObject = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_MONSTER, this);

	return S_OK;
}

HRESULT CMon_Bear_Solider::Ready_Animation()
{
	/* Bear_Solider_Attack */
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_Down"), TEXT("Bear_Soldier_Attack_Down"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_Left"), TEXT("Bear_Soldier_Attack_Left"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_LeftDown"), TEXT("Bear_Soldier_Attack_LeftDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_LeftUp"), TEXT("Bear_Soldier_Attack_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_Right"), TEXT("Bear_Soldier_Attack_Right"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_RightDown"), TEXT("Bear_Soldier_Attack_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_RightUp"), TEXT("Bear_Soldier_Attack_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_Up"), TEXT("Bear_Soldier_Attack_Up"));

	/* Bear_Solider_Walk */
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_Down"), TEXT("Bear_Soldier_Walk_Down"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_Left"), TEXT("Bear_Soldier_Walk_Left"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_LeftDown"), TEXT("Bear_Soldier_Walk_LeftDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_LeftUp"), TEXT("Bear_Soldier_Walk_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_Right"), TEXT("Bear_Soldier_Walk_Right"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_RightDown"), TEXT("Bear_Soldier_Walk_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_RightUp"), TEXT("Bear_Soldier_Walk_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_Up"), TEXT("Bear_Soldier_Walk_Up"));

	/* Bear_Solider_Hit */
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_Down"), TEXT("Bear_Soldier_Hit_Down"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_Left"), TEXT("Bear_Soldier_Hit_Left"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_LeftDown"), TEXT("Bear_Soldier_Hit_LeftDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_LeftUp"), TEXT("Bear_Soldier_Hit_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_Right"), TEXT("Bear_Soldier_Hit_Right"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_RightDown"), TEXT("Bear_Soldier_Hit_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_RightUp"), TEXT("Bear_Soldier_Hit_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_Up"), TEXT("Bear_Soldier_Hit_Up"));

	/* Bear_Solider_Stun */
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_Down"), TEXT("Bear_Soldier_Stun_Down"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_Left"), TEXT("Bear_Soldier_Stun_Left"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_LeftDown"), TEXT("Bear_Soldier_Stun_LeftDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_LeftUp"), TEXT("Bear_Soldier_Stun_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_Right"), TEXT("Bear_Soldier_Stun_Right"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_RightDown"), TEXT("Bear_Soldier_Stun_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_RightUp"), TEXT("Bear_Soldier_Stun_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_Up"), TEXT("Bear_Soldier_Stun_Up"));

	/* Monster_Bear_Soldier_Idle*/
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Idle"), TEXT("Bear_Soldier_Idle"));

	return S_OK;
}

HRESULT CMon_Bear_Solider::Begin_RenderState()
{
	return S_OK;
}

HRESULT CMon_Bear_Solider::End_RenderState()
{
	return S_OK;
}

void CMon_Bear_Solider::OnCollisionEnter(CCollider* other)
{
}

void CMon_Bear_Solider::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CMon_Bear_Solider::OnCollisionExit(CCollider* other)
{
}

void CMon_Bear_Solider::Anim_State(_float fTimeDelta)
{
}

void CMon_Bear_Solider::Mon_State(_float fTimeDelta)
{
}

CMon_Bear_Solider* CMon_Bear_Solider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMon_Bear_Solider* pInstance = new CMon_Bear_Solider(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Mon_Bear_Solider"));
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject* CMon_Bear_Solider::Clone(void* pArg)
{
	CMon_Bear_Solider* pInstance = new CMon_Bear_Solider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : Mon_Bear_Solider"));
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CMon_Bear_Solider::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pPlayerTransform);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
