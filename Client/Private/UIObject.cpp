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
	// ���� ���� Ȱ��ȭ
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �ø� ��� ����
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CUIObject::End_RenderState()
{
	// ���� ���� ��Ȱ��ȭ
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// ������ �ø� ���� ����
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

	// pArg�� FILEDATA Ÿ������ Ȯ��
	FILEDATA* pFileData = reinterpret_cast<FILEDATA*>(pArg);
	if (pFileData != nullptr && !pFileData->isParsing) // isParsing ���� Ȯ��
	{
		// ���޵� ��ġ�� ������ ����
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

	//��ī�̹ڽ��� �� �տ� -> ������ �Ѵ�.
	//UI�� �� �ڿ� -> ����� �Ѵ�.
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
