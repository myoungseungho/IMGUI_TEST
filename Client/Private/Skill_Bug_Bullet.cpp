#include "stdafx.h"

#include "Skill_Monster.h"
#include "Skill_Bug_Bullet.h"
#include "GameInstance.h"

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

	SKILL_MONSTER__DESC* pDesc = static_cast<SKILL_MONSTER__DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(0.5f, 0.5f, 0.5f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_pTargetTransform->Get_State(CTransform::STATE_POSITION));

	_float fDegree = (float)(rand() % 200 + 1900) / 10.f;
	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), (pDesc->iBulletCnt * 5 - fDegree) * D3DX_PI / 180.f);

	return S_OK;
}

void CSkill_Bug_Bullet::Priority_Update(_float fTimeDelta)
{

}

void CSkill_Bug_Bullet::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);
	CSkill_Bug_Bullet* bullet = this;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		Safe_Release(bullet);
}

void CSkill_Bug_Bullet::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
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

	return S_OK;
}

HRESULT CSkill_Bug_Bullet::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Skill_Bug_Bullet"), TEXT("SKILL_BUG_BULLET"));

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

void CSkill_Bug_Bullet::Distroy(_float fTimeDelta)
{
	CSkill_Bug_Bullet* pThis = this;

	if (m_pTimerCom->Time_Limit(fTimeDelta, 1.f))
		Safe_Release(pThis);
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


	__super::Free();
}
