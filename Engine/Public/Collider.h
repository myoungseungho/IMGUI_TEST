#pragma once

#include "Component.h""
#include "VIBuffer.h"
#include "VIBuffer_Cube.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final :public CVIBuffer
{
private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render();

private:
	void Begin_Render();
	void End_Render();

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END

