#pragma once

#include "Component.h"

/* VI = Vertex + Index */
/* Buffer = 메모리 */

/* 1. 특정 모델을 구성하기위한 클래스들의 부모클래스다. */
/* 2. 특정모델 == 사각형, 지형, 큐브 */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& Prototype);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

protected:
	LPDIRECT3DVERTEXBUFFER9				m_pVB = { nullptr };
	_uint								m_iNumVertices = { 0 };
	_uint								m_iVertexStride = { 0 };
	_ulong								m_dwFVF = { 0 };
	D3DPRIMITIVETYPE					m_PrimitiveType = {};
	_uint								m_iNumPrimitive = {};

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END