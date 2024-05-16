#include "stdafx.h"

#include "Skill_Player.h"
#include "GameInstance.h"

CSkill_Player::CSkill_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill{ pGraphic_Device }
{
}

CSkill_Player::CSkill_Player(const CSkill_Player& Prototype)
	:CSkill{ Prototype }
{
}

HRESULT CSkill_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Player::Initialize(void* pArg)
{

	SKILL_PLAYER_DESC* pDesc = static_cast<SKILL_PLAYER_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_pTargetTransform->Get_State(CTransform::STATE_POSITION)));



	
	return S_OK;
}

void CSkill_Player::Priority_Update(_float fTimeDelta)
{

}

void CSkill_Player::Update(_float fTimeDelta)
{
	_float Targetx = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).x;
	_float Targety = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).y;
	_float Targetz = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).z;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(Targetx+10.f * fTimeDelta, Targety, Targetz + 10.f * fTimeDelta));
}

void CSkill_Player::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

}

HRESULT CSkill_Player::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	_float4x4		ViewMatrix, ProjMatrix;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CSkill_Player::OnCollisionEnter(CCollider* other)
{
	int a = 3;
}

void CSkill_Player::OnCollisionStay(CCollider* other)
{
	int a = 3;
}

void CSkill_Player::OnCollisionExit(CCollider* other)
{
	int a = 3;
}


HRESULT CSkill_Player::Ready_Components()
{
	/* For.Com_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Skill_Bug_Bullet"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Calc_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Calc_Timer"), reinterpret_cast<CComponent**>(&m_pCal_Timercom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{ 0.f, 0.f };
	//TransformDesc.fSpeedPerSec = 1.0f;
	//TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_pTargetTransform->Get_State(CTransform::STATE_POSITION));

	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 1.f;
	ColliderDesc.MineGameObject = this;

	//콜라이더 사본을 만들때 Cube 정보 추가해줘야 함.
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//콜라이더오브젝트 추가
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_PLAYER_SKILL, this);

	return S_OK;
}

CSkill_Player* CSkill_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkill_Player* pInstance = new CSkill_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Skill_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Player::Clone(void* pArg)
{
	CSkill_Player* pInstance = new CSkill_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Skill_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Player::Free()
{
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pColliderCom);
	__super::Free();
}
