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

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[3].vTexcoord = _float2(0.0f, 0.f);

	m_pVB->Unlock();




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
