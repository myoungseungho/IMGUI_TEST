#include "..\Public\VIBuffer_Rect.h"

#include "GameInstance.h"
#include "Transform.h"

CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect& Prototype)
	: CVIBuffer{ Prototype }
	, m_iNumVerticesX{ Prototype.m_iNumVerticesX }
	, m_iNumVerticesY{ Prototype.m_iNumVerticesY }
{
}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
	/* �׸� �����ϱ����� �������� �����Ѵ�. */
	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXPOSTEX);
	// m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_PrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;

	m_iIndexStride = 2;
	m_iNumIndices = 6;
	m_eIndexFormat = D3DFMT_INDEX16;

	m_iNumVerticesX = 2;
	m_iNumVerticesY = 2;

	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	/* �����迭�� ���� ������ ��� �����ߴ�. */
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	/* ������ ���� ä������� �۾��� �����ؾ��Ѵ�. */
	VTXPOSTEX* pVertices = { nullptr };

	/* void** <- void** */
	/* void*�� ��� �ּҸ� �� ���� �� �ִ°��� ������, */
	/* void**�� ������ void*���� �ּҸ� ���� �� �ִ�. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.f);
	m_pVerticesPos[0] = _float3(-0.5f, 0.5f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);
	m_pVerticesPos[1] = _float3(0.5f, 0.5f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);
	m_pVerticesPos[2] = _float3(0.5f, -0.5f, 0.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);
	m_pVerticesPos[3] = _float3(-0.5f, -0.5f, 0.f);

	m_pVB->Unlock();

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices
		, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize(void* pArg)
{
	return S_OK;
}

_bool CVIBuffer_Rect::Picked(CTransform* pTransformCom, _float3* pPickedPos)
{
	/* ��ŷ�� ���� ������ ���÷� ��ȯ�Ѵ�. */
	//���彺���̽� ������ �����ϴ� RayPos, RayDir�� EnviormentObject�� �������� ���ִ� ���ý����̽� �������� ��ġ��Ų��.
	m_pGameInstance->Transform_ForPicking_ToLocalSpace(&pTransformCom->Get_WorldMatrix());

	for (size_t i = 0; i < m_iNumVerticesY - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			//129���� 129���� ������ �̷�� �簢�� �� �ϳ��� �������� ����� �ε����� ���� 3���� �Ű������� �ִ´�.

			if (true == m_pGameInstance->Picked_InLocalSpace(
				&m_pVerticesPos[iIndices[0]],
				&m_pVerticesPos[iIndices[1]],
				&m_pVerticesPos[iIndices[2]],
				pPickedPos))
			{
				//�ͷ����� ���ý����̽����� ���� Pos�� �ٽ� ��������� ���� ���彺���̽��� ��ȯ�ϴ� ����
				D3DXVec3TransformCoord(pPickedPos, pPickedPos, &pTransformCom->Get_WorldMatrix());
				return true;
			}

			if (true == m_pGameInstance->Picked_InLocalSpace(
				&m_pVerticesPos[iIndices[0]],
				&m_pVerticesPos[iIndices[2]],
				&m_pVerticesPos[iIndices[3]],
				pPickedPos))
			{
				D3DXVec3TransformCoord(pPickedPos, pPickedPos, &pTransformCom->Get_WorldMatrix());
				return true;
			}
		}
	}


	return false;
}

CVIBuffer_Rect* CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CVIBuffer_Rect::Clone(void* pArg)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();

}
