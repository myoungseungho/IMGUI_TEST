#include "..\Public\BlendObject.h"
#include "Transform.h"

CBlendObject::CBlendObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CBlendObject::CBlendObject(const CBlendObject & Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CBlendObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlendObject::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CBlendObject::Priority_Update(_float fTimeDelta)
{
}

void CBlendObject::Update(_float fTimeDelta)
{
}

void CBlendObject::Late_Update(_float fTimeDelta)
{
	CTransform*		pTransform = dynamic_cast<CTransform*>(Get_Component(TEXT("Com_Transform")));

	_float3			vWorldPos = pTransform->Get_State(CTransform::STATE_POSITION);

	_float4x4			ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXVec3TransformCoord(&vWorldPos, &vWorldPos, &ViewMatrix);

	m_fViewSpaceZ = vWorldPos.z;
}

HRESULT CBlendObject::Render()
{
	return S_OK;
}

HRESULT CBlendObject::Begin_Blend_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	return S_OK;
}

HRESULT CBlendObject::End_Blend_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	return S_OK;
}

void CBlendObject::Free()
{
	__super::Free();
}
