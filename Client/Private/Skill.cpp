#include "stdafx.h"

#include "Skill.h"

CSkill::CSkill(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject { pGraphic_Device }
{
}

CSkill::CSkill(const CSkill& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CSkill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill::Initialize(void* pArg)
{
	return S_OK;
}

void CSkill::Free()
{
	__super::Free();
}
