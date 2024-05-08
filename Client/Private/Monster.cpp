#include "stdafx.h"

#include "Monster.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 _pGraphicDevice)
	:CGameObject{ _pGraphicDevice }
{
}

CMonster::CMonster(const CMonster& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CMonster::Ready_Component()
{
	return S_OK;
}

void CMonster::Free()
{
	__super::Free();

}
