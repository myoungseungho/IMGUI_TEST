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
	/* 네모를 구성하기위한 정점들을 생성한다. */
	m_iNumVertices = 8;
	m_iVertexStride = sizeof(VTXPOSTEX);
	// m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_DIFFUSE;
	m_PrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	m_iIndexStride = 2;
	m_iNumIndices = 36;
	m_eIndexFormat = D3DFMT_INDEX16;

	/* 정점배열에 대한 공간을 모두 생성했다. */
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	/* 공간에 값을 채우기위한 작업을 수행해야한다. */
	VTXPOSTEX* pVertices = { nullptr };

	/* void** <- void** */
	/* void*는 모든 주소를 다 담을 수 있는것이 맞지만, */

	/* void**는 오로지 void*형의 주소만 담을 수 있다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.f);
	pVertices[0].vColor = RED;

	pVertices[1].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[1].vTexcoord = _float2(0.f, 1.f);
	pVertices[1].vColor = RED;

	pVertices[2].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);
	pVertices[2].vColor = RED;

	pVertices[3].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[3].vTexcoord = _float2(1.0f, 0.f);
	pVertices[3].vColor = RED;

	pVertices[4].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[4].vTexcoord = _float2(0.0f, 0.f);
	pVertices[4].vColor = RED;

	pVertices[5].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[5].vTexcoord = _float2(0.f, 1.f);
	pVertices[5].vColor = RED;

	pVertices[6].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[6].vTexcoord = _float2(1.f, 1.f);
	pVertices[6].vColor = RED;

	pVertices[7].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[7].vTexcoord = _float2(1.0f, 0.f);
	pVertices[7].vColor = RED;


	m_pVB->Unlock();

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices
		, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	//아랫
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	//위
	pIndices[6] = 4;
	pIndices[7] = 6;
	pIndices[8] = 5;

	pIndices[9] = 4;
	pIndices[10] = 7;
	pIndices[11] = 6;

	//뒷
	pIndices[12] = 4;
	pIndices[13] = 5;
	pIndices[14] = 1;

	pIndices[15] = 4;
	pIndices[16] = 1;
	pIndices[17] = 0;

	//오른쪽
	pIndices[18] = 3;
	pIndices[19] = 2;
	pIndices[20] = 6;

	pIndices[21] = 3;
	pIndices[22] = 6;
	pIndices[23] = 7;

	//왼쪽
	pIndices[24] = 1;
	pIndices[25] = 5;
	pIndices[26] = 6;

	pIndices[27] = 1; 
	pIndices[28] = 6;
	pIndices[29] = 2;

	//앞면
	pIndices[30] = 4;
	pIndices[31] = 0;
	pIndices[32] = 3;

	pIndices[33] = 4;
	pIndices[34] = 3;
	pIndices[35] = 7;

	m_pIB->Unlock();


	return S_OK;
}

HRESULT CVIBuffer_Cube::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Cube::Render()
{
	//m_pGraphic_Device->
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
