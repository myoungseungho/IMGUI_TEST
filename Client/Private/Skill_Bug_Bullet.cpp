#include "stdafx.h"

#include "Skill_Monster.h"
#include "Skill_Bug_Bullet.h"
#include "GameInstance.h"
#include "Player.h"

#include "Effect_Monster.h"

CSkill_Bug_Bullet::CSkill_Bug_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill_Monster{ pGraphic_Device }
{
}

CSkill_Bug_Bullet::CSkill_Bug_Bullet(const CSkill_Bug_Bullet& Prototype)
	:CSkill_Monster{ Prototype }
{
}

HRESULT CSkill_Bug_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Bug_Bullet::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BULLET_SKILL_DESC* pDesc = static_cast<BULLET_SKILL_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	m_iBulletCnt = pDesc->iBulletCnt;
	m_iTotalBullet = pDesc->iTotalBullet;
	m_iBulletType = pDesc->iBulletType;

	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_vCreatePos.x = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).x;
	m_vCreatePos.y = 0.5f;
	m_vCreatePos.z = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).z - 1.5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_vCreatePos);

	Bullet_State();

	return S_OK;
}

void CSkill_Bug_Bullet::Priority_Update(_float fTimeDelta)
{

}

void CSkill_Bug_Bullet::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_Backward(fTimeDelta);
}

void CSkill_Bug_Bullet::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	Destroy(fTimeDelta);
}

HRESULT CSkill_Bug_Bullet::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Play_Animator(TEXT("SKILL_BUG_BULLET"), 0.5f, fTimeDelta, true)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Bug_Bullet::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(0.5f, 0.5f, 0.5f));

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

HRESULT CSkill_Bug_Bullet::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_Texture_Skill_Bug_Bullet"), TEXT("SKILL_BUG_BULLET"));

	return S_OK;
}

HRESULT CSkill_Bug_Bullet::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSkill_Bug_Bullet::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CSkill_Bug_Bullet::OnCollisionEnter(class CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;
	CSkill_Bug_Bullet* pThis = this;

	CPlayer* player = static_cast<CPlayer*>(otherObject);

	CEffect_Monster::EFFECT_MONSTER__DESC  Desc = {};
	Desc.pTargetTransform = m_pTransformCom;

	if (player != nullptr)
	{
		if (player->Get_Player_CurState() != CPlayer::STATE_ATTACK)
			m_bPlayerAttack = true;

	}

	if(player->Get_Player_CurState() != CPlayer::STATE_ATTACK)
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Effect_Bug_Bullet_Destroy"), TEXT("Layer_Effect_Bullet_Destroy"), &Desc);

}

void CSkill_Bug_Bullet::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}


void CSkill_Bug_Bullet::OnCollisionExit(class CCollider* other)
{

}

void CSkill_Bug_Bullet::Destroy(_float fTimeDelta)
{
	CSkill_Bug_Bullet* pThis = this;

	if (m_pTimerCom->Time_Limit(fTimeDelta, 10.f))
		Safe_Release(pThis);

	if (m_bPlayerAttack)
		Safe_Release(pThis);
}

void CSkill_Bug_Bullet::Bullet_State()
{
	switch (m_iBulletType)
	{
	case BULLET_STATE::NORMAL:
		BulletType_Normal();
		break;

	case BULLET_STATE::CIRCLE:
		BulletType_Circle();
		break;
	}
}

void CSkill_Bug_Bullet::BulletType_Normal()
{
	m_fAngleRange = 90.f;
	_float fStartRange = m_fAngleRange * 0.5f;
	_float fAngle_Per_Piece = m_fAngleRange / m_iTotalBullet;

	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), (m_pTargetTransform->Dir_Degree() - fStartRange + fAngle_Per_Piece * m_iBulletCnt) * D3DX_PI / 180.f);
}

void CSkill_Bug_Bullet::BulletType_Circle()
{
	m_fAngleRange = 360.f;
	_float fStartRange = m_fAngleRange * 0.5f;
	_float fAngle_Per_Piece = m_fAngleRange / m_iTotalBullet;

	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), (m_pTargetTransform->Dir_Degree() - fStartRange + fAngle_Per_Piece * m_iBulletCnt) * D3DX_PI / 180.f);

}

CSkill_Bug_Bullet* CSkill_Bug_Bullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkill_Bug_Bullet* pInstance = new CSkill_Bug_Bullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Skill_Bug_Bullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Bug_Bullet::Clone(void* pArg)
{
	CSkill_Bug_Bullet* pInstance = new CSkill_Bug_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Skill_Bug_Bullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Bug_Bullet::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
