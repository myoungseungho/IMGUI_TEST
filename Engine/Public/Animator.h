#pragma once

#include "Component.h"

class ENGINE_DLL CAnimator final:public CComponent
{
private:
	CAnimator(LPDIRECT3DDEVICE9 pGrpahic_Device);
	CAnimator(const CAnimator& pPrototype);
	virtual ~CAnimator() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CAnimator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

