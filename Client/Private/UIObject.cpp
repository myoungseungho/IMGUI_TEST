#include "stdafx.h"
#include "..\Public\UIObject.h"

#include "GameInstance.h"

CUIObject::CUIObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CUIObject::CUIObject(const CUIObject& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CUIObject::Begin_RenderState()
{
	// 알파 블렌딩 활성화
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 컬링 모드 설정
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CUIObject::End_RenderState()
{
	// 알파 블렌딩 비활성화
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// 원래의 컬링 모드로 복원
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CUIObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIObject::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
}

void CUIObject::Update(_float fTimeDelta)
{

}

void CUIObject::Late_Update(_float fTimeDelta)
{
	_float4x4		ViewMatrix{};

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	CComponent* component = Get_Component(TEXT("Com_Transform"));
	CTransform* transform = static_cast<CTransform*>(component);

	_float3  cameraPosition = *(_float3*)&ViewMatrix.m[3][0];

	cameraPosition.z += offsetZ;

	transform->Set_State(CTransform::STATE_POSITION, &cameraPosition);

	BillBoarding();
}

HRESULT CUIObject::Render(_float fTimeDelta)
{
	return S_OK;
}

void CUIObject::BillBoarding()
{
	_float4x4      ViewMatrix{};

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	CComponent* component = Get_Component(TEXT("Com_Transform"));
	CTransform* transform = static_cast<CTransform*>(component);

	transform->Set_State(CTransform::STATE_RIGHT, (_float3*)&ViewMatrix.m[0][0]);
	transform->Set_State(CTransform::STATE_UP, (_float3*)&ViewMatrix.m[1][0]);
	transform->Set_State(CTransform::STATE_LOOK, (_float3*)&ViewMatrix.m[2][0]);
}


void CUIObject::Free()
{
	__super::Free();
}
