#include "stdafx.h"
#include "..\Public\EnviormentObject.h"

#include "GameInstance.h"

CEnviormentObject::CEnviormentObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CEnviormentObject::CEnviormentObject(const CEnviormentObject& rhs)
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

HRESULT CEnviormentObject::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (pArg == nullptr)
	{
		//ī�޶� Ʈ������
		CComponent* camera_componenet = m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Com_Transform"));
		CTransform* camera_transform = static_cast<CTransform*>(camera_componenet);

		// ī�޶��� ���� ��ġ ��������
		_float3 cameraPosition = camera_transform->Get_State(CTransform::STATE_POSITION);

		// ���ο� ��ġ ��� (Z ���� ī�޶� ��ġ���� 5��ŭ ��������, Y�� 0���� ����)
		float zOffset = 5.0f;
		_float3 newPosition = cameraPosition;
		newPosition.z += zOffset;
		newPosition.y = 0.006f;

		CComponent* componenet = Get_Component(TEXT("Com_Transform"));
		CTransform* transform = static_cast<CTransform*>(componenet);

		// ������Ʈ�� ��ġ ����
		transform->Set_State(CTransform::STATE_POSITION, &newPosition);

		return S_OK;
	}
	else
	{
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
}

void CEnviormentObject::Priority_Update(_float fTimeDelta)
{
}

void CEnviormentObject::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		_float3		vPickedPos{};
		CComponent* component_Rect = Get_Component(TEXT("Com_VIBuffer"));
		CVIBuffer_Rect* bufferRectCom = static_cast<CVIBuffer_Rect*>(component_Rect);

		CComponent* componenet_Transform = Get_Component(TEXT("Com_Transform"));
		CTransform* transform = static_cast<CTransform*>(componenet_Transform);

		_bool isPicked = bufferRectCom->Picked(transform, &vPickedPos);

		if (isPicked)
			m_bIsPicking = true;
	}
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
