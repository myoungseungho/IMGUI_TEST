#include "stdafx.h"

#include "Skill_Bug_SludgeWave.h"

CSkill_Bug_SludgeWave::CSkill_Bug_SludgeWave(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill_Monster{ pGraphic_Device }
{
}

CSkill_Bug_SludgeWave::CSkill_Bug_SludgeWave(const CSkill_Bug_SludgeWave& pPrototype)
	:CSkill_Monster{ pPrototype }
{
}

HRESULT CSkill_Bug_SludgeWave::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Bug_SludgeWave::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SKILL_MONSTER__DESC* pDesc = static_cast<SKILL_MONSTER__DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	m_tSkill_Desc.iBulletCnt = pDesc->iBulletCnt;
	Safe_AddRef(m_pTargetTransform);


	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;
	
	Create_Position(39.5f, 36.f, 55.f);
	
	return S_OK;
}

void CSkill_Bug_SludgeWave::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Bug_SludgeWave::Update(_float fTimeDelta)
{
	Move(fTimeDelta);
	Destroy(fTimeDelta);
}

void CSkill_Bug_SludgeWave::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSkill_Bug_SludgeWave::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		E_FAIL;

	m_pAnimCom->Play_Animator(TEXT("SKILL_BUG_SLUDGEWAVE"), 1.f, fTimeDelta, true);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		E_FAIL;

	return S_OK;
}

HRESULT CSkill_Bug_SludgeWave::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		E_FAIL;

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 6.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(5.f, 5.f ,1.f));

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

HRESULT CSkill_Bug_SludgeWave::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_Texture_SludgeWave"), TEXT("SKILL_BUG_SLUDGEWAVE"));

	return S_OK;
}

void CSkill_Bug_SludgeWave::Create_Position(_float fPosX, _float fPosZ, _float fDistance)
{
	_float WarfPosX = fPosX + fDistance * cos(rand() % 360 * (D3DX_PI / 180.f));
	_float WarfPosZ = fPosZ + fDistance * sin(rand() % 360 * (D3DX_PI / 180.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(WarfPosX, 0.75f, WarfPosZ));
}

void CSkill_Bug_SludgeWave::Move(_float fTimeDelta)
{
	m_pTransformCom->LookAt(_float3(39.5f, 0.5f, 36.f));
	m_pTransformCom->Go_Straight(fTimeDelta);
}

void CSkill_Bug_SludgeWave::Destroy(_float fTimeDelta)
{
	CSkill_Bug_SludgeWave* pThis = this;

	if (m_pTimerCom->Time_Limit(fTimeDelta, 4.5f))
		Safe_Release(pThis);
}

HRESULT CSkill_Bug_SludgeWave::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSkill_Bug_SludgeWave::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CSkill_Bug_SludgeWave* CSkill_Bug_SludgeWave::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkill_Bug_SludgeWave* pInstance = new CSkill_Bug_SludgeWave(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Cloned : Skill_Koofu_Bubble"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Bug_SludgeWave::Clone(void* pArg)
{
	CSkill_Bug_SludgeWave* pInstance = new CSkill_Bug_SludgeWave(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Skill_Koofu_Bubble"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Bug_SludgeWave::Free()
{
	Safe_Release(m_pAnimCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
