#pragma once

#include "VIBuffer.h"

/* 1. CVIBuffer_Cube + CVIBuffer 네모를 구성하기위한 정점을 생성하는 작업을 한다. */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cube final : public CVIBuffer
{
private:
	CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Cube(const CVIBuffer_Cube& Prototype);
	virtual ~CVIBuffer_Cube() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END