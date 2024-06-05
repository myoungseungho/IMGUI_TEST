#include "stdafx.h"

#include "Mon_Bear_Solider.h"

#include "GameInstance.h"
#include "Player.h"

#include"Effect_Monster.h"

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

	m_tMonsterDesc.iCurrentHp = pDesc->iCurrentHp;
	m_tMonsterDesc.iAttack = pDesc->iAttack;
	m_tMonsterDesc.iSpawnNum = pDesc->iSpawnNum;
	m_pPlayerTransform = pDesc->pTargetTransform;

	Safe_AddRef(m_pPlayerTransform);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	if (m_tMonsterDesc.iSpawnNum == 1)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(43.f, 1.0f, 23.f));
	else if (m_tMonsterDesc.iSpawnNum == 2)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(43.f, 1.0f, 20.f));
	else if (m_tMonsterDesc.iSpawnNum == 3)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(32.f, 1.0f, 33.f));
	else if (m_tMonsterDesc.iSpawnNum == 4)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(37.f, 1.0f, 33.f));

	m_eMon_State = MON_STATE::IDLE;
	m_eAnim_State = ANIM_STATE::IDLE;

	m_fAttackRange = 1.5f;

	return S_OK;
}

void CMon_Bear_Solider::Priority_Update(_float fTimeDelta)
{
	if(m_pTimerCom->Time_Limit(fTimeDelta , 0.25f))
		m_fAlpha = 255.f;

	m_fMovetTimer += fTimeDelta;
	
	if (m_fMovetTimer >= 2.5f)
	{
		m_bMoveStop = false;
		m_fMovetTimer = { 0.f };
	}

	if(m_eMon_State != MON_STATE::ATTACK)
		__super::Move_Dir(fTimeDelta);

	m_vTargetDistance = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_fMoveRange = D3DXVec3Length(&m_vTargetDistance);
}

void CMon_Bear_Solider::Update(_float fTimeDelta)
{
	if (m_bHit)
	{
		m_fAlphaTimer += fTimeDelta;

		if (m_fAlphaTimer >= 0.25f)
		{
			m_fAlpha = 50.f;
		}
		else
			m_fAlpha = 255.f;

		if (m_fAlphaTimer >= 0.5f)
		{
			m_fAlphaTimer = 0.f;
			m_bHit = false;
		}

	}

	Mon_State(fTimeDelta);
}

void CMon_Bear_Solider::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
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
	TransformDesc.fSpeedPerSec = 2.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(2.f, 2.f, 1.f));

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
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 컬링 모드 설정
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 텍스처 페이저 설정
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(static_cast<DWORD>(m_fAlpha), 255, 255, 255));
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return S_OK;
}

HRESULT CMon_Bear_Solider::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	// 알파 블렌딩 비활성화
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// 원래의 컬링 모드로 복원
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 알파 블렌딩 비활성화 및 텍스처 스테이지 상태 복원
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	return S_OK;
}

void CMon_Bear_Solider::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

}

void CMon_Bear_Solider::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

	CPlayer* pPlayer = static_cast<CPlayer*>(otherObject);

	if (pPlayer->Get_Player_CurState() == CPlayer::STATE_ATTACK && !m_bHit)
	{
		_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float3 vDir = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
		vDir.y = 0.0f;

		vPosition += *D3DXVec3Normalize(&vDir, &vDir) * 0.25f;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPosition);

		m_bHit = true;
		m_bMoveStop = true;

		// 사운드 재생
		m_pGameInstance->Play_Sound(L"SFX_BearWhiteGuard_Hit", LEVEL_STATIC, false);

		// 볼륨 설정
		m_pGameInstance->Set_Volume(L"SFX_BearWhiteGuard_Hit", LEVEL_STATIC, 0.2f);
	}

	if (dynamic_cast<CMonster*>(otherObject))
	{
		CComponent* other_component = otherObject->Get_Component(TEXT("Com_Transform"));
		CTransform* other_transform = static_cast<CTransform*>(other_component);

		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vTargetPos = other_transform->Get_State(CTransform::STATE_POSITION);

		_float3 vDir = vPos - vTargetPos;

		vPos += *D3DXVec3Normalize(&vDir, &vDir) * 3.f * fTimeDelta;
		vPos.y = 1.f;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);
	}
}

void CMon_Bear_Solider::OnCollisionExit(CCollider* other)
{

}

void CMon_Bear_Solider::Anim_State(_float fTimeDelta)
{
	switch (m_eAnim_State)
	{
	case ANIM_STATE::IDLE:
		m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Idle"), 1.f, fTimeDelta, true);
		break;
	case ANIM_STATE::ATTACK:
		switch (m_eMon_Dir)
		{
		case MON_DIR::DIR_D:
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Attack_Down"), 1.f, fTimeDelta, true);
			break;							
											
		case MON_DIR::DIR_L:				
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Attack_Left"), 1.f, fTimeDelta, true);
			break;									
													
		case MON_DIR::DIR_LD:						
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Attack_LeftDown"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_LU:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Attack_LeftUp"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_R:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Attack_Right"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_RD:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Attack_RightDown"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_RU:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Attack_RightUp"), 1.f, fTimeDelta, true);
			break;										 
														 
		case MON_DIR::DIR_U:							 
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Attack_Up"), 1.f, fTimeDelta, true);
			break;
		}
		break;

	case ANIM_STATE::MOVE:
		switch (m_eMon_Dir)
		{
		case MON_DIR::DIR_D:
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Walk_Down"), 1.f, fTimeDelta, true);
			break;									
													
		case MON_DIR::DIR_L:						
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Walk_Left"), 1.f, fTimeDelta, true);
			break;									
													
		case MON_DIR::DIR_LD:						
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Walk_LeftDown"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_LU:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Walk_LeftUp"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_R:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Walk_Right"), 1.f, fTimeDelta, true);
			break;									
													
		case MON_DIR::DIR_RD:						
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Walk_RightDown"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_RU:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Walk_RightUp"), 1.f, fTimeDelta, true);
			break;									
													
		case MON_DIR::DIR_U:						
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Walk_Up"), 1.f, fTimeDelta, true);
			break;
		}
		break;

	case ANIM_STATE::HIT:
		switch (m_eMon_Dir)
		{
		case MON_DIR::DIR_D:
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Hit_Down"), 1.f, fTimeDelta, true);
			break;							
											
		case MON_DIR::DIR_L:				
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Hit_Left"), 1.f, fTimeDelta, true);
			break;						
										
		case MON_DIR::DIR_LD:			
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Hit_LeftDown"), 1.f, fTimeDelta, true);
			break;						
										
		case MON_DIR::DIR_LU:			
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Hit_LeftUp"), 1.f, fTimeDelta, true);
			break;						
										
		case MON_DIR::DIR_R:			
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Hit_Right"), 1.f, fTimeDelta, true);
			break;						
										
		case MON_DIR::DIR_RD:			
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Hit_RightDown"), 1.f, fTimeDelta, true);
			break;						
										
		case MON_DIR::DIR_RU:			
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Hit_RightUp"), 1.f, fTimeDelta, true);
			break;							
											
		case MON_DIR::DIR_U:				
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Hit_Up"), 1.f, fTimeDelta, true);
			break;
		}
		break;

	case ANIM_STATE::STUN:
		switch (m_eMon_Dir)
		{
		case MON_DIR::DIR_D:
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Stun_Down"), 1.f, fTimeDelta, true);
			break;									
													
		case MON_DIR::DIR_L:						
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Stun_Left"), 1.f, fTimeDelta, true);
			break;										 
														 
		case MON_DIR::DIR_LD:							 
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Stun_LeftDown"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_LU:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Stun_LeftUp"), 1.f, fTimeDelta, true);
			break;									
													
		case MON_DIR::DIR_R:						
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Stun_Right"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_RD:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Stun_RightDown"), 1.f, fTimeDelta, true);
			break;									
													
		case MON_DIR::DIR_RU:						
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Stun_RightUp"), 1.f, fTimeDelta, true);
			break;										 
														 
		case MON_DIR::DIR_U:							 
			m_pAnimCom->Play_Animator(TEXT("Bear_Soldier_Stun_Up"), 1.f, fTimeDelta, true);
			break;
		}
		break;
	}
}

void CMon_Bear_Solider::Mon_State(_float fTimeDelta)
{
	switch (m_eMon_State)
	{
	case MON_STATE::IDLE:
		State_Idle(fTimeDelta);
		break;

	case MON_STATE::MOVE:
		State_Move(fTimeDelta);
		break;

	case MON_STATE::ATTACK:
		State_Attack(fTimeDelta);
		break;

	case MON_STATE::HIT:
		State_Hit(fTimeDelta);
		break;

	case MON_STATE::STUN:
		State_Stun(fTimeDelta);
		break;

	}
}

void CMon_Bear_Solider::State_Idle(_float fTimeDelta)
{
	if (m_fMoveRange <= 5.f)
	{
		m_ePrev_State = MON_STATE::IDLE;
		m_eMon_State = MON_STATE::MOVE;
	}

	
}

void CMon_Bear_Solider::State_Move(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::MOVE;

	_float3 TargetPos;
	TargetPos.x = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION).x;
	TargetPos.y = 1.f;
	TargetPos.z = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION).z;

	if(m_pTimerCom->Time_Limit(fTimeDelta, 0.5f))
	{
		// 볼륨 설정
		m_pGameInstance->Set_Volume(L"SFX_BearWhiteGuard_Walk", LEVEL_STATIC, 0.2f);
		// 사운드 재생
		m_pGameInstance->Play_Sound(L"SFX_BearWhiteGuard_Walk", LEVEL_STATIC, false);
	}

	if(!m_bMoveStop)
		m_pTransformCom->Chase(TargetPos, fTimeDelta);


	if (m_fMoveRange <= m_fAttackRange)
	{
		m_ePrev_State = MON_STATE::MOVE;
		m_eMon_State = MON_STATE::ATTACK;
	}

	if (m_tMonsterDesc.iCurrentHp <= 0)
	{
		m_ePrev_State = MON_STATE::MOVE;
		m_eMon_State = MON_STATE::STUN;
	}
}

void CMon_Bear_Solider::State_Attack(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::ATTACK;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_SNOW, TEXT("Layer_Player")));


	m_fAttackTimer += fTimeDelta;

	if (m_bHit)
	{
		m_ePrev_State = MON_STATE::ATTACK;
		m_eMon_State = MON_STATE::MOVE;
	}

	if ((m_fMoveRange > m_fAttackRange) && m_fAttackTimer >= 1.f)
	{
		m_ePrev_State = MON_STATE::ATTACK;
		m_eMon_State = MON_STATE::MOVE;

		m_fAttackTimer = 0.f;
	}

	if (m_fAttackTimer >= 1.f )
	{
		// 사운드 재생
		m_pGameInstance->Play_Sound(L"SFX_BearWhiteGuard_Attack", LEVEL_STATIC, false);

		m_fAttackTimer = 0.f;

		if (m_fMoveRange <= m_fAttackRange)
		{
			pPlayer->Set_Player_Hp(-1);
		}
	}

	if (m_tMonsterDesc.iCurrentHp <= 0)
	{
		m_ePrev_State = MON_STATE::ATTACK;
		m_eMon_State = MON_STATE::STUN;
	}
}

void CMon_Bear_Solider::State_Hit(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::HIT;

	if (m_pTimerCom->Time_Limit(fTimeDelta, 1.f))
	{
		m_ePrev_State = MON_STATE::HIT;
		m_eMon_State = MON_STATE::STUN;
	}
}

void CMon_Bear_Solider::State_Stun(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::STUN;

	CEffect_Monster::EFFECT_MONSTER__DESC Desc = {};
	Desc.pTargetTransform = m_pTransformCom;

	m_fAlphaTimer += fTimeDelta;

	if (m_fAlphaTimer >= 0.25f)
	{
		m_fAlpha = 50.f;
	}
	else
		m_fAlpha = 255.f;


	if (m_fAlphaTimer >= 0.5f)
		m_fAlphaTimer = 0.f;

	if (!m_bStunEffect)
	{
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_Stun"), TEXT("Layer_Effect_Stun"), &Desc);

		// 사운드 재생
		m_pGameInstance->Play_Sound(L"SFX_BearWhiteGuard_Death", LEVEL_STATIC, false);

		m_bStunEffect = true;
	}

	m_fDeathTimer += fTimeDelta;

	if ((m_ePrev_State == MON_STATE::MOVE || m_ePrev_State == MON_STATE::ATTACK ) && m_fDeathTimer >= 4.f)
	{
		Destroy();
	}

}

void CMon_Bear_Solider::Destroy()
{
	CMon_Bear_Solider* pThis = this;
	Safe_Release(pThis);
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
