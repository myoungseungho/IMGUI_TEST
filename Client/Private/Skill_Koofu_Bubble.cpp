#include "stdafx.h"

#include "Skill_Koofu_Bubble.h"
#include "Effect_Monster.h"

CSkill_Koofu_Bubble::CSkill_Koofu_Bubble(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill_Monster{ pGraphic_Device }
{
}

CSkill_Koofu_Bubble::CSkill_Koofu_Bubble(const CSkill_Koofu_Bubble& pPrototype)
	:CSkill_Monster{ pPrototype }
{
}

HRESULT CSkill_Koofu_Bubble::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Koofu_Bubble::Initialize(void* pArg)
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

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), 90.f * D3DX_PI/ 180.f);

	return S_OK;
}

void CSkill_Koofu_Bubble::Priority_Update(_float fTimeDelta)
{
	m_fAngle += fTimeDelta * 2.f;

	if(m_fAngle > 360)
		m_fAngle = 0.f;
}

void CSkill_Koofu_Bubble::Update(_float fTimeDelta)
{
	Set_Scale(fTimeDelta);

	_float vPositionX = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).x;
	_float vPositionY = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).y;
	_float vPositionZ = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).z;

	if (m_tSkill_Desc.iBulletCnt % 2 != 0)
	{
		Move(vPositionX, vPositionZ, m_tSkill_Desc.iBulletCnt * 3.f, m_fAngle);
	}
	else 
		Move(vPositionX, vPositionZ, m_tSkill_Desc.iBulletCnt * 3.f, -m_fAngle);
}

void CSkill_Koofu_Bubble::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSkill_Koofu_Bubble::Render(_float fTimeDelta)
{

	if (FAILED(Begin_RenderState()))
		E_FAIL;

	if(FAILED(m_pTextureCom->Bind_Texture(0)))
		E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		E_FAIL;

	return S_OK;
}

HRESULT CSkill_Koofu_Bubble::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Bubble"),
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

HRESULT CSkill_Koofu_Bubble::Ready_Animation()
{
	return S_OK;
}

void CSkill_Koofu_Bubble::Set_Scale(_float fTimeDelta)
{
	m_fScaleAcc += m_fScalePlus;

	if (m_fScaleAcc > 3 || m_fScaleAcc < 0)
		m_fScalePlus *= -1;

	m_pTransformCom->Set_Scaled(_float3(1.f + m_fScaleAcc, 1.f + m_fScaleAcc, 1.f));
}

void CSkill_Koofu_Bubble::Move(_int iPosX, _int iPosZ, _float fDistance, _float fAngle)
{
	_float WarfPosX = iPosX + fDistance * cos((fAngle + D3DX_PI / 180.f));
	_float WarfPosZ = iPosZ - fDistance * sin((fAngle + D3DX_PI / 180.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(WarfPosX, 1.5f, WarfPosZ));

	//TransformDesc.fSpeedPerSec 
}

void CSkill_Koofu_Bubble::Destroy(_float fTimeDelta)
{
}

HRESULT CSkill_Koofu_Bubble::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;	
}

HRESULT CSkill_Koofu_Bubble::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CSkill_Koofu_Bubble::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CPlayer*>(otherObject))
	{
		CEffect_Monster::EFFECT_MONSTER__DESC	Desc = {};
		Desc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_KOOFU, TEXT("Layer_Player"), TEXT("Com_Transform")));

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_Clean"), TEXT("Layer_Effect_Clean"), &Desc);
	}
}

void CSkill_Koofu_Bubble::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CSkill_Koofu_Bubble::OnCollisionExit(CCollider* other)
{
}

CSkill_Koofu_Bubble* CSkill_Koofu_Bubble::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkill_Koofu_Bubble* pInstance = new CSkill_Koofu_Bubble(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Skill_Koofu_Bubble"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Koofu_Bubble::Clone(void* pArg)
{
	CSkill_Koofu_Bubble* pInstance = new CSkill_Koofu_Bubble(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Skill_Koofu_Bubble"));
		Safe_Release(pInstance);
	}

	return pInstance;  
}

void CSkill_Koofu_Bubble::Free()
{
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
