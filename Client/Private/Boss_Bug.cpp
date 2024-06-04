#include "stdafx.h"

#include "Boss_Bug.h"
#include "GameInstance.h"
#include "Mon_Turtle.h"
#include "Player.h"
#include "Skill_Bug_Bullet.h"

#include "Skill_Player.h"
#include "Effect_Monster.h"
#include "Effect_Bug_Line.h"
#include <UI_FadeInOut.h>
#include <Camera.h>


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
	m_pPlayerTransform = pDesc->pTargetTransform;
	m_tMonsterDesc.iCurrentHp = pDesc->iCurrentHp;
	m_tMonsterDesc.iAttack = pDesc->iAttack;

	Safe_AddRef(m_pPlayerTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_eMon_State = MON_STATE::IDLE;

	return S_OK;
}

void CBoss_Bug::Priority_Update(_float fTimeDelta)
{
	m_fAngle++;
	m_fHitTimer += fTimeDelta;
	m_fAlpha = 255.f;

	if (m_fAngle > 360.f)
		m_fAngle = 0.f;

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (vPos.x >= 32.f && vPos.x <= 47.f && vPos.z >= 27.f && vPos.z <= 48.f)
		m_bPosRange = true;
	else
		m_bPosRange = false;

	CGameObject* cameraObject = m_pGameInstance->Get_GameObject(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Camera"));

	static_cast<CCamera*>(cameraObject)->Circle_Moving(_float3(39.5f, 3.f, 36.f), 20.f, m_fAngle, 7.5f, fTimeDelta);
}

void CBoss_Bug::Update(_float fTimeDelta)
{
	Mon_State(fTimeDelta);
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

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 2.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(5.f, 5.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(39.5f, 1.5f, 36.f));
	
	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 0.5f;
	ColliderDesc.MineGameObject = this;

	//콜라이더 사본을 만들때 Cube 정보 추가해줘야 함.
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//콜라이더오브젝트 추가
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_MONSTER, this);

	return S_OK;
}

HRESULT CBoss_Bug::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase1_Idle"), TEXT("BOSS_BUG_PHASE1_IDLE"));
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase1_Ready"), TEXT("BOSS_BUG_PHASE1_READY"));
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase1_Attack"), TEXT("BOSS_BUG_PHASE1_ATTACK"));

	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Ready"), TEXT("BOSS_BUG_PHASE2_READY"));
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Regen"), TEXT("BOSS_BUG_PHASE2_REGEN"));

	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Attack"), TEXT("BOSS_BUG_PHASE2_ATTACK"));
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Death"), TEXT("BOSS_BUG_PHASE2_DEATH"));
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Down"), TEXT("BOSS_BUG_PHASE2_DOWN"));

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

void CBoss_Bug::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CSkill_Bug_Bullet*>(otherObject))
	{
		if ((m_eMon_State == MON_STATE::BULLET) || (m_eMon_State == MON_STATE::IDLE))
		{
			m_tMonsterDesc.iCurrentHp--;
			otherObject->Delete_Object();
		}
		return;
	}	
}

void CBoss_Bug::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

	CPlayer* pPlayer = static_cast<CPlayer*>(otherObject);

	if (pPlayer != nullptr)
	{
		if (pPlayer->Get_Player_CurState() == CPlayer::STATE_ATTACK && m_fHitTimer >= 1.f)
		{
			m_pGameInstance->Play_Sound(L"SFX_BossMoonMoth_Hit", LEVEL_STATIC, false);

			m_fHitTimer = 0.f;
		}
	}

	if (pPlayer->Get_Player_CurState() == CPlayer::STATE_ATTACK)
	{
		m_fAlphaTimer += fTimeDelta;

		if (m_fAlphaTimer >= 0.25f)
		{
			m_fAlpha = 50.f;
		}
		else
			m_fAlpha = 255.f;

		if (m_fAlphaTimer >= 0.5f)
			m_fAlphaTimer = 0.f;
	}

}

void CBoss_Bug::OnCollisionExit(class CCollider* other)
{

}

void CBoss_Bug::Warf(_int iPosX, _int iPosZ, _float fDistance, _float fAngle)
{
	_float WarfPosX = iPosX + fDistance * cos(fAngle * (D3DX_PI / 180.f));
	_float WarfPosZ = iPosZ - fDistance * sin(fAngle * (D3DX_PI / 180.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(WarfPosX, 0.5f, WarfPosZ));

	_float3 PlayerPos;

	PlayerPos.x = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION).x;
	PlayerPos.y = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION).y + 0.5f;
	PlayerPos.z = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION).z;

	m_pTransformCom->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
	_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	m_pTransformCom->Radian_Turn(vRight, 270.f * D3DX_PI / 180.f);


	CEffect_Bug_Line::EFFECT_BUG_LINE_DESC Desc = {};
	Desc.pTargetTransform = m_pTransformCom;
	Desc.pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_BUG, TEXT("Layer_Player"), TEXT("Com_Transform")));

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Effect_Bug_Line"), TEXT("Layer_Effect_Bug_Line"), &Desc);
}

void CBoss_Bug::Skill_Dash(_float fTimeDelta)
{
	m_fDashBulletTimer += fTimeDelta;

	if (m_pTimerCom->Time_Limit(fTimeDelta, 4.f))
	{
		Warf(30, 20, 50.f, m_fAngle);
		m_fDashTimer = 0.f;

		m_pGameInstance->Play_Sound(L"SFX_BossMoonMoth_Fly", LEVEL_STATIC, false);

	}
	else
	{
		m_fDashTimer += fTimeDelta;

		if (m_fDashEffectTimer >= 0.2f)
		{
			CEffect_Monster::EFFECT_MONSTER__DESC Desc = {};
			Desc.pTargetTransform = m_pTransformCom;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Effect_Bug_Dash"), TEXT("Layer_Effect_Bug_Dash"), &Desc);
			m_fDashEffectTimer = 0.f;
		}

		if (m_fDashTimer >= 1.f)
		{
			m_fDashEffectTimer += fTimeDelta;

			m_pTransformCom->Set_Speed(10.f);
			m_pTransformCom->Go_VectorDown(fTimeDelta * 5.f);

			if (m_fDashBulletTimer >= 1.f)
			{
				Bullet_Create(48, CSkill_Bug_Bullet::BULLET_STATE::CIRCLE);
				m_fDashBulletTimer = 0.f;
			}
		}
	}
	
}

void CBoss_Bug::Fly(_float fTimeDelta)
{
	m_fWaveTimer += fTimeDelta;
	if (m_pTimerCom->Time_Limit(fTimeDelta, 8.f))
	{
		m_isLand = false;
		m_eMon_State = MON_STATE::LAND;
	}
	else
	{
		m_pTransformCom->Go_Up(fTimeDelta);

		if (m_fWaveTimer >= 0.2f)
		{
			Wave_Create();
			m_fWaveTimer = { 0.f };
		}
	}
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
		CEffect_Monster::EFFECT_MONSTER__DESC Desc = {};
		Desc.pTargetTransform = m_pTransformCom;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Effect_Bug_Down"), TEXT("Layer_Effect_Bug_Down"), &Desc);

		m_eMon_State = MON_STATE::DASH;
		Bullet_Create(36, CSkill_Bug_Bullet::BULLET_STATE::CIRCLE);
		m_bDown = true;
	}
	else
	{
		m_pTransformCom->Go_Down(fTimeDelta);
	}
}


HRESULT CBoss_Bug::Turtle_Create()
{
	m_pGameInstance->Play_Sound(L"SFX_MonsterBugColorBeatle_In", LEVEL_STATIC, false);


	CMon_Turtle::MON_TURTLE_DESC	 Desc{};

	Desc.iCurrentHp = 3;
	Desc.iAttack = 1;
	Desc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_BUG, TEXT("Layer_Player"), TEXT("Com_Transform")));

	const wstring A[3] = { L"Prototype_Component_Texture_Monster_Red_Turtle" , L"Prototype_Component_Texture_Monster_Green_Turtle" , L"Prototype_Component_Texture_Monster_Blue_Turtle" };

	for (int i = 0; i < 3; ++i)
	{
		Desc.ColorTexTag = A[i];

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Turtle"), TEXT("Layer_Monster_Turtle"), &Desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CBoss_Bug::Bullet_Create(_uint iBulletNum, CSkill_Bug_Bullet::BULLET_STATE iBulletType)
{
	CSkill_Bug_Bullet::BULLET_SKILL_DESC SkillDesc{};
	SkillDesc.pTargetTransform = m_pTransformCom;
	SkillDesc.iTotalBullet = iBulletNum;
	SkillDesc.iBulletType = iBulletType;


	for (int i = 1; i <= SkillDesc.iTotalBullet; ++i)
	{
		SkillDesc.iBulletCnt = i;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Skill_Bug_Bullet"), TEXT("Layer_Skill_Bug_Bullet"), &SkillDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CBoss_Bug::Wave_Create()
{
	CSkill_Monster::SKILL_MONSTER__DESC SkillDesc{};
	SkillDesc.iTotalBullet = 5;
	SkillDesc.iBulletCnt = 0;
	SkillDesc.pTargetTransform = m_pTransformCom;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Skill_Bug_SludgeWave"), TEXT("Layer_Skill_Bug_SludgeWave"), &SkillDesc)))
		return E_FAIL;
	

	return S_OK;
}

void CBoss_Bug::State_Idle(_float  _fTimeDelta)
{
	if (m_pTimerCom->Time_Limit(_fTimeDelta, 3.f))
		m_eMon_State = MON_STATE::BULLET;
}

void CBoss_Bug::State_Bullet(_float  _fTimeDelta)
{
	if (m_pTimerCom->Time_Limit(_fTimeDelta, 0.95f))
	{
		m_pGameInstance->Play_Sound(L"SFX_BossMoonMoth_BulletAttack", LEVEL_STATIC, false);


		Bullet_Create(12, CSkill_Bug_Bullet::BULLET_STATE::NORMAL);
		m_iBulletCnt++;
	}

	if (m_iBulletCnt >= 3)
	{
		m_eMon_State = MON_STATE::IDLE;
		m_iBulletCnt = 0;
	}

	if (m_tMonsterDesc.iCurrentHp <= 5)
	{
		m_iPhaseCnt = 2;
		m_ePrev_State = MON_STATE::BULLET;
		m_eMon_State = MON_STATE::REGEN;
	}
}

void CBoss_Bug::State_Regen(_float _fTimeDelta)
{
	if (m_ePrev_State == MON_STATE::BULLET && m_pTimerCom->Time_Limit(_fTimeDelta, 2.f))
	{
		m_eMon_State = MON_STATE::READY;
	}

	if (m_ePrev_State == MON_STATE::STUN && m_pTimerCom->Time_Limit(_fTimeDelta, 3.f))
	{
		m_eMon_State = MON_STATE::FLY;
	}

}

void CBoss_Bug::State_Stan(_float fTimeDelta)
{
	m_pTransformCom->Gravity(0.1f, 2.f, fTimeDelta);

	if (m_pTimerCom->Time_Limit(fTimeDelta, 5.f))
	{
		m_ePrev_State = MON_STATE::STUN;
		m_eMon_State = MON_STATE::REGEN;
	}
}

void CBoss_Bug::State_Death(_float fTimeDelta)
{
	CBoss_Bug* pThis = this;

	if (m_pTimerCom->Time_Limit(fTimeDelta, 7.f))
	{
		m_pGameInstance->Play_Sound(L"SFX_BossMoonMoth_Death", LEVEL_STATIC, false);

		Safe_Release(pThis);
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
	auto iter = dynamic_cast<CMon_Turtle*>(m_pGameInstance->Get_GameObject(LEVEL_BUG, TEXT("Layer_Monster_Turtle")));

	if (!m_bStartDash)
	{
		m_pGameInstance->Play_Sound(L"SFX_BossMoonMoth_Soar", LEVEL_STATIC, false);

		_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		m_pTransformCom->Rotation(vRight, 90.f * D3DX_PI / 180.f);
		m_bStartDash = true;
	}

	if(iter || !m_bPosRange)
		Skill_Dash(_fTimeDelta);
	
	else if(!iter)
	{
		m_pTransformCom->Set_Speed(2.f);

		if (m_isTurtle && m_bPosRange)
		{
			CGameObject* fadeInOutObject = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_UI_FadeInOut"));
			static_cast<CUI_FadeInOut*>(fadeInOutObject)->StartFading(0.1f, 0.f, 255.f, true, 3.5f);

			m_pGameInstance->Play_Sound(L"SFX_BossMoonMoth_Down", LEVEL_STATIC, false);

			m_pTransformCom->Init_Rotation(_float3(1.f, 1.f, 1.f), 0.f* D3DX_PI / 180.f);

			m_ePrev_State = MON_STATE::DASH;
			m_eMon_State = MON_STATE::STUN;
			m_bStartDash = false;

			Turtle_Create();
		}

		if(m_pTimerCom->Time_Limit(_fTimeDelta, 1.f) && !m_isTurtle)
		{
			Turtle_Create();
			m_isTurtle = true;
		}
	}
}


void CBoss_Bug::State_Fly(_float  _fTimeDelta)
{
	Fly(_fTimeDelta);
}

void CBoss_Bug::State_Land(_float  _fTimeDelta)
{
	Land(39.5f, 36.f, _fTimeDelta);
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

	case MON_STATE::STUN:
		if (m_iPhaseCnt == 2)
			m_pAnimCom->Play_Animator(TEXT("BOSS_BUG_PHASE2_DOWN"), 1.f, _fTimeDelta, false);
		break;

	case MON_STATE::FLY:
		if (m_iPhaseCnt == 2)
			m_pAnimCom->Play_Animator(TEXT("BOSS_BUG_PHASE2_REGEN"), 1.f, _fTimeDelta, false);
		break;

	case MON_STATE::LAND:
		if (m_iPhaseCnt == 2)
			m_pAnimCom->Play_Animator(TEXT("BOSS_BUG_PHASE2_REGEN"), 1.f, _fTimeDelta, false);
		break;
	case MON_STATE::DEATH:
		if (m_iPhaseCnt == 2)
			m_pAnimCom->Play_Animator(TEXT("BOSS_BUG_PHASE2_DEATH"), 1.f, _fTimeDelta, false);
		break;
	}

}

void CBoss_Bug::Mon_State(_float fTimeDelta)
{
	if (m_tMonsterDesc.iCurrentHp <= 0)
	{
		m_eMon_State = MON_STATE::DEATH;
	}
		
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

	case MON_STATE::STUN:
		State_Stan(fTimeDelta);
		break;

	case MON_STATE::DEATH:
		State_Death(fTimeDelta);
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
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pPlayerTransform);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
