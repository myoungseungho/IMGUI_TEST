#include "stdafx.h"

#include "Skill_Koofu_Rolling.h"
#include "Player.h"

CSkill_Koofu_Rolling::CSkill_Koofu_Rolling(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill_Monster { pGraphic_Device }
{
}

CSkill_Koofu_Rolling::CSkill_Koofu_Rolling(const CSkill_Koofu_Rolling& pPrototype)
	:CSkill_Monster{ pPrototype }
{
}

HRESULT CSkill_Koofu_Rolling::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Koofu_Rolling::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SKILL_MONSTER__DESC* pDesc = static_cast<SKILL_MONSTER__DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	_float vPositionX = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).x + (pDesc->iBulletCnt * 3)- 9.f; 
	_float vPositionY = 1.5f;
	_float vPositionZ = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).z + (1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3 (vPositionX , vPositionY , vPositionZ));

	m_pGameInstance->Play_Sound(L"SFX_MonsterBugRolling_Rolling", LEVEL_STATIC, false);


	return S_OK;
}

void CSkill_Koofu_Rolling::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Koofu_Rolling::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_Backward(fTimeDelta);
	Destroy(fTimeDelta);
}

void CSkill_Koofu_Rolling::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

}

HRESULT CSkill_Koofu_Rolling::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		E_FAIL;

	m_pAnimCom->Play_Animator(TEXT("SKILL_ROLLING"), 0.5f, fTimeDelta, true);

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
		

	if (FAILED(End_RenderState()))
		E_FAIL;

	return S_OK;
}

HRESULT CSkill_Koofu_Rolling::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 10.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(3.f, 3.f, 1.f));

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

HRESULT CSkill_Koofu_Rolling::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_RollingIce"), TEXT("SKILL_ROLLING"));

	return S_OK;
}

void CSkill_Koofu_Rolling::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{

	/*CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CPlayer*>(otherObject))
	{
		CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));

		pPlayerTransform->Set_State(CTransform::STATE_POSITION, &_float3(10.f, 0.5f , 10.f));
		
	}*/
	
}

void CSkill_Koofu_Rolling::OnCollisionStay(CCollider* other)
{
}

void CSkill_Koofu_Rolling::OnCollisionExit(class CCollider* other)
{
}

void CSkill_Koofu_Rolling::Destroy(_float fTimeDelta)
{
	CSkill_Koofu_Rolling* pRolling = this;

	if (m_pTimerCom->Time_Limit(fTimeDelta  ,1.75f))
	{
		Safe_Release(pRolling);
	}
}

HRESULT CSkill_Koofu_Rolling::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSkill_Koofu_Rolling::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CSkill_Koofu_Rolling* CSkill_Koofu_Rolling::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkill_Koofu_Rolling* pInstance = new CSkill_Koofu_Rolling(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Skill_Koofu_Rolling"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Koofu_Rolling::Clone(void* pArg)
{
	CSkill_Koofu_Rolling* pInstance = new CSkill_Koofu_Rolling(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Skill_Koofu_Rolling"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Koofu_Rolling::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pAnimCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
