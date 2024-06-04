#include "stdafx.h"

#include "GameInstance.h"

#include "Mon_Trash_Slime.h"
#include "Effect_Monster.h"
#include "Player.h"

CMon_Trash_Slime::CMon_Trash_Slime(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster{ pGraphic_Device }
{
}

CMon_Trash_Slime::CMon_Trash_Slime(const CMon_Trash_Slime& Prototype)
	:CMonster{ Prototype }
{
}

HRESULT CMon_Trash_Slime::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMon_Trash_Slime::Initialize(void* pArg)
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

	if( m_tMonsterDesc.iSpawnNum == 1)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(34.f, 0.5f, 26.f));
	else if(m_tMonsterDesc.iSpawnNum == 2)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(36.f, 0.5f, 26.f));

	m_eMon_State = MON_STATE::IDLE;
	m_eAnim_State = ANIM_STATE::IDLE;
	
	return S_OK;
}

void CMon_Trash_Slime::Priority_Update(_float fTimeDelta)
{
	__super::Move_Dir(fTimeDelta);

	m_fMovetTimer += fTimeDelta;

	if (m_fMovetTimer >= 0.5f)
	{
		m_bMoveStop = false;
		m_fMovetTimer = { 0.f };
	}

 	m_vTargetDistance = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_fAttackRange = D3DXVec3Length(&m_vTargetDistance);
}

void CMon_Trash_Slime::Update(_float fTimeDelta)
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
			m_fAlpha = 255.f;
			m_bHit = false;
		}

	}

	Destory();
	Mon_State(fTimeDelta);
}

void CMon_Trash_Slime::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CMon_Trash_Slime::Render(_float fTimeDelta)
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

HRESULT CMon_Trash_Slime::Ready_Components()
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

HRESULT CMon_Trash_Slime::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_Down"), TEXT("Trash_Idle_Down"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_Left"), TEXT("Trash_Idle_Left"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_LeftDown"), TEXT("Trash_Idle_LeftDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_LeftUp"), TEXT("Trash_Idle_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_Right"), TEXT("Trash_Idle_Right"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_RightDown"), TEXT("Trash_Idle_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_RightUp"), TEXT("Trash_Idle_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_Up"), TEXT("Trash_Idle_Up"));

	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_Down"), TEXT("Trash_Walk_Down"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_Left"), TEXT("Trash_Walk_Left"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_LeftDown"), TEXT("Trash_Walk_LeftDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_LeftUp"), TEXT("Trash_Walk_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_Right"), TEXT("Trash_Walk_Right"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_RightDown"), TEXT("Trash_Walk_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_RightUp"), TEXT("Trash_Walk_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_Up"), TEXT("Trash_Walk_Up"));

	return S_OK;
}

HRESULT CMon_Trash_Slime::Begin_RenderState()
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

HRESULT CMon_Trash_Slime::End_RenderState()
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

void CMon_Trash_Slime::Anim_State(_float fTimeDelta)
{
	switch (m_eAnim_State)
	{
	case ANIM_STATE::IDLE:
		switch (m_eMon_Dir)
		{
		case MON_DIR::DIR_D:
			m_pAnimCom->Play_Animator(TEXT("Trash_Idle_Down"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_L:
			m_pAnimCom->Play_Animator(TEXT("Trash_Idle_Left"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_LD:
			m_pAnimCom->Play_Animator(TEXT("Trash_Idle_LeftDown"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_LU:
			m_pAnimCom->Play_Animator(TEXT("Trash_Idle_LeftUp"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_R:
			m_pAnimCom->Play_Animator(TEXT("Trash_Idle_Right"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_RD:
			m_pAnimCom->Play_Animator(TEXT("Trash_Idle_RightDown"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_RU:
			m_pAnimCom->Play_Animator(TEXT("Trash_Idle_RightUp"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_U:
			m_pAnimCom->Play_Animator(TEXT("Trash_Idle_Up"), 1.f, fTimeDelta, true);
			break;
		}
		break;

	case ANIM_STATE::MOVE:
		switch (m_eMon_Dir)
		{
		case MON_DIR::DIR_D:
			m_pAnimCom->Play_Animator(TEXT("Trash_Walk_Down"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_L:
			m_pAnimCom->Play_Animator(TEXT("Trash_Walk_Left"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_LD:
			m_pAnimCom->Play_Animator(TEXT("Trash_Walk_LeftDown"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_LU:
			m_pAnimCom->Play_Animator(TEXT("Trash_Walk_LeftUp"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_R:
			m_pAnimCom->Play_Animator(TEXT("Trash_Walk_Right"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_RD:
			m_pAnimCom->Play_Animator(TEXT("Trash_Walk_RightDown"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_RU:
			m_pAnimCom->Play_Animator(TEXT("Trash_Walk_RightUp"), 1.f, fTimeDelta, true);
			break;

		case MON_DIR::DIR_U:
			m_pAnimCom->Play_Animator(TEXT("Trash_Walk_Up"), 1.f, fTimeDelta, true);
			break;
		}
		break;
	}
}

void CMon_Trash_Slime::Mon_State(_float fTimeDelta)
{
	switch (m_eMon_State)
	{
	case MON_STATE::IDLE:
		State_Idle(fTimeDelta);
		break;

	case MON_STATE::MOVE:
		State_Move(fTimeDelta);
		break;
	}
}

void CMon_Trash_Slime::State_Idle(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::IDLE;

	if (m_fAttackRange <= 10.f)
		m_eMon_State = MON_STATE::MOVE;

}

void CMon_Trash_Slime::State_Move(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::MOVE;

	if (!m_bMoveStop)
		m_pTransformCom->Chase(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), fTimeDelta, 0.75f);
		
	Move(fTimeDelta);

	if (m_fAttackRange > 10.f)
		m_eMon_State = MON_STATE::IDLE;

}

void CMon_Trash_Slime::Move(_float fTimeDelta)
{
	if(m_pTimerCom->Time_Limit(fTimeDelta,1.f))
		m_pTransformCom->Go_Up(fTimeDelta + 0.2f);

	m_pTransformCom->Gravity(0.2f, 0.5f, fTimeDelta);
}

void CMon_Trash_Slime::Destory()
{
	CEffect_Monster::EFFECT_MONSTER__DESC Desc = {};
	Desc.pTargetTransform = m_pTransformCom;

	CMon_Trash_Slime* pThis = this;

	if (m_tMonsterDesc.iCurrentHp <= 0)
	{
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_Effect_Mon_Destory"), TEXT("Layer_Effect_Mon_Destroy"), &Desc);

		m_pGameInstance->Play_Sound(L"SFX_MonsterGarbage_Death", LEVEL_STATIC, false);

		Safe_Release(pThis);
	}
}

void CMon_Trash_Slime::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

}

void CMon_Trash_Slime::OnCollisionStay(CCollider* other, _float fTimeDelta)
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

		m_pGameInstance->Play_Sound(L"SFX_MonsterGarbage_Hit", LEVEL_STATIC, false);

		m_bHit = true;
		m_bMoveStop = true;
	}
}

void CMon_Trash_Slime::OnCollisionExit(CCollider* other)
{
}

CMon_Trash_Slime* CMon_Trash_Slime::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMon_Trash_Slime* pInstance = new CMon_Trash_Slime(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Mon_Trash_Slime"));
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject* CMon_Trash_Slime::Clone(void* pArg)
{
	CMon_Trash_Slime* pInstance = new CMon_Trash_Slime(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : Mon_Trash_Slime"));
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CMon_Trash_Slime::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pPlayerTransform);
	
	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
