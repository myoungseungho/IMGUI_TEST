#include "stdafx.h"

#include "Skill_Bug_Bullet.h"
#include "GameInstance.h"

CSkill_Bug_Bullet::CSkill_Bug_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill_Monster { pGraphic_Device }
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

	SKILL_BUG_BULLET_DESC* pDesc = static_cast<SKILL_BUG_BULLET_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;

	return S_OK;
}

void CSkill_Bug_Bullet::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Bug_Bullet::Update(_float fTimeDelta)
{
}

void CSkill_Bug_Bullet::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSkill_Bug_Bullet::Render()
{
	return S_OK;
}

HRESULT CSkill_Bug_Bullet::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
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

	return S_OK;
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

	Safe_Release(m_pTargetTransform);
	__super::Free();
}
