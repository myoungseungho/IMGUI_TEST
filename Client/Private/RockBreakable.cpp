#include "stdafx.h"
#include "..\Public\RockBreakable.h"

#include "GameInstance.h"
#include <Skill_Player.h>

CRockBreakable::CRockBreakable(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CRockBreakable::CRockBreakable(const CRockBreakable& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CRockBreakable::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CRockBreakable::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//if (m_bIsPasingObject)
	//{
	//	FILEDATA* fileData = static_cast<FILEDATA*>(pArg);
	//	m_pTransformCom->Set_Scaled(_float3(fileData->scale.x, fileData->scale.y, fileData->scale.z));
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(fileData->position.x, fileData->position.y, fileData->position.z));
	//}

	//플레이어 충돌 테스트용 포지션
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(35.f, 0.5f, 15.f));

	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 1.f;
	ColliderDesc.MineGameObject = this;

	//콜라이더 사본을 만들때 Cube 정보 추가해줘야 함.
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//콜라이더오브젝트 추가
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_STATIC, this);

	return S_OK;
}

void CRockBreakable::Priority_Update(_float fTimeDelta)
{
}

void CRockBreakable::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);


}

void CRockBreakable::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

}

HRESULT CRockBreakable::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	AnimState(fTimeDelta);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

void CRockBreakable::OnCollisionEnter(CCollider* other)
{
	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CSkill_Player*>(otherObject))
		m_eAnimState = ANIM_DIE;
		return;
}

void CRockBreakable::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CRockBreakable::OnCollisionExit(CCollider* other)
{

}

HRESULT CRockBreakable::Ready_Components()
{
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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(35.f, 0.5f, 15.f));

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CRockBreakable::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_RockBreakable_Idle"), TEXT("AnimTexture_RockBreakable_Idle"));
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_RockBreakable_Die"), TEXT("AnimTexture_RockBreakable_MDie"));

	return S_OK;
}

void CRockBreakable::AnimState(_float _fTimeDelta)
{
	switch (m_eAnimState)
	{
	case ANIMATION_STATE::ANIM_IDLE:
		m_pAnimCom->Play_Animator(TEXT("AnimTexture_RockBreakable_Idle"), 0.3f, _fTimeDelta, false);
	
		break;

	case ANIMATION_STATE::ANIM_DIE:
		if (m_pAnimCom->Play_Animator(TEXT("AnimTexture_RockBreakable_MDie"), 0.5f, _fTimeDelta, false))
		break;
	}
}

CRockBreakable* CRockBreakable::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRockBreakable* pInstance = new CRockBreakable(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CRockBreakable"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CRockBreakable::Clone(void* pArg)
{
	CRockBreakable* pInstance = new CRockBreakable(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CRockBreakable"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRockBreakable::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pAnimCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
