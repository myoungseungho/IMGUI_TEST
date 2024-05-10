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
	m_iNumVertices = 24;
	m_iVertexStride = sizeof(VTXPOSTEX);
	// m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
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

	pVertices[1].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[1].vTexcoord = _float2(0.f, 1.f);

	pVertices[2].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[3].vTexcoord = _float2(1.0f, 0.f);

	pVertices[4].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[4].vTexcoord = _float2(0.0f, 0.f);

	pVertices[5].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[5].vTexcoord = _float2(0.f, 1.f);

	pVertices[6].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[6].vTexcoord = _float2(1.f, 1.f);

	pVertices[7].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[7].vTexcoord = _float2(1.0f, 0.f);

	pVertices[8].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[8].vTexcoord = _float2(0.0f, 0.f);

	pVertices[9].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[9].vTexcoord = _float2(0.f, 1.f);

	pVertices[10].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[10].vTexcoord = _float2(1.f, 1.f);

	pVertices[11].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[11].vTexcoord = _float2(1.0f, 0.f);

	pVertices[12].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[12].vTexcoord = _float2(0.0f, 0.f);

	pVertices[13].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[13].vTexcoord = _float2(0.f, 1.f);

	pVertices[14].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[14].vTexcoord = _float2(1.f, 1.f);

	pVertices[15].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[15].vTexcoord = _float2(1.0f, 0.f);

	pVertices[16].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[16].vTexcoord = _float2(0.0f, 0.f);

	pVertices[17].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[17].vTexcoord = _float2(0.f, 1.f);

	pVertices[18].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[18].vTexcoord = _float2(1.f, 1.f);

	pVertices[19].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[19].vTexcoord = _float2(1.0f, 0.f);

	pVertices[20].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[20].vTexcoord = _float2(0.0f, 0.f);

	pVertices[21].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[21].vTexcoord = _float2(0.f, 1.f);

	pVertices[22].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[22].vTexcoord = _float2(1.f, 1.f);

	pVertices[23].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[23].vTexcoord = _float2(1.0f, 0.f);

	m_pVB->Unlock();

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices
		, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	//아랫
	//pIndices[0] = 0;
	//pIndices[1] = 1;
	//pIndices[2] = 2;

	//pIndices[3] = 0;
	//pIndices[4] = 2;
	//pIndices[5] = 3;

	////위
	//pIndices[6] = 4;
	//pIndices[7] = 5;
	//pIndices[8] = 6;

	//pIndices[9] = 4;
	//pIndices[10] = 6;
	//pIndices[11] = 7;

	////뒷
	//pIndices[12] = 5;
	//pIndices[13] = 4;
	//pIndices[14] = 0;

	//pIndices[15] = 5;
	//pIndices[16] = 0;
	//pIndices[17] = 1;

	////오른쪽
	//pIndices[18] = 6;
	//pIndices[19] = 5;
	//pIndices[20] = 1;

	//pIndices[21] = 6;
	//pIndices[22] = 1;
	//pIndices[23] = 2;

	////왼쪽
	//pIndices[24] = 7;
	//pIndices[25] = 4;
	//pIndices[26] = 0;

	//pIndices[27] = 7;
	//pIndices[28] = 0;
	//pIndices[29] = 3;

	////앞면
	//pIndices[30] = 7;
	//pIndices[31] = 6;
	//pIndices[32] = 2;

	//pIndices[33] = 7;
	//pIndices[34] = 2;
	//pIndices[35] = 3;

	// fill in the front face index data
	pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 2;
	pIndices[3] = 0; pIndices[4] = 2; pIndices[5] = 3;

	// fill in the back face index data
	pIndices[6] = 4; pIndices[7] = 5; pIndices[8] = 6;
	pIndices[9] = 4; pIndices[10] = 6; pIndices[11] = 7;

	// fill in the top face index data
	pIndices[12] = 8; pIndices[13] = 9; pIndices[14] = 10;
	pIndices[15] = 8; pIndices[16] = 10; pIndices[17] = 11;

	// fill in the bottom face index data
	pIndices[18] = 12; pIndices[19] = 13; pIndices[20] = 14;
	pIndices[21] = 12; pIndices[22] = 14; pIndices[23] = 15;

	// fill in the left face index data
	pIndices[24] = 16; pIndices[25] = 17; pIndices[26] = 18;
	pIndices[27] = 16; pIndices[28] = 18; pIndices[29] = 19;
	
	pIndices[30] = 20; pIndices[31] = 21; pIndices[32] = 22;
	pIndices[33] = 20; pIndices[34] = 22; pIndices[35] = 23;

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
