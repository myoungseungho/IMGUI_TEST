#include "stdafx.h"

#include "Mon_Bear_Cannon.h"
#include "GameInstance.h"

#include "Skill_Cannon_Ball.h"
#include "Skill_Player.h"

#include "Effect_Monster.h"

CMon_Bear_Cannon::CMon_Bear_Cannon(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster{ pGraphic_Device }
{
}

CMon_Bear_Cannon::CMon_Bear_Cannon(const CMon_Bear_Cannon& Prototype)
	:CMonster{ Prototype }
{
}

HRESULT CMon_Bear_Cannon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMon_Bear_Cannon::Initialize(void* pArg)
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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(37.f, 1.f, 38.f));

	m_eMon_State = MON_STATE::IDLE;
	m_eAnim_State = ANIM_STATE::IDLE;

	return S_OK;
}

void CMon_Bear_Cannon::Priority_Update(_float fTimeDelta)
{
	__super::Move_Dir(fTimeDelta);
	m_fInitAlhpaTimer += fTimeDelta;

	if (m_fInitAlhpaTimer >= 0.5f)
	{
		m_fAlpha = 255.f;
		m_fInitAlhpaTimer = { 0.f };
	}


	m_vTargetDistance = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_fMoveRange = D3DXVec3Length(&m_vTargetDistance);
}

void CMon_Bear_Cannon::Update(_float fTimeDelta)
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

void CMon_Bear_Cannon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CMon_Bear_Cannon::Render(_float fTimeDelta)
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

HRESULT CMon_Bear_Cannon::Ready_Components()
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

HRESULT CMon_Bear_Cannon::Ready_Animation()
{
	/* Bear_Cannon_Idle */
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_Down"), TEXT("Bear_Cannon_Idle_Down"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_Left"), TEXT("Bear_Cannon_Idle_Left"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_LeftDown"), TEXT("Bear_Cannon_Idle_LeftDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_LeftUp"), TEXT("Bear_Cannon_Idle_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_Right"), TEXT("Bear_Cannon_Idle_Right"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_RightDown"), TEXT("Bear_Cannon_Idle_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_RightUp"), TEXT("Bear_Cannon_Idle_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_Up"), TEXT("Bear_Cannon_Idle_Up"));

	/* Bear_Cannon_Attack */
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_Down"), TEXT("Bear_Cannon_Attack_Down"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_Left"), TEXT("Bear_Cannon_Attack_Left"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_LeftDown"), TEXT("Bear_Cannon_Attack_LeftDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_LeftUp"), TEXT("Bear_Cannon_Attack_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_Right"), TEXT("Bear_Cannon_Attack_Right"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_RightDown"), TEXT("Bear_Cannon_Attack_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_RightUp"), TEXT("Bear_Cannon_Attack_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_Up"), TEXT("Bear_Cannon_Attack_Up"));

	/* Bear_Cannon_Stun */
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_Down"), TEXT("Bear_Cannon_Stun_Down"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_Left"), TEXT("Bear_Cannon_Stun_Left"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_LeftDown"), TEXT("Bear_Cannon_Stun_LeftDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_LeftUp"), TEXT("Bear_Cannon_Stun_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_Right"), TEXT("Bear_Cannon_Stun_Right"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_RightDown"), TEXT("Bear_Cannon_Stun_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_RightUp"), TEXT("Bear_Cannon_Stun_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_Up"), TEXT("Bear_Cannon_Stun_Up"));

	return S_OK;
}

HRESULT CMon_Bear_Cannon::Begin_RenderState()
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

HRESULT CMon_Bear_Cannon::End_RenderState()
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

void CMon_Bear_Cannon::Mon_State(_float fTimeDelta)
{
	switch (m_eMon_State)
	{
	case MON_STATE::IDLE:
		State_Idle(fTimeDelta);
		break;

	case MON_STATE::ATTACK:
		State_Attack(fTimeDelta);
		break;

	case MON_STATE::STUN:
		State_Stun(fTimeDelta);
		break;
	}
}

void CMon_Bear_Cannon::Anim_State(_float fTimeDelta)
{
	switch (m_eAnim_State)
	{
	case ANIM_STATE::IDLE:
		switch (m_eMon_Dir)
		{
		case MON_DIR::DIR_D:
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Idle_Down"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_L:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Idle_Left"), 1.f, fTimeDelta, true);
			break;									
													
		case MON_DIR::DIR_LD:						
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Idle_LeftDown"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_LU:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Idle_LeftUp"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_R:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Idle_Right"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_RD:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Idle_RightDown"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_RU:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Idle_RightUp"), 1.f, fTimeDelta, true);
			break;									
													
		case MON_DIR::DIR_U:						
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Idle_Up"), 1.f, fTimeDelta, true);
			break;
		}
		break;
	case ANIM_STATE::ATTACK:
		switch (m_eMon_Dir)
		{
		case MON_DIR::DIR_D:
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Attack_Down"), 1.f, fTimeDelta, true);
			break;							
											
		case MON_DIR::DIR_L:				
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Attack_Left"), 1.f, fTimeDelta, true);
			break;							
											
		case MON_DIR::DIR_LD:				
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Attack_LeftDown"), 1.f, fTimeDelta, true);
			break;								
												
		case MON_DIR::DIR_LU:					
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Attack_LeftUp"), 1.f, fTimeDelta, true);
			break;								
												
		case MON_DIR::DIR_R:					
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Attack_Right"), 1.f, fTimeDelta, true);
			break;								
												
		case MON_DIR::DIR_RD:					
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Attack_RightDown"), 1.f, fTimeDelta, true);
			break;								
												
		case MON_DIR::DIR_RU:					
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Attack_RightUp"), 1.f, fTimeDelta, true);
			break;								
												
		case MON_DIR::DIR_U:					
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Attack_Up"), 1.f, fTimeDelta, true);
			break;
		}
		break;

	case ANIM_STATE::STUN:
		switch (m_eMon_Dir)
		{
		case MON_DIR::DIR_D:
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Stun_Down"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_L:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Stun_Left"), 1.f, fTimeDelta, true);
			break;									
													
		case MON_DIR::DIR_LD:						
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Stun_LeftDown"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_LU:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Stun_LeftUp"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_R:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Stun_Right"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_RD:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Stun_RightDown"), 1.f, fTimeDelta, true);
			break;									
													
		case MON_DIR::DIR_RU:						
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Stun_RightUp"), 1.f, fTimeDelta, true);
			break;										
														
		case MON_DIR::DIR_U:							
			m_pAnimCom->Play_Animator(TEXT("Bear_Cannon_Stun_Up"), 1.f, fTimeDelta, true);
			break;
		}
		break;
	}
}

void CMon_Bear_Cannon::State_Idle(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::IDLE;

	if (m_fMoveRange <= 10.f && m_pTimerCom->Time_Limit(fTimeDelta , 2.f))
	{
		m_eMon_State = MON_STATE::ATTACK;
	}

	if (m_tMonsterDesc.iHp <= 0)
	{
		m_eMon_State = MON_STATE::STUN;
	}
}

void CMon_Bear_Cannon::State_Attack(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::ATTACK;

	if (m_pTimerCom->Time_Limit(fTimeDelta, 1.f))
	{
		Attack();
		m_eMon_State = MON_STATE::IDLE;
	}

	if (m_fMoveRange > 10.f)
	{
		m_eMon_State = MON_STATE::IDLE;
	}

	if (m_tMonsterDesc.iHp <= 0)
	{
		m_eMon_State = MON_STATE::STUN;
	}
}

void CMon_Bear_Cannon::State_Stun(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::STUN;
	CMon_Bear_Cannon* pThis = this;

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
		m_bStunEffect = true;
	}

	if (m_pTimerCom->Time_Limit(fTimeDelta, 4.f))
	{
		Safe_Release(pThis);
	}
}

HRESULT CMon_Bear_Cannon::Attack()
{
	CSkill_Cannon_Ball::SKILL_CANNON_DESC  Desc{};
	Desc.pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_SNOW, TEXT("Layer_Player"), TEXT("Com_Transform")));
	Desc.pTargetTransform = m_pTransformCom;
	Desc.iTotalBullet = 1;
	Desc.iBulletCnt = 0;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_Skill_Cannon_Ball"), TEXT("Layer_Cannon_Ball"), &Desc)))
		return E_FAIL;
}



void CMon_Bear_Cannon::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{

}

void CMon_Bear_Cannon::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

	CPlayer* pPlayer = static_cast<CPlayer*>(otherObject);

	if (dynamic_cast<CSkill_Player*>(otherObject) && !m_bHit)
	{
		_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		CSkill_Player* pSkill = dynamic_cast<CSkill_Player*>(otherObject);
		CTransform* pSkillTransform = dynamic_cast<CTransform*>(pSkill->Get_Component(TEXT("Com_Transform")));

		_float3 vDir = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pSkillTransform->Get_State(CTransform::STATE_POSITION);
		vDir.y = 0.0f;

		vPosition += *D3DXVec3Normalize(&vDir, &vDir) * 0.5f;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPosition);

		m_pGameInstance->Play_Sound(L"SFX_BearWhiteGuard_Hit", LEVEL_STATIC, false);
		return;
	}
}

void CMon_Bear_Cannon::OnCollisionExit(CCollider* other)
{
}

CMon_Bear_Cannon* CMon_Bear_Cannon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMon_Bear_Cannon* pInstance = new CMon_Bear_Cannon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Mon_Bear_Cannon"));
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject* CMon_Bear_Cannon::Clone(void* pArg)
{
	CMon_Bear_Cannon* pInstance = new CMon_Bear_Cannon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : Mon_Bear_Cannon"));
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CMon_Bear_Cannon::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pPlayerTransform);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
