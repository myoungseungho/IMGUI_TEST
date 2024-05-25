#include "stdafx.h"

#include "Skill_Cannon_Ball.h"

CSkill_Cannon_Ball::CSkill_Cannon_Ball(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill_Monster{ pGraphic_Device }
{
}

CSkill_Cannon_Ball::CSkill_Cannon_Ball(const CSkill_Cannon_Ball& Prototype)
	:CSkill_Monster{ Prototype }
{
}

HRESULT CSkill_Cannon_Ball::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Cannon_Ball::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SKILL_CANNON_DESC* pDesc = static_cast<SKILL_CANNON_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	m_pPlayerTransform = pDesc->pPlayerTransform;

	Safe_AddRef(m_pTargetTransform);
	Safe_AddRef(m_pPlayerTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	//생성 위치
	_float vPositionX = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).x;
	_float vPositionY = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).y + 2.f;
	_float vPositionZ = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).z;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vPositionX, vPositionY, vPositionZ));

	return S_OK;
}

void CSkill_Cannon_Ball::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Cannon_Ball::Update(_float fTimeDelta)
{
	Move(fTimeDelta);
}

void CSkill_Cannon_Ball::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSkill_Cannon_Ball::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	if (FAILED(End_RenderState()))
		E_FAIL;

	return S_OK;
}

HRESULT CSkill_Cannon_Ball::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Sprite_Snow_Ball"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

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

HRESULT CSkill_Cannon_Ball::Ready_Animation()
{
	return S_OK;
}

HRESULT CSkill_Cannon_Ball::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSkill_Cannon_Ball::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CSkill_Cannon_Ball::OnCollisionEnter(CCollider* other)
{
}

void CSkill_Cannon_Ball::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CSkill_Cannon_Ball::OnCollisionExit(CCollider* other)
{
}

void CSkill_Cannon_Ball::Move(_float fTimeDelta)
{
	m_pTransformCom->Chase(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), fTimeDelta);
}

CSkill_Cannon_Ball* CSkill_Cannon_Ball::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkill_Cannon_Ball* pInstance = new CSkill_Cannon_Ball(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Skill_Cannon_Ball"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Cannon_Ball::Clone(void* pArg)
{
	CSkill_Cannon_Ball* pInstance = new CSkill_Cannon_Ball(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Skill_Cannon_Ball"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Cannon_Ball::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pPlayerTransform);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
