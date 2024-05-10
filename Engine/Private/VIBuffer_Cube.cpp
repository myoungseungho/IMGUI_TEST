#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& Prototype)
	:CVIBuffer{ Prototype }
{
}

HRESULT CVIBuffer_Cube::Initialize_Prototype()
{
	/* �׸� �����ϱ����� �������� �����Ѵ�. */
	m_iNumVertices = 8;
	m_iVertexStride = sizeof(VTXPOSTEX);
	// m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_PrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	/* �����迭�� ���� ������ ��� �����ߴ�. */
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	/* ������ ���� ä������� �۾��� �����ؾ��Ѵ�. */
	VTXPOSTEX* pVertices = { nullptr };

	/* void** <- void** */
	/* void*�� ��� �ּҸ� �� ���� �� �ִ°��� ������, */
	/* void**�� ������ void*���� �ּҸ� ���� �� �ִ�. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexcoord = _float2(0.0f, 0.f);

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexcoord = _float2(0.0f, 0.f);

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexcoord = _float2(1.f, 0.f);

	pVertices[6].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[6].vTexcoord = _float2(1.f, 1.f);

	pVertices[7].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[7].vTexcoord = _float2(0.0f, 0.f);

	m_pVB->Unlock();

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices
		, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	//�Ʒ�
	pIndices[0] = 1;
	pIndices[1] = 0;
	pIndices[2] = 3;

	pIndices[3] = 1;
	pIndices[4] = 3;
	pIndices[5] = 2;

	//��
	pIndices[6] = 4;
	pIndices[7] = 5;
	pIndices[8] = 6;

	pIndices[9] = 4;
	pIndices[10] = 6;
	pIndices[11] = 7;

	//��
	pIndices[12] = 5;
	pIndices[13] = 4;
	pIndices[14] = 0;

	pIndices[15] = 5;
	pIndices[16] = 0;
	pIndices[17] = 1;

	//������
	pIndices[18] = 6;
	pIndices[19] = 5;
	pIndices[20] = 1;

	pIndices[21] = 6;
	pIndices[22] = 1;
	pIndices[23] = 2;

	//����
	pIndices[24] = 7;
	pIndices[25] = 4;
	pIndices[26] = 0;

	pIndices[27] = 7;
	pIndices[28] = 0;
	pIndices[29] = 3;

	//�ո�
	pIndices[30] = 7;
	pIndices[31] = 6;
	pIndices[32] = 2;

	pIndices[33] = 7;
	pIndices[34] = 2;
	pIndices[35] = 3;

	m_pIB->Unlock();


	return S_OK;
}

HRESULT CVIBuffer_Cube::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube* pInstance  = new CVIBuffer_Cube(pGraphic_Device);
	
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Cube::Clone(void* pArg)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}
