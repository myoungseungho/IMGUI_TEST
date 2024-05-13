#include "stdafx.h"

#include "Skill_Player.h"

CSkill_Player::CSkill_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CSkill{ pGraphic_Device }
{
}

CSkill_Player::CSkill_Player(const CSkill_Player& Prototype)
	: CSkill { Prototype }
{
}

HRESULT CSkill_Player::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CSkill_Player::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

HRESULT CSkill_Player::Ready_Components()
{
	return E_NOTIMPL;
}

CGameObject* CSkill_Player::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_Player::Free()
{
}
