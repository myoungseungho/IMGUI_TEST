#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollision final:public CComponent
{
private:
	CCollision(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollision(const CCollision& Prototype);
	virtual ~CCollision() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:

private:

public:
	static CCollision* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END