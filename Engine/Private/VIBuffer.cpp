#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent { pGraphic_Device }
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & Prototype)
	: CComponent{ Prototype }
	, m_pVB { Prototype.m_pVB }
	, m_iNumVertices{ Prototype.m_iNumVertices }
	, m_iVertexStride { Prototype.m_iVertexStride } 
	, m_dwFVF{ Prototype.m_dwFVF }
	, m_PrimitiveType{ Prototype.m_PrimitiveType }
	, m_iNumPrimitive{ Prototype.m_iNumPrimitive }
{
	Safe_AddRef(m_pVB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride);
	m_pGraphic_Device->SetFVF(m_dwFVF);

	m_pGraphic_Device->DrawPrimitive(m_PrimitiveType, 0, m_iNumPrimitive);

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
}
