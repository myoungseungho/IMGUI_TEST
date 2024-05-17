#include "stdafx.h"

#include "Skill_Koofu_Fuit.h"

CSkill_Koofu_Fuit::CSkill_Koofu_Fuit(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill_Monster { pGraphic_Device }
{
}

CSkill_Koofu_Fuit::CSkill_Koofu_Fuit(const CSkill_Koofu_Fuit& Prototype)
	:CSkill_Monster{ Prototype }
{
}

HRESULT CSkill_Koofu_Fuit::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Koofu_Fuit::Initialize(void* pArg)
{
	return S_OK;
}

void CSkill_Koofu_Fuit::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Koofu_Fuit::Update(_float fTimeDelta)
{
}

void CSkill_Koofu_Fuit::Late_Update(_float fTimeDelta)
{
}

HRESULT CSkill_Koofu_Fuit::Render(_float fTimeDelta)
{
	return E_NOTIMPL;
}

HRESULT CSkill_Koofu_Fuit::Ready_Components()
{
	return E_NOTIMPL;
}

CSkill_Koofu_Fuit* CSkill_Koofu_Fuit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkill_Koofu_Fuit* pInstance = new CSkill_Koofu_Fuit(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Skill_Koofu_Fuit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Koofu_Fuit::Clone(void* pArg)
{
	CSkill_Koofu_Fuit* pInstance = new CSkill_Koofu_Fuit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Skill_Koofu_Fuit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Koofu_Fuit::Free()
{
}
