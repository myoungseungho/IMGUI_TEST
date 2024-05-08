#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCalc_Timer final : public CComponent
{
private:
	CCalc_Timer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCalc_Timer(const CCalc_Timer& Prototype);
	virtual ~CCalc_Timer() = default;

public:
	virtual HRESULT Initialize_Prototype(_float fTimeDelta);
	virtual HRESULT Initialize(void* pArg)	override;

public:
	_bool Time_Limit(_float fTimeDelta, _float fTimer)
	{
		if (fTimeDelta >= fTimer)
			return true;
	}

public:
	static CCalc_Timer* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fTimeDelta);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float PrevTimer = { 0.f };
	_float CurrTimer = { 0.f };

};

END