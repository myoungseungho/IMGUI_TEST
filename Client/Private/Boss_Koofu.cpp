#include "stdafx.h"

#include "Boss_Koofu.h"
#include "GameInstance.h"

#include "Skill_Monster.h"
#include "Skill_Koofu_Fuit.h"

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

	m_pTargetTransform = pDesc->m_pTargetTransform;
	m_isClone = pDesc->isClone;
	m_isCheck = pDesc->isCheck;
	m_tMonsterDesc.iHp = pDesc->iHp;
	Safe_AddRef(m_pTargetTransform);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

 	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(3.f, 3.f, 1.f));

	m_eMon_State = MON_STATE::IDLE;
	m_eAnim_State = ANIM_STATE::IDLE;
	m_eMon_Dir = MON_DIR::DIR_D;

	if (m_isClone)
	{
		Warf(20.f, 30.f, 10.f);
		m_eMon_State = MON_STATE::BULLET;
		m_tMonsterDesc.iHp = pDesc->iHp;

	}
	else
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(30, 1.5f, 20));


	return S_OK;
}


void CBoss_Koofu::Priority_Update(_float fTimeDelta)
{

}

void CBoss_Koofu::Update(_float fTimeDelta)
{
	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), 180 * D3DX_PI / 180.f);
	//BillBoarding();

	Move_Dir();
	Key_Input(fTimeDelta);
	MonState(fTimeDelta);
	//Distory();
}

void CBoss_Koofu::Late_Update(_float fTimeDelta)
{ 
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

}

HRESULT CBoss_Koofu::Render(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

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

	case MON_STATE::READY:
		State_Ready(fTimeDelta);
		break;

	case MON_STATE::BULLET:
		State_Bullet(fTimeDelta);
		break;

	case MON_STATE::BULLET_B:
		State_Bullet_B(fTimeDelta);
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
		m_pAnimCom->Play_Animator(TEXT("KOOFU_STUN"), 1.f, fTimeDelta, true);
		break;

	case ANIM_STATE::READY:
		m_pAnimCom->Play_Animator(TEXT("KOOFU_READY"), 1.f, fTimeDelta, true);
		break;

	case ANIM_STATE::DEADTH:
		m_pAnimCom->Play_Animator(TEXT("KOOFU_DEATH"), 1.f, fTimeDelta, true);
		break;
	}

}

void CBoss_Koofu::State_Idle(_float fTimeDelta)
{
	if (m_pTimerCom->Time_Limit(fTimeDelta, 2.f))
		m_eMon_State = MON_STATE::CAST;

	m_ePrev_State = MON_STATE::IDLE;
}

void CBoss_Koofu::State_Ready(_float fTimeDelta)
{
	m_ePrev_State = MON_STATE::READY;
}

void CBoss_Koofu::State_Bullet(_float fTimeDelta)
{
	m_ePrev_State = MON_STATE::BULLET;

	if (!m_isClone)
	{
		CloneCreate();
		m_isClone = true;

		if (m_pTimerCom->Time_Limit(fTimeDelta, 2.f))
			Warf(10, 10, 30, 30);
	}

	if (m_pTimerCom->Time_Limit(fTimeDelta, 5.f, 1.f))
	{
		m_eAnim_State = ANIM_STATE::THROW;

		if (m_pTimerCom->Time_Limit(fTimeDelta, 1.f))
			FuitCreate();
	}
	else
	{
		m_eAnim_State = ANIM_STATE::IDLE;
	}
}

void CBoss_Koofu::State_Bullet_B(_float fTimeDelta)
{
	m_ePrev_State = MON_STATE::BULLET_B;

	if (m_pTimerCom->Time_Limit(fTimeDelta, 3.f))
	{
		RollingCreate();
	}
}

void CBoss_Koofu::State_Cast(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::CAST;

	if (m_ePrev_State == MON_STATE::IDLE && m_pTimerCom->Time_Limit(fTimeDelta, 2.f))
	{
		m_ePrev_State = MON_STATE::CAST;
		m_eMon_State = MON_STATE::BULLET;

	}
	else if (m_ePrev_State == MON_STATE::BULLET)
	{
		m_ePrev_State = MON_STATE::CAST;
		m_eMon_State = MON_STATE::BULLET_B;
	}
}

void CBoss_Koofu::Move_Dir()
{
	_float fAngle = m_pTransformCom->Dir_Degree();

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

void CBoss_Koofu::Key_Input(_float fTimeDelta)
{
	CBoss_Koofu* pKoofu = this;

	if(m_pKeyCom->Key_Down('1'))
		Warf(-10, -10, 20, 20);

	if (m_pKeyCom->Key_Down('2'))
		RollingCreate();

	if (m_pKeyCom->Key_Down('3'))
		FuitCreate();

	if (m_pKeyCom->Key_Down('4') && m_isCheck)
	{
		Safe_Release(pKoofu);
	}

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

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
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

	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
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

HRESULT CBoss_Koofu::Ready_Animation()
{
	//Idle
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_Down"), TEXT("KOOFU_IDLE_DOWN"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_Left"), TEXT("KOOFU_IDLE_LEFT"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_LeftDown"), TEXT("KOOFU_IDLE_LEFTDOWN"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_LeftUp"), TEXT("KOOFU_IDLE_LEFTUP"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_Right"), TEXT("KOOFU_IDLE_RIGHT"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_RightDown"), TEXT("KOOFU_IDLE_RIGHTDOWN"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_RightUp"), TEXT("KOOFU_IDLE_RIGHTUP"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_Up"), TEXT("KOOFU_IDLE_UP"));

	//Death
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Death"), TEXT("KOOFU_DEATH"));

	//Ready
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Ready"), TEXT("KOOFU_READY"));

	//Cast
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Cast"), TEXT("KOOFU_CAST"));

	//Throw
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_Down"), TEXT("KOOFU_THROW_DOWN"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_Left"), TEXT("KOOFU_THROW_LEFT"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_LeftDown"), TEXT("KOOFU_THROW_LEFTDOWN"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_LeftUp"), TEXT("KOOFU_THROW_LEFTUP"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_Right"), TEXT("KOOFU_THROW_RIGHT"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_RightDown"), TEXT("KOOFU_THROW_RIGHTDOWN"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_RightUp"), TEXT("KOOFU_THROW_RIGHTUP"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_Up"), TEXT("KOOFU_THROW_UP"));

	//Walk
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_Down"), TEXT("KOOFU_WALK_DOWN"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_Left"), TEXT("KOOFU_WALK_LEFT"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_LeftDown"), TEXT("KOOFU_WALK_LEFTDOWN"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_LeftUp"), TEXT("KOOFU_WALK_LEFTUP"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_Right"), TEXT("KOOFU_WALK_RIGHT"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_RightDown"), TEXT("KOOFU_WALK_RIGHTDOWN"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_RightUp"), TEXT("KOOFU_WALK_RIGHTUP"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_Up"), TEXT("KOOFU_WALK_UP"));

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

void CBoss_Koofu::OnCollisionStay(CCollider* other)
{
	int a = 10;
}

void CBoss_Koofu::OnCollisionExit(CCollider* other)
{
	int a = 10;
}

void CBoss_Koofu::ScaleUp(_float fTimeDelta)
{
	if (!m_pTimerCom->Time_Limit(fTimeDelta,1.f ,3.5f))
		m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f) * (fTimeDelta + 1));
}

void CBoss_Koofu::Warf(_int fMinPosX, _int fMinPosZ , _int fMaxPosX , _int fMaxPosZ)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(rand() % fMaxPosX - fMinPosX, 1.5f, rand() % fMaxPosZ - fMinPosZ));
}

void CBoss_Koofu::Warf(_int iPosX, _int iPosZ, _float fDistance)
{
	_float WarfPosX = iPosX + fDistance * cos(rand() % 360 * (D3DX_PI / 180.f));
	_float WarfPosZ = iPosZ - fDistance * sin(rand() % 360 * (D3DX_PI / 180.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(WarfPosX, 1.5f, WarfPosZ));

}

void CBoss_Koofu::Distory()
{
	CBoss_Koofu* pKoofu = this;
}

HRESULT CBoss_Koofu::RollingCreate()
{
	 
	CSkill_Monster::SKILL_MONSTER__DESC Desc = {};

	Desc.iBulletCnt = 0; 
	Desc.pTargetTransform = m_pTransformCom;

	for (int i = 1; i <= 5; ++i)
	{
		Desc.iBulletCnt = i;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Koofu_Rolling"), TEXT("Layer_Rolling") ,&Desc)))
			return E_FAIL;
	}
}

HRESULT CBoss_Koofu::FuitCreate()
{
	CSkill_Koofu_Fuit::SKILL_FUIT_DESC Desc = {};

	Desc.iBulletCnt = 1;
	Desc.pTargetTransform = m_pTransformCom;
	Desc.pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Koofu_Fuit"), TEXT("Layer_Fuit"), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Koofu::CloneCreate()
{
	CBoss_Koofu::BOSS_KOOFU_DESC			Bosskoofu{};

	Bosskoofu.iHp = 1;
	Bosskoofu.iAttack = 1;
	Bosskoofu.isClone = true;
	Bosskoofu.isCheck = true;
	Bosskoofu.m_pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));

	for (int i = 1; i <= 3; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Koofu"), TEXT("Layer_Boss_Koofu"), &Bosskoofu)))
			return E_FAIL;
		
		Warf(-10, -10, 10, 10);
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
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTargetTransform); 
	Safe_Release(m_pColliderCom);
}
