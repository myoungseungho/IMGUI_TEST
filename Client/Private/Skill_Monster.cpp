#include "stdafx.h"

#include "Skill_Monster.h"

CSkill_Monster::CSkill_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill{ pGraphic_Device }
{
}

CSkill_Monster::CSkill_Monster(const CSkill_Monster& Prototype)
	:CSkill{ Prototype }
{
}

HRESULT CSkill_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Monster::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CSkill_Monster::Ready_Components()
{
	return E_NOTIMPL;
}

void CSkill_Monster::Free()
{
	__super::Free();
}
