#include "stdafx.h"
#include "..\Public\EnviormentObject.h"

#include "GameInstance.h"

CEnviormentObject::CEnviormentObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CEnviormentObject::CEnviormentObject(const CEnviormentObject & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CEnviormentObject::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	return S_OK;
}

HRESULT CEnviormentObject::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CEnviormentObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnviormentObject::Initialize(void * pArg)
{
	return S_OK;
}

void CEnviormentObject::Priority_Update(_float fTimeDelta)
{
}

void CEnviormentObject::Update(_float fTimeDelta)
{
}

void CEnviormentObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CEnviormentObject::Render(_float fTimeDelta)
{
	return S_OK;
}

void CEnviormentObject::Free()
{
	__super::Free();
}
