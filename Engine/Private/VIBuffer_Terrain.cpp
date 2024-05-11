#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype)
	: CVIBuffer{ Prototype }
	, m_iNumVerticesX{ Prototype.m_iNumVerticesX }
	, m_iNumVerticesZ{ Prototype.m_iNumVerticesZ }
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;

	/* 네모를 구성하기위한 정점들을 생성한다. */
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	/* 네모를 구성하기위한 정점들을 생성한다. */
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iVertexStride = sizeof(VTXPOSTEX);
	// m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_PrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;

	m_iIndexStride = 4;
	m_iNumIndices = m_iNumPrimitive * 3;
	m_eIndexFormat = D3DFMT_INDEX32;

	/* 정점배열에 대한 공간을 모두 생성했다. */
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	/* 공간에 값을 채우기위한 작업을 수행해야한다. */
	VTXPOSTEX* pVertices = { nullptr };

	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	/* void** <- void** */
	/* void*는 모든 주소를 다 담을 수 있는것이 맞지만, */
	/* void**는 오로지 void*형의 주소만 담을 수 있다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < iNumVerticesX; j++)
		{
			_uint		iIndex = i * iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3(j, 0.0f, i);
			pVertices[iIndex].vTexcoord = _float2(j / (iNumVerticesX - 1.f) * 50.f, i / (iNumVerticesZ - 1.f) * 50.f);
		}
	}

	m_pVB->Unlock();

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices
		, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	_uint* pIndices = { nullptr };

	_uint			iNumIndices = { 0 };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (size_t i = 0; i < iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + iNumVerticesX,
				iIndex + iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

		}
	}


	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{
	_ulong		dwByte = { 0 };
	HANDLE		hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER			fh;
	BITMAPINFOHEADER			ih;
	_uint* pPixel = { nullptr };

	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	pPixel = new _uint[ih.biWidth * ih.biHeight];

	ReadFile(hFile, pPixel, sizeof(_uint) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	/* 네모를 구성하기위한 정점들을 생성한다. */
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iVertexStride = sizeof(VTXPOSTEX);
	// m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_PrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	m_iIndexStride = 4;
	m_iNumIndices = m_iNumPrimitive * 3;
	m_eIndexFormat = D3DFMT_INDEX32;

	/* 정점배열에 대한 공간을 모두 생성했다. */
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	/* 공간에 값을 채우기위한 작업을 수행해야한다. */
	VTXPOSTEX* pVertices = { nullptr };

	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	/* void** <- void** */
	/* void*는 모든 주소를 다 담을 수 있는것이 맞지만, */
	/* void**는 오로지 void*형의 주소만 담을 수 있다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	//	11111111 10111011 10111011 10111011
	//
	//&	00000000 00000000 00000000 11111111
	//
	//	00000000 00000000 00000000 10111011

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3(j, (pPixel[iIndex] & 0x000000ff) / 10.f, i);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f) * 50.f, i / (m_iNumVerticesZ - 1.f) * 50.f);
		}
	}

	m_pVB->Unlock();

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices
		, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	_uint* pIndices = { nullptr };

	_uint			iNumIndices = { 0 };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
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

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

		}
	}

	m_pIB->Unlock();

	Safe_Delete_Array(pPixel);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

_float CVIBuffer_Terrain::Compute_Height(const _float3& vPosition)
{
	/* 1. 어떤 삼각형안에 있는지를 구한다. */
	/* 1-1. 어떤 네모안에 있는지?! */
	_uint	 iIndex = _uint(vPosition.z) * m_iNumVerticesX + _uint(vPosition.x);

	_uint	iIndices[4] = {
		iIndex + m_iNumVerticesX,
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,
		iIndex
	};

	/* 1-2. 어떤 삼각형안에 있는지?! */
	_float		fWidth = vPosition.x - m_pVerticesPos[iIndices[0]].x;
	_float		fDepth = m_pVerticesPos[iIndices[0]].z - vPosition.z;

	/* 2. 그 삼각형의 평면의 방정식을 구한다. */
	D3DXPLANE		Plane{};

	/* 오른쪽 위 삼각형 안에 있었어. */
	if (fWidth > fDepth)
	{
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]]);
	}
	/* 왼쪽 아래 삼각형 안에 있었어. */
	else
	{
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[2]], &m_pVerticesPos[iIndices[3]]);
	}

	/* 3. 평면상에 플레이어를 위치시키기위해 xz는 고정하고 y를 새롭게 구해얗나다.*/
	return (-Plane.a * vPosition.x - Plane.c * vPosition.z - Plane.d) / Plane.b;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMapFilePath)
{

	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath)))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

}
