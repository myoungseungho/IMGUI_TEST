#pragma once

#include "Component.h"

class CCalc_Timer final : public CComponent
{
private:
	CCalc_Timer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCalc_Timer(const CCalc_Timer& Prototype);
	virtual ~CCalc_Timer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)	override;
	void Update();

public:
	static CCalc_Timer* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fTimeDelta);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float PrevTimer = { 0.f };
	_float CurrTimer = { 0.f };

};

