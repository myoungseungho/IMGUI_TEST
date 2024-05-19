#pragma once

#include "VIBuffer.h"

/* 1. CVIBuffer_Rect + CVIBuffer �׸� �����ϱ����� ������ �����ϴ� �۾��� �Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
private:
	CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Rect(const CVIBuffer_Rect& Prototype);
	virtual ~CVIBuffer_Rect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	_bool Picked(class CTransform* pTransformCom, _float3* pPickedPos);

private:
	_uint				m_iNumVerticesX = { 0 };
	_uint				m_iNumVerticesY = { 0 };

public:
	static CVIBuffer_Rect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END