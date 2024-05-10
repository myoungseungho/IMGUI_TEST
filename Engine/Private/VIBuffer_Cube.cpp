#include "..\Public\VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& Prototype)
	: CVIBuffer{ Prototype }
{
}

HRESULT CVIBuffer_Cube::Initialize_Prototype()
{
	/* 큐브를 구성하기위한 정점들을 생성한다. */
	m_iNumVertices = 36;
	m_iVertexStride = sizeof(VTXPOSTEX);
	// m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_PrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	/* 정점배열에 대한 공간을 모두 생성했다. */
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	/* 공간에 값을 채우기위한 작업을 수행해야한다. */
	VTXPOSTEX*		pVertices = { nullptr };

	/* void** <- void** */
	/* void*는 모든 주소를 다 담을 수 있는것이 맞지만, */
	/* void**는 오로지 void*형의 주소만 담을 수 있다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
	// 앞면
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[3].vTexcoord = _float2(0.0f, 0.f);

	pVertices[4].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[4].vTexcoord = _float2(1.f, 1.f);

	pVertices[5].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[5].vTexcoord = _float2(0.f, 1.f);
	// 윗면
	pVertices[6].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[6].vTexcoord = _float2(0.f, 0.f);

	pVertices[7].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[7].vTexcoord = _float2(1.f, 0.f);

	pVertices[8].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[8].vTexcoord = _float2(1.f, 1.f);

	pVertices[9].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[9].vTexcoord = _float2(0.f, 0.f);

	pVertices[10].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[10].vTexcoord = _float2(1.f, 1.f);

	pVertices[11].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[11].vTexcoord = _float2(0.f, 1.f);
	//뒷면
	pVertices[12].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[12].vTexcoord = _float2(0.f, 0.f);

	pVertices[13].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[13].vTexcoord = _float2(1.f, 0.f);

	pVertices[14].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[14].vTexcoord = _float2(1.f, 1.f);

	pVertices[15].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[15].vTexcoord = _float2(0.f, 0.f);

	pVertices[16].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[16].vTexcoord = _float2(1.f, 1.f);

	pVertices[17].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[17].vTexcoord = _float2(0.f, 1.f);
	//아래
	pVertices[18].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[18].vTexcoord = _float2(0.f, 1.f);

	pVertices[19].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[19].vTexcoord = _float2(0.f, 1.f);

	pVertices[20].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[20].vTexcoord = _float2(0.f, 1.f);

	pVertices[21].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[21].vTexcoord = _float2(0.f, 1.f);

	pVertices[22].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[22].vTexcoord = _float2(0.f, 1.f);

	pVertices[23].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[23].vTexcoord = _float2(0.f, 1.f);

	pVertices[24].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[24].vTexcoord = _float2(0.f, 1.f);

	pVertices[25].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[25].vTexcoord = _float2(0.f, 1.f);

	pVertices[26].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[26].vTexcoord = _float2(0.f, 1.f);

	pVertices[27].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[27].vTexcoord = _float2(0.f, 1.f);

	pVertices[28].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[28].vTexcoord = _float2(0.f, 1.f);

	pVertices[29].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[29].vTexcoord = _float2(0.f, 1.f);

	pVertices[30].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[30].vTexcoord = _float2(0.f, 1.f);

	pVertices[31].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[31].vTexcoord = _float2(0.f, 1.f);

	pVertices[32].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[32].vTexcoord = _float2(0.f, 1.f);

	pVertices[33].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[33].vTexcoord = _float2(0.f, 1.f);

	pVertices[34].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[34].vTexcoord = _float2(0.f, 1.f);

	pVertices[35].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[35].vTexcoord = _float2(0.f, 1.f);

	pVertices[36].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[36].vTexcoord = _float2(0.f, 1.f);



	m_pVB->Unlock();
	
	
	return S_OK;
}

HRESULT CVIBuffer_Cube::Initialize(void * pArg)
{
	return S_OK;
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube*		pInstance = new CVIBuffer_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CVIBuffer_Cube::Clone(void * pArg)
{
	CVIBuffer_Cube*		pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();

}
