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

	// pArg가 FILEDATA 타입인지 확인
	FILEDATA* pFileData = reinterpret_cast<FILEDATA*>(pArg);
	if (pFileData != nullptr && !pFileData->isParsing) // isParsing 값을 확인
	{
		// 전달된 위치와 스케일 적용
		CComponent* component = Get_Component(TEXT("Com_Transform"));
		CTransform* transform = static_cast<CTransform*>(component);

		transform->Set_State(CTransform::STATE_POSITION, &pFileData->position);
		transform->Set_Scaled(pFileData->scale);

		return S_OK;
	}
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
	CTransform* transform = dynamic_cast<CTransform*>(component);

	_float3  cameraPosition = *(_float3*)&ViewMatrix.m[3][0];
	_float offsetZ = 5.f;

	cameraPosition.z += offsetZ;

	//스카이박스는 맨 앞에 -> 덮여야 한다.
	//UI는 맨 뒤에 -> 덮어야 한다.
	transform->Set_State(CTransform::STATE_POSITION, &cameraPosition);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUIObject::Render(_float fTimeDelta)
{
	return S_OK;
}

void CUIObject::Free()
{
	__super::Free();
}
