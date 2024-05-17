#include "stdafx.h"

#include "Skill_Koofu_Rolling.h"

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

	if (FAILED(Ready_Components()))
		return E_FAIL;



	return S_OK;
}

void CSkill_Koofu_Rolling::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Koofu_Rolling::Update(_float fTimeDelta)
{
}

void CSkill_Koofu_Rolling::Late_Update(_float fTimeDelta)
{
}

HRESULT CSkill_Koofu_Rolling::Render(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CSkill_Koofu_Rolling::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_RollingIce"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
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
		MSG_BOX(TEXT("Failed to Created : Skill_Koofu_Rolling"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Koofu_Rolling::Free()
{
	__super::Free();
}
