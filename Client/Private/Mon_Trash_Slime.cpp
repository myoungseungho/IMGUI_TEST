#include "stdafx.h"

#include "GameInstance.h"

#include "Mon_Trash_Slime.h"

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

	m_tMonsterDesc.iHp = pDesc->iHp;
	m_tMonsterDesc.iAttack = pDesc->iAttack;
	m_pTargetTransform = pDesc->pTargetTransform;

	Safe_AddRef(m_pTargetTransform);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	return S_OK;
}

void CMon_Trash_Slime::Priority_Update(_float fTimeDelta)
{
}

void CMon_Trash_Slime::Update(_float fTimeDelta)
{
}

void CMon_Trash_Slime::Late_Update(_float fTimeDelta)
{
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
	return S_OK;
}

HRESULT CMon_Trash_Slime::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CMon_Trash_Slime::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CMon_Trash_Slime::Anim_State(_float fTimeDelta)
{

}

void CMon_Trash_Slime::OnCollisionEnter(CCollider* other)
{
}

void CMon_Trash_Slime::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CMon_Trash_Slime::OnCollisionExit(CCollider* other)
{
}

void CMon_Trash_Slime::Move_Dir(_float fTimeDelta)
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

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
