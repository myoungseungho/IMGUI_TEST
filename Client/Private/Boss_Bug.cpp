#include "stdafx.h"

#include "Boss_Bug.h"
#include "GameInstance.h"
#include "Mon_Turtle.h"


CBoss_Bug::CBoss_Bug(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster{ pGraphic_Device }
{
}

CBoss_Bug::CBoss_Bug(const CBoss_Bug& Prototype)
	:CMonster{ Prototype }
{
}

HRESULT CBoss_Bug::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Bug::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BOSS_BUG_DESC* pDesc = static_cast<BOSS_BUG_DESC*>(pArg);
	m_pTargetTransform = pDesc->pTargetTransform;
	m_tMonsterDesc.iHp = pDesc->iHp;
	m_tMonsterDesc.iAttack = pDesc->iAttack;

	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(5.f, 5.f, 5.f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(30.0f, 1.5f, 20.f));
	m_pTransformCom->LookAt(m_pTargetTransform->Get_State(CTransform::STATE_POSITION));

	m_eMon_State = MON_STATE::IDLE;

	return S_OK;
}

void CBoss_Bug::Priority_Update(_float fTimeDelta)
{
	m_fAngle++;

	if (m_fAngle > 360.f)
		m_fAngle = 0.f;
}

void CBoss_Bug::Update(_float fTimeDelta)
{
	if (m_pKeyCom->Key_Down('1'))
		m_tMonsterDesc.iHp--;

	Mon_State(fTimeDelta);

	CBoss_Bug* boss = this;
	if (m_pKeyCom->Key_Down('5'))
	{
		m_isTmp = true;
	}
	else if (m_pKeyCom->Key_Down('6'))
		m_isTmp = false;
	else if (m_pKeyCom->Key_Down('7'))
		Safe_Release(boss);

}

void CBoss_Bug::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBoss_Bug::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	Mon_AnimState(fTimeDelta);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Bug::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 2.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x / 2.f;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y / 2.f;
	ColliderDesc.depth = 0.5f;
	ColliderDesc.MineGameObject = this;

	//콜라이더 사본을 만들때 Cube 정보 추가해줘야 함.
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//콜라이더오브젝트 추가
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_MONSTER, this);

	return S_OK;
}

HRESULT CBoss_Bug::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase1_Idle"), TEXT("BOSS_BUG_PHASE1_IDLE"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase1_Ready"), TEXT("BOSS_BUG_PHASE1_READY"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase1_Attack"), TEXT("BOSS_BUG_PHASE1_ATTACK"));

	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Ready"), TEXT("BOSS_BUG_PHASE2_READY"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Regen"), TEXT("BOSS_BUG_PHASE2_REGEN"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Attack"), TEXT("BOSS_BUG_PHASE2_ATTACK"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Death"), TEXT("BOSS_BUG_PHASE2_DEATH"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Down"), TEXT("BOSS_BUG_PHASE2_DOWN"));

	return S_OK;
}

HRESULT CBoss_Bug::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CBoss_Bug::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CBoss_Bug::OnCollisionEnter(CCollider* other)
{
	int a = 3;
}

void CBoss_Bug::OnCollisionStay(CCollider* other)
{
	int a = 3;
}

void CBoss_Bug::OnCollisionExit(CCollider* other)
{
	int a = 3;
}

void CBoss_Bug::Warf(_int iPosX, _int iPosZ, _float fDistance, _float fAngle)
{
	_float WarfPosX = iPosX + fDistance * cos(fAngle * (D3DX_PI / 180.f));
	_float WarfPosZ = iPosZ - fDistance * sin(fAngle * (D3DX_PI / 180.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(WarfPosX, 1.5f, WarfPosZ));

	_float3 PlayerPos;

	PlayerPos.x = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).x;
	PlayerPos.y = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).y + 1.5f;
	PlayerPos.z = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).z;

	m_pTransformCom->LookAt(PlayerPos);

}

void CBoss_Bug::Skill_Dash(_float fTimeDelta)
{
	auto iter = dynamic_cast<CMon_Turtle*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster_Turtle")));

	if (iter)
	{
		if (m_pTimerCom->Time_Limit(fTimeDelta, 3.f))
		{
			Warf(30, 20, 50.f, m_fAngle);
		}
		else
		{
			m_pTransformCom->Set_Speed(10.f);
			m_pTransformCom->Go_Straight(fTimeDelta * 5.f);
		}
	}
	else
	{
		m_pTransformCom->Set_Speed(2.f);
	}
}

void CBoss_Bug::Fly(_float fTimeDelta)
{

	if (m_pTimerCom->Time_Limit(fTimeDelta, 5.f))
	{
		m_isLand = false;
		m_eMon_State = MON_STATE::LAND;
	}
	else
		m_pTransformCom->Go_Up(fTimeDelta);
}

void CBoss_Bug::Land(_int iPosX, _int iPosZ, _float fTimeDelta)
{
	if (!m_isLand)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (&_float3(iPosX, 5.f, iPosZ)));
		m_isLand = true;
	}

	if (m_pTimerCom->Time_Limit(fTimeDelta, 2.f))
	{
		m_eMon_State = MON_STATE::DASH;
	}
	else
		m_pTransformCom->Go_Down(fTimeDelta);

}

HRESULT CBoss_Bug::Turtle_Create()
{
	CMon_Turtle::MON_TURTLE_DESC	 Desc{};

	Desc.iHp = 10;
	Desc.iAttack = 1;
	Desc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));

	const wstring A[3] = { L"Prototype_Component_Texture_Monster_Red_Turtle" , L"Prototype_Component_Texture_Monster_Green_Turtle" , L"Prototype_Component_Texture_Monster_Blue_Turtle" };


	for (int i = 0; i < 3; ++i)
	{
		Desc.ColorTexTag = A[i];

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Turtle"), TEXT("Layer_Monster_Turtle"), &Desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CBoss_Bug::Bullet_Create()
{
	CSkill_Monster::SKILL_MONSTER__DESC SkillDesc{};
	SkillDesc.pTargetTransform = m_pTransformCom;
	SkillDesc.iTotalBullet = 12;

	for (int i = 1; i <= SkillDesc.iTotalBullet; ++i)
	{
		SkillDesc.iBulletCnt = i;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Bug_Bullet"), TEXT("Layer_Skill_Bug_Bullet"), &SkillDesc)))
			return E_FAIL;
	}

	return S_OK;
}

void CBoss_Bug::State_Idle(_float  _fTimeDelta)
{
	if (m_pTimerCom->Time_Limit(_fTimeDelta, 3.f))
		m_eMon_State = MON_STATE::BULLET;
}

void CBoss_Bug::State_Bullet(_float  _fTimeDelta)
{
	if (m_pTimerCom->Time_Limit(_fTimeDelta, 1.f))
	{
		Bullet_Create();
		m_iBulletCnt++;
	}

	if (m_iBulletCnt >= 3)
	{
		m_eMon_State = MON_STATE::IDLE;
		m_iBulletCnt = 0;
	}

	if (m_tMonsterDesc.iHp <= 40)
	{
		m_iPhaseCnt = 2;
		m_ePrev_State = MON_STATE::BULLET;
		m_eMon_State = MON_STATE::REGEN;
	}
}

void CBoss_Bug::State_Regen(_float _fTimeDelta)
{
	if (m_pTimerCom->Time_Limit(_fTimeDelta, 2.f) && m_ePrev_State == MON_STATE::BULLET)
	{
		m_eMon_State = MON_STATE::READY;
	}

	if (m_ePrev_State == MON_STATE::STAN && m_pTimerCom->Time_Limit(_fTimeDelta, 5.f))
	{
		m_eMon_State = MON_STATE::FLY;
	}

}

void CBoss_Bug::State_Stan(_float fTimeDelta)
{
	m_pTransformCom->Gravity(0.1f, 3.f, fTimeDelta);

	if (m_pTimerCom->Time_Limit(fTimeDelta, 5.f))
	{
		m_ePrev_State = MON_STATE::STAN;
		m_eMon_State = MON_STATE::REGEN;
	}
}

void CBoss_Bug::State_Ready(_float _fTimeDelta)
{
	if (m_iPhaseCnt == 1 && m_pTimerCom->Time_Limit(_fTimeDelta, 3.f))
		m_eMon_State = MON_STATE::BULLET;
	else if (m_iPhaseCnt == 2 && m_pTimerCom->Time_Limit(_fTimeDelta, 1.f))
		m_eMon_State = MON_STATE::DASH;

}

void CBoss_Bug::State_Dash(_float  _fTimeDelta)
{
	auto iter = dynamic_cast<CMon_Turtle*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster_Turtle")));
	Skill_Dash(_fTimeDelta);

	if (!iter)
	{
		if (m_isTurtle)
		{
			m_ePrev_State == MON_STATE::DASH;
			m_eMon_State = MON_STATE::STAN;
		}
		if (m_pTimerCom->Time_Limit(_fTimeDelta, 3.f))
		{
			Turtle_Create();
			m_isTurtle = true;
		}
	}
}


void CBoss_Bug::State_Fly(_float  _fTimeDelta)
{
	m_isTurtle = false;
	Fly(_fTimeDelta);
}

void CBoss_Bug::State_Land(_float  _fTimeDelta)
{
	Land(0.f, 0.f, _fTimeDelta);
}


void CBoss_Bug::Mon_AnimState(_float _fTimeDelta)
{
	switch (m_eMon_State)
	{
	case MON_STATE::IDLE:
		if (m_iPhaseCnt == 1)
			m_pAnimCom->Play_Animator(TEXT("BOSS_BUG_PHASE1_IDLE"), 1.f, _fTimeDelta, true);
		break;

	case MON_STATE::BULLET:
		if (m_iPhaseCnt == 1)
			m_pAnimCom->Play_Animator(TEXT("BOSS_BUG_PHASE1_ATTACK"), 1.1f, _fTimeDelta, true);
		break;

	case MON_STATE::READY:
		if (m_iPhaseCnt == 1)
			m_pAnimCom->Play_Animator(TEXT("BOSS_BUG_PHASE1_READY"), 1.f, _fTimeDelta, true);
		else if (m_iPhaseCnt == 2)
			m_pAnimCom->Play_Animator(TEXT("BOSS_BUG_PHASE2_READY"), 1.f, _fTimeDelta, false);
		break;

	case MON_STATE::DASH:
		if (m_iPhaseCnt == 2)
			m_pAnimCom->Play_Animator(TEXT("BOSS_BUG_PHASE2_ATTACK"), 1.f, _fTimeDelta, false);
		break;

	case MON_STATE::REGEN:
		if (m_iPhaseCnt == 2)
			m_pAnimCom->Play_Animator(TEXT("BOSS_BUG_PHASE2_REGEN"), 1.f, _fTimeDelta, false);
		break;

	case MON_STATE::STAN:
		if (m_iPhaseCnt == 2)
			m_pAnimCom->Play_Animator(TEXT("BOSS_BUG_PHASE2_DOWN"), 1.f, _fTimeDelta, false);
		break;

	}

}

void CBoss_Bug::Mon_State(_float fTimeDelta)
{
	if (m_tMonsterDesc.iHp <= 0)
		m_eMon_State = MON_STATE::DEATH;

	switch (m_eMon_State)
	{
	case MON_STATE::IDLE:
		State_Idle(fTimeDelta);
		break;

	case MON_STATE::DASH:
		State_Dash(fTimeDelta);
		break;

	case MON_STATE::READY:
		State_Ready(fTimeDelta);
		break;

	case MON_STATE::BULLET:
		State_Bullet(fTimeDelta);
		break;

	case MON_STATE::FLY:
		State_Fly(fTimeDelta);
		break;

	case MON_STATE::LAND:
		State_Land(fTimeDelta);
		break;

	case MON_STATE::REGEN:
		State_Regen(fTimeDelta);
		break;

	case MON_STATE::STAN:
		State_Stan(fTimeDelta);
		break;
	}
}

CBoss_Bug* CBoss_Bug::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoss_Bug* pInstance = new CBoss_Bug(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Boss_Bug"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Bug::Clone(void* pArg)
{
	CBoss_Bug* pInstance = new CBoss_Bug(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Boss_Bug"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Bug::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}
