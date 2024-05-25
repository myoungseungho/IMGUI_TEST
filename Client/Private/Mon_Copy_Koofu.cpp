#include "stdafx.h"

#include "Mon_Copy_Koofu.h"
#include "Skill_Koofu_Fuit.h"

#include "Boss_Koofu.h"


CMon_Copy_Koofu::CMon_Copy_Koofu(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster{ pGraphic_Device }
{
}

CMon_Copy_Koofu::CMon_Copy_Koofu(const CMon_Copy_Koofu& Prototype)
	:CMonster{ Prototype }
{
}

HRESULT CMon_Copy_Koofu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMon_Copy_Koofu::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	MON_COPY_KOOFU_DESC* pDesc = static_cast<MON_COPY_KOOFU_DESC*>(pArg);

	m_pTargetTransform = pDesc->m_pTargetTransform;
	m_tMonsterDesc.iHp = pDesc->iHp;
	m_tMonsterDesc.iAttack = pDesc->iAttack;

	Safe_AddRef(m_pTargetTransform);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_eMon_State = MON_STATE::IDLE;
	m_eAnim_State = ANIM_STATE::IDLE;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(30, 0.75f, 20));
	 
	Warf(49.f, 37.f, 7.f);
	
	return S_OK;
}

void CMon_Copy_Koofu::Priority_Update(_float fTimeDelta)
{
	Move_Dir();
}

void CMon_Copy_Koofu::Update(_float fTimeDelta)
{
	Mon_State(fTimeDelta);
}

void CMon_Copy_Koofu::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	Destory();
}

HRESULT CMon_Copy_Koofu::Render(_float fTimeDelta)
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

void CMon_Copy_Koofu::Move_Dir()
{
	_float fAngle = m_pTransformCom->Target_Dir_Degree(m_pTargetTransform->Get_State(CTransform::STATE_POSITION));

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

void CMon_Copy_Koofu::Mon_State(_float fTimeDelta)
{
	switch (m_eMon_State)
	{
	case MON_STATE::IDLE:
		State_Idle(fTimeDelta);
		break;

	case MON_STATE::THROW:
		State_Throw(fTimeDelta);
		break;
	}
}

void CMon_Copy_Koofu::Anim_State(_float fTimeDelta)
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

	case ANIM_STATE::THROW:
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
	}
}

void CMon_Copy_Koofu::State_Idle(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::IDLE;

	if(m_pTimerCom->Time_Limit(fTimeDelta , 4.f))
		m_eMon_State = MON_STATE::THROW;
}

void CMon_Copy_Koofu::State_Throw(_float fTimeDelta)
{
	m_eAnim_State = ANIM_STATE::THROW;
	
	if (m_pTimerCom->Time_Limit(fTimeDelta, 1.f))
	{
		m_eMon_State = MON_STATE::IDLE;
		FuitCreate();
	}
}


HRESULT CMon_Copy_Koofu::Ready_Components()
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

	m_pTransformCom->Set_Scaled(_float3(1.5f, 1.5f, 1.f));

	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = m_pTransformCom->Get_Scaled().z;
	ColliderDesc.MineGameObject = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_MONSTER, this);

	return S_OK;
}

HRESULT CMon_Copy_Koofu::Ready_Animation()
{
	//Idle
	if(FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_Down"), TEXT("KOOFU_IDLE_DOWN"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_Left"), TEXT("KOOFU_IDLE_LEFT"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_LeftDown"), TEXT("KOOFU_IDLE_LEFTDOWN"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_LeftUp"), TEXT("KOOFU_IDLE_LEFTUP"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_Right"), TEXT("KOOFU_IDLE_RIGHT"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_RightDown"), TEXT("KOOFU_IDLE_RIGHTDOWN"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_RightUp"), TEXT("KOOFU_IDLE_RIGHTUP"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_Up"), TEXT("KOOFU_IDLE_UP"))))
		return E_FAIL;

	//Throw
	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_Down"), TEXT("KOOFU_THROW_DOWN"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_Left"), TEXT("KOOFU_THROW_LEFT"))))
		return E_FAIL;
	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_LeftDown"), TEXT("KOOFU_THROW_LEFTDOWN"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_LeftUp"), TEXT("KOOFU_THROW_LEFTUP"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_Right"), TEXT("KOOFU_THROW_RIGHT"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_RightDown"), TEXT("KOOFU_THROW_RIGHTDOWN"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_RightUp"), TEXT("KOOFU_THROW_RIGHTUP"))))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_Up"), TEXT("KOOFU_THROW_UP"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMon_Copy_Koofu::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CMon_Copy_Koofu::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CMon_Copy_Koofu::OnCollisionEnter(CCollider* other)
{
}

void CMon_Copy_Koofu::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CMon_Copy_Koofu::OnCollisionExit(CCollider* other)
{
}

void CMon_Copy_Koofu::Warf(_int iPosX, _int iPosZ, _float fDistance)
{
	_float WarfPosX = iPosX + fDistance * cos(rand() % 360 * (D3DX_PI / 180.f));
	_float WarfPosZ = iPosZ - fDistance * sin(rand() % 360 * (D3DX_PI / 180.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(WarfPosX, 0.75f, WarfPosZ));
}

void CMon_Copy_Koofu::Destory()
{
	CMon_Copy_Koofu* pThis = this;

	if (m_tMonsterDesc.iHp <= 0)
		Safe_Release(pThis);

	CBoss_Koofu* pOrigin = dynamic_cast<CBoss_Koofu*>(m_pGameInstance->Get_GameObject(LEVEL_KOOFU, TEXT("Layer_Boss_Koofu")));
	if (pOrigin->HitCheck())
	{
		Safe_Release(pThis);

	}
}

HRESULT CMon_Copy_Koofu::FuitCreate()
{
	CSkill_Koofu_Fuit::SKILL_FUIT_DESC Desc = {};

	Desc.iBulletCnt = 1;
	Desc.pTargetTransform = m_pTransformCom;
	Desc.pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_KOOFU, TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_Skill_Koofu_Fuit"), TEXT("Layer_Fuit"), &Desc)))
		return E_FAIL;

	return S_OK;
}

CMon_Copy_Koofu* CMon_Copy_Koofu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMon_Copy_Koofu* pInstance = new CMon_Copy_Koofu(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Copy_Koofu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMon_Copy_Koofu::Clone(void* pArg)
{
	CMon_Copy_Koofu* pInstance = new CMon_Copy_Koofu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{

		MSG_BOX(TEXT("Failed to Cloned : Copy_Koofu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMon_Copy_Koofu::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
