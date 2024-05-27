#include "stdafx.h"

#include "Effect.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CEffect::CEffect(const CEffect& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}


void CEffect::Free()
{
	__super::Free();
}
