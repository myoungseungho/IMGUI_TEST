#include "stdafx.h"

#include "Boss_Koofu.h"
#include "GameInstance.h"

#include "Skill_Monster.h"
#include "Skill_Koofu_Fuit.h"
#include "Skill_Koofu_Bubble.h"

#include "Mon_Copy_Koofu.h"

#include "Player.h"


CBoss_Koofu::CBoss_Koofu(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster{ pGraphic_Device }
{
}

CBoss_Koofu::CBoss_Koofu(const CBoss_Koofu& Prototype)
	:CMonster{ Prototype }
{
}

HRESULT CBoss_Koofu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Koofu::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BOSS_KOOFU_DESC* pDesc = static_cast<BOSS_KOOFU_DESC*>(pArg);

	m_pPlayerTransform = pDesc->m_pTargetTransform;
	m_tMonsterDesc.iHp = pDesc->iHp;

	Safe_AddRef(m_pPlayerTransform);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

 	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_eMon_State = MON_STATE::IDLE;
	m_eAnim_State = ANIM_STATE::IDLE;
	m_eMon_Dir = MON_DIR::DIR_D;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(49.f, 0.75f, 37.f));
	
	return S_OK;
}


void CBoss_Koofu::Priority_Update(_float fTimeDelta)
{
	
}

void CBoss_Koofu::Update(_float fTimeDelta)
{
	Move_Dir();
	Key_Input(fTimeDelta);
	MonState(fTimeDelta);
}

void CBoss_Koofu::Late_Update(_float fTimeDelta)
{ 
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	Destory();
}

HRESULT CBoss_Koofu::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	AnimState(fTimeDelta);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CBoss_Koofu::MonState(_float fTimeDelta)
{
	switch (m_eMon_State)
	{
	case MON_STATE::IDLE:
		State_Idle(fTimeDelta);
		break;

	case MON_STATE::MOVE:
		State_Move(fTimeDelta);
		break;

	case MON_STATE::READY:
		State_Ready(fTimeDelta);
		break;

	case MON_STATE::BULLET:
		State_Bullet(fTimeDelta);
		break;

	case MON_STATE::BULLET_B:
		State_Bullet_B(fTimeDelta);
		break;

	case MON_STATE::BULLET_C:
		State_Bullet_C(fTimeDelta);
		break;

	case MON_STATE::STUN:
		State_Stan(fTimeDelta);
		break;

	case MON_STATE::CAST:
		State_Cast(fTimeDelta);
		break;
	}
}

void CBoss_Koofu::AnimState(_float fTimeDelta)
{
	switch (m_eAnim_State)
	{
	case ANIM_STATE::IDLE:
		switch (m_eMon_Dir)
		{
		case MON_DIR::DIR_D:
 			m_pAnimCom->Play_Animator(TEXT("KOOFU_IDLE_DOWN"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_L:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_IDLE_LEFT"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_LD:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_IDLE_LEFTDOWN"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_LU:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_IDLE_LEFTUP"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_R:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_IDLE_RIGHT"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_RD:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_IDLE_RIGHTDOWN"), 1.f, fTimeDelta, true);
			break;
			
		case MON_DIR::DIR_RU:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_IDLE_RIGHTUP"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_U:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_IDLE_UP"), 1.f, fTimeDelta, true);
			break;
		}
		break;

	case ANIM_STATE::THROW :
		switch (m_eMon_Dir)
		{
		case MON_DIR::DIR_D:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_THROW_DOWN"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_L:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_THROW_LEFT"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_LD:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_THROW_LEFTDOWN"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_LU:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_THROW_LEFTUP"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_R:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_THROW_RIGHT"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_RD:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_THROW_RIGHTDOWN"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_RU:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_THROW_RIGHTUP"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_U:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_THROW_UP"), 1.f, fTimeDelta, true);
			break;
		}
		break;

	case ANIM_STATE::WALK:
		switch (m_eMon_Dir)
		{
		case MON_DIR::DIR_D:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_WALK_DOWN"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_L:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_WALK_LEFT"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_LD:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_WALK_LEFTDOWN"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_LU:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_WALK_LEFTUP"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_R:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_WALK_RIGHT"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_RD:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_WALK_RIGHTDOWN"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_RU:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_WALK_RIGHTUP"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_U:
			m_pAnimCom->Play_Animator(TEXT("KOOFU_WALK_UP"), 1.f, fTimeDelta, true);
			break;
		}
		break;

	case ANIM_STATE::CAST:
		m_pAnimCom->Play_Animator(TEXT("KOOFU_CAST"), 1.f, fTimeDelta, false);
		break;

	case ANIM_STATE::STUN:
		m_pAnimCom->Play_Animator(TEXT("KOOFU_STUN"), 1.5f, fTimeDelta, false);
		break;

	case ANIM_STATE::READY:
		m_pAnimCom->Play_Animator(TEXT("KOOFU_READY"), 1.f, fTimeDelta, false);
		break;

	case ANIM_STATE::DEADTH:
		m_pAnimCom->Play_Animator(TEXT("KOOFU_DEATH"), 1.f, fTimeDelta, false);
		break;
	}

}

void CBoss_Koofu::State_Idle(_float fTimeDelta)
{
	if (m_pTimerCom->Time_Limit(fTimeDelta, 2.f))
		m_eMon_State = MON_STATE::CAST;

}

void CBoss_Koofu::State_Move(_float fTimeDelta)
{
	Move(fTimeDelta);

	if (m_tMonsterDesc.iHp <= 0)
	{
		m_eMon_State = MON_STATE::DEATH;
		m_eAnim_State = ANIM_STATE::DEADTH; 
	}
}

void CBoss_Koofu::State_Ready(_float fTimeDelta)
{
	if (m_pTimerCom->Time_Limit(fTimeDelta, 1.f))
	{
		m_ePrev_State = MON_STATE::READY;
		m_eAnim_State = ANIM_STATE::CAST;
		m_eMon_State = MON_STATE::CAST;
	}

}

void CBoss_Koofu::State_Bullet(_float fTimeDelta)
{
	if (!m_isClone_Create)
	{
		m_isClone_Create = true;
		CloneCreate();

		Warf(49.f, 37.f, 7.f);
	}

	if (m_pTimerCom->Time_Limit(fTimeDelta, 5.f, 1.f))
	{
		m_ePrev_State = MON_STATE::BULLET;
		m_eAnim_State = ANIM_STATE::THROW;

		if (m_pTimerCom->Time_Limit(fTimeDelta, 1.f))
			FuitCreate();
	}
	else
	{
		m_ePrev_State = MON_STATE::BULLET;
		m_eAnim_State = ANIM_STATE::IDLE;
	}

	if (m_bHitCheck)
	{
		m_eAnim_State = ANIM_STATE::STUN;
		m_eMon_State = MON_STATE::STUN; 
		m_isClone_Create = false;

	}

	if (m_tMonsterDesc.iHp <= 30)
	{
		m_eMon_State = MON_STATE::BULLET_C;
	}

}

void CBoss_Koofu::State_Bullet_B(_float fTimeDelta)
{
	m_bHitCheck = false;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(50, 0.75f, 45.f));

	if (m_pTimerCom->Time_Limit(fTimeDelta, 3.f) && !m_isBullet)
	{
		RollingCreate();
		m_isBullet = true;
	}

	if (m_isBullet)
	{
		if (m_pTimerCom->Time_Limit(fTimeDelta, 2.f))
		{
			m_ePrev_State = MON_STATE::BULLET_B;
			m_eMon_State = MON_STATE::IDLE;
			m_isBullet = false;
		}
	}


	if (m_tMonsterDesc.iHp <= 60)
	{
		m_eMon_State = MON_STATE::BULLET_C;
	}
}

void CBoss_Koofu::State_Bullet_C(_float fTimeDelta)
{
	if (!m_bWarf)
	{
		Warf(39, 28, 60, 47);
		m_bWarf = true;
	}

	m_eAnim_State = ANIM_STATE::CAST;

	if (!m_isBubbleSpanw && m_pTimerCom->Time_Limit(fTimeDelta,2.f))
	{
		CircleCreate();
		m_isBubbleSpanw = true;
	}

	if (m_bHitCheck)
	{
		m_ePrev_State = MON_STATE::BULLET_C;
		m_eMon_State = MON_STATE::STUN;

		m_bHitCheck = false;
		m_isBubbleSpanw = false;

		for (int i = 0; i < 3; ++i)
		{
			CSkill_Koofu_Bubble* pClone = dynamic_cast<CSkill_Koofu_Bubble*>(m_pGameInstance->Get_GameObject(LEVEL_KOOFU, TEXT("Layer_Bubble"), i));

			if (!pClone)
				break;

			Safe_Release(pClone);
		}
	}

	if (m_tMonsterDesc.iHp <= 20)
	{
		m_eMon_State = MON_STATE::MOVE;

		for (int i = 0; i < 3; ++i)
		{
			CSkill_Koofu_Bubble* pClone = dynamic_cast<CSkill_Koofu_Bubble*>(m_pGameInstance->Get_GameObject(LEVEL_KOOFU, TEXT("Layer_Bubble"), i));

			if (!pClone)
				break;

			Safe_Release(pClone);
		}
	}
	
}

void CBoss_Koofu::State_Stan(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::STUN;

	if (m_ePrev_State != MON_STATE::BULLET_C  &&  m_pTimerCom->Time_Limit(fTimeDelta, 2.5f))
	{
		m_eAnim_State = ANIM_STATE::READY;
		m_eMon_State = MON_STATE::READY;
	}

	if (m_ePrev_State == MON_STATE::BULLET_C && m_pTimerCom->Time_Limit(fTimeDelta, 2.5f))
	{
		
		m_eMon_State = MON_STATE::BULLET_C;
		m_bHitCheck = false;
		m_bWarf = false;
	}

}

void CBoss_Koofu::State_Cast(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::CAST;

	if ( (m_ePrev_State == MON_STATE::IDLE || m_ePrev_State == MON_STATE::BULLET_B)&& m_pTimerCom->Time_Limit(fTimeDelta, 2.f))
	{
		m_ePrev_State = MON_STATE::CAST;
		m_eMon_State = MON_STATE::BULLET;
		m_bHitCheck = false;

	}
	else if (m_ePrev_State == MON_STATE::READY)
	{
		m_ePrev_State = MON_STATE::CAST;
		m_eMon_State = MON_STATE::BULLET_B;
	}

}

void CBoss_Koofu::Move_Dir()
{
	_float fAngle = m_pTransformCom->Target_Dir_Degree(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	if (fAngle >= 337.5f || fAngle <= 22.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_U;
	}
	else if (fAngle <= 67.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_RU;
	}
	else if (fAngle <= 112.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_R;
	}
	else if (fAngle <= 157.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_RD;
	}
	else if (fAngle <= 202.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_D;
	}
	else if (fAngle <= 247.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_LD;
	}
	else if (fAngle <= 292.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_L;
	}
	else if (fAngle < 337.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_LU;
	}
	else
	{
		MSG_BOX(TEXT("Dir_Error"));
	}
}

void CBoss_Koofu::Move(_float fDeltaTime)
{
	m_eAnim_State = ANIM_STATE::WALK;

	_float3 vChase = {};
	vChase.x = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION).x;
	vChase.y = 0.75f;
	vChase.z = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION).z;

	m_pTransformCom->Chase(vChase, fDeltaTime , 0.5f);
}

void CBoss_Koofu::Key_Input(_float fTimeDelta)
{
	if (m_pKeyCom->Key_Down('2'))
		m_tMonsterDesc.iHp--;
}

void CBoss_Koofu::BillBoarding()
{
	_float4x4		ViewMatrix{};

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, (_float3*)&ViewMatrix.m[2][0]);
}

HRESULT CBoss_Koofu::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;
	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;
	
	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 3.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(50.f, 0.75f, 36.567f));
	m_pTransformCom->Set_Scaled(_float3(1.5f, 1.5f, 1.f));


	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = m_pTransformCom->Get_Scaled().z;
	ColliderDesc.MineGameObject = this;


	//콜라이더 사본을 만들때 Cube 정보 추가해줘야 함.
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//콜라이더오브젝트 추가
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_MONSTER, this);
	

	return S_OK;
}

HRESULT CBoss_Koofu::Ready_Animation()
{
	//Idle
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_Down"), TEXT("KOOFU_IDLE_DOWN"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_Left"), TEXT("KOOFU_IDLE_LEFT"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_LeftDown"), TEXT("KOOFU_IDLE_LEFTDOWN"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_LeftUp"), TEXT("KOOFU_IDLE_LEFTUP"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_Right"), TEXT("KOOFU_IDLE_RIGHT"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_RightDown"), TEXT("KOOFU_IDLE_RIGHTDOWN"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_RightUp"), TEXT("KOOFU_IDLE_RIGHTUP"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_Up"), TEXT("KOOFU_IDLE_UP"));
	
	//Death
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Death"), TEXT("KOOFU_DEATH"));

	//Stun
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Stun"), TEXT("KOOFU_STUN"));

	//Ready
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Ready"), TEXT("KOOFU_READY"));

	//Cast
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Cast"), TEXT("KOOFU_CAST"));

	//Throw
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_Down"), TEXT("KOOFU_THROW_DOWN"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_Left"), TEXT("KOOFU_THROW_LEFT"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_LeftDown"), TEXT("KOOFU_THROW_LEFTDOWN"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_LeftUp"), TEXT("KOOFU_THROW_LEFTUP"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_Right"), TEXT("KOOFU_THROW_RIGHT"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_RightDown"), TEXT("KOOFU_THROW_RIGHTDOWN"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_RightUp"), TEXT("KOOFU_THROW_RIGHTUP"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_Up"), TEXT("KOOFU_THROW_UP"));
	
	//Walk
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_Down"), TEXT("KOOFU_WALK_DOWN"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_Left"), TEXT("KOOFU_WALK_LEFT"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_LeftDown"), TEXT("KOOFU_WALK_LEFTDOWN"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_LeftUp"), TEXT("KOOFU_WALK_LEFTUP"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_Right"), TEXT("KOOFU_WALK_RIGHT"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_RightDown"), TEXT("KOOFU_WALK_RIGHTDOWN"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_RightUp"), TEXT("KOOFU_WALK_RIGHTUP"));
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_Up"), TEXT("KOOFU_WALK_UP"));

	return S_OK;
}

HRESULT CBoss_Koofu::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	
	return S_OK;
}

HRESULT CBoss_Koofu::End_RenderState()
{

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CBoss_Koofu::OnCollisionEnter(CCollider* other)
{

}

void CBoss_Koofu::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CBoss_Koofu::OnCollisionExit(CCollider* other)
{
	CGameObject* otherObject = other->m_MineGameObject;

	if (otherObject->m_Died)
		return;
}

void CBoss_Koofu::ScaleUp(_float fTimeDelta)
{
	if (!m_pTimerCom->Time_Limit(fTimeDelta,1.f ,3.5f))
		m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f) * (fTimeDelta + 1));
}

void CBoss_Koofu::Warf(_int fMinPosX, _int fMinPosZ , _int fMaxPosX , _int fMaxPosZ)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(rand() % (fMaxPosX - fMinPosX) + fMinPosX, 0.75f, rand() % (fMaxPosZ - fMinPosZ )+ fMinPosZ));
}

void CBoss_Koofu::Warf(_int iPosX, _int iPosZ, _float fDistance)
{
	_float WarfPosX = iPosX + fDistance * cos(rand() % 360 * (D3DX_PI / 180.f));
	_float WarfPosZ = iPosZ - fDistance * sin(rand() % 360 * (D3DX_PI / 180.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(WarfPosX, 0.75f, WarfPosZ));

}

void CBoss_Koofu::Destory()
{
	CBoss_Koofu* pthis = this;
}

HRESULT CBoss_Koofu::RollingCreate()
{
	CSkill_Monster::SKILL_MONSTER__DESC Desc = {};

	Desc.iBulletCnt = 0; 
	Desc.pTargetTransform = m_pTransformCom;

	for (int i = 1; i <= 5; ++i)
	{
		Desc.iBulletCnt = i;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_Skill_Koofu_Rolling"), TEXT("Layer_Rolling") ,&Desc)))
			return E_FAIL;
	}
}

HRESULT CBoss_Koofu::FuitCreate()
{
	CSkill_Koofu_Fuit::SKILL_FUIT_DESC Desc = {};

	Desc.iBulletCnt = 1;
	Desc.pTargetTransform = m_pTransformCom;
	Desc.pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_KOOFU, TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_Skill_Koofu_Fuit"), TEXT("Layer_Fuit"), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Koofu::CloneCreate()
{
	CMon_Copy_Koofu::MON_COPY_KOOFU_DESC			Copykoofu{};

	Copykoofu.iHp = 1;
	Copykoofu.iAttack = 1;
	Copykoofu.pTargetTransform  = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_KOOFU, TEXT("Layer_Player"), TEXT("Com_Transform")));

	for (int i = 0; i < 3; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_Koofu_Copy"), TEXT("Layer_Boss_Koofu_Copy"), &Copykoofu)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CBoss_Koofu::CircleCreate()
{
	CSkill_Monster::SKILL_MONSTER__DESC Desc = {};

	Desc.iBulletCnt = 0;
	Desc.pTargetTransform = m_pTransformCom;

	for (int i = 1; i <= 3; ++i)
	{
		Desc.iBulletCnt = i;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_Skill_Koofu_Bubble"), TEXT("Layer_Bubble"), &Desc)))
			return E_FAIL;
	}

	return S_OK;
}


CBoss_Koofu* CBoss_Koofu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoss_Koofu* pInstance = new CBoss_Koofu(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Boss_Koofu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Koofu::Clone(void* pArg)
{
	CBoss_Koofu* pInstance = new CBoss_Koofu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{

		MSG_BOX(TEXT("Failed to Cloned : Boss_Koofu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Koofu::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pPlayerTransform); 
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
