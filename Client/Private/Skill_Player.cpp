#include "stdafx.h"

#include "Skill_Player.h"
#include "Monster.h"
#include "GameInstance.h"

CSkill_Player::CSkill_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill{ pGraphic_Device }
{
}

CSkill_Player::CSkill_Player(const CSkill_Player& Prototype)
	:CSkill{ Prototype }
{
}

HRESULT CSkill_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Player::Initialize(void* pArg)
{
	int a = 0;

	SKILL_PLAYER_DESC* pDesc = static_cast<SKILL_PLAYER_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	m_iSkillCount = pDesc->m_iCurrentSkillCount;
	m_SkillDir = pDesc->m_SkillDir;

	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//_float3 TargetLook = m_pTargetTransform->Get_State(CTransform::STATE_LOOK);
	_float3 TargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	_float3 ReScaled = m_pTransformCom->Get_Scaled() * (m_iSkillCount*0.8);

	TargetPos += *D3DXVec3Normalize(&m_SkillDir, &m_SkillDir) * (m_iSkillCount);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &TargetPos);
	m_pTransformCom->Set_Scaled(ReScaled);
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), 1);

	// 플레이어의 방향 벡터 가지고 와서 스킬 포지션에 적용하고 업데이트에서 시간 값따라 이동하면 하나씩 증가되는 것처럼 보일 듯


	return S_OK;
}

void CSkill_Player::Priority_Update(_float fTimeDelta)
{
	
}

void CSkill_Player::Update(_float fTimeDelta)
{
	For_Skill_Damage(fTimeDelta);

	if (m_pTimerCom->Time_Limit(fTimeDelta, 0.5f))
	{
		Delete_Object();
	}
}

void CSkill_Player::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSkill_Player::Render(_float fTimeDelta)
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

void CSkill_Player::For_Skill_Damage(_float fTimeDelta)
{
	m_fSkillTime += fTimeDelta;

	if (m_fSkillTime >= 5.f)
	{
		
		m_bSkillAttack = true;
	}
	
}

void CSkill_Player::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CSkill_Player::OnCollisionStay(CCollider* other, _float fTimeDelta)
{

	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CMonster*>(otherObject))
	{
		if (m_bSkillAttack)
		{
			++m_Check;;
			CMonster* pDamagedObj = dynamic_cast<CMonster*>(otherObject);
			pDamagedObj->Damaged();
			m_bSkillAttack = false;
		}

		return;
	}

}

void CSkill_Player::OnCollisionExit(class CCollider* other)
{
	int a = 3;
}
HRESULT CSkill_Player::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSkill_Player::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}


HRESULT CSkill_Player::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Skill"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Calc_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Calc_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
		return E_FAIL;

	/* For.Com_KeyState */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Key"),
		TEXT("Com_KeyState"), reinterpret_cast<CComponent**>(&m_pKeyCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{ 0.f, 0.f };
	//TransformDesc.fSpeedPerSec = 1.0f;
	//TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_pTargetTransform->Get_State(CTransform::STATE_POSITION));

	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 1.f;
	ColliderDesc.MineGameObject = this;

	//콜라이더 사본을 만들때 Cube 정보 추가해줘야 함.
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//콜라이더오브젝트 추가
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_PLAYER_SKILL, this);

	return S_OK;
}

CSkill_Player* CSkill_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkill_Player* pInstance = new CSkill_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Skill_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Player::Clone(void* pArg)
{
	CSkill_Player* pInstance = new CSkill_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Skill_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Player::Free()
{
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pKeyCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
