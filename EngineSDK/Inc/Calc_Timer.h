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
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg)	override;

public:
	_bool Time_Limit(_float fTimeDelta, _float fTimer)
	{
		m_fTimer += fTimeDelta;

		if (m_fTimer >= fTimer)
		{
			m_fTimer = { 0 };
		
			return true;
		}
		else
			return false;
	}

	_bool Time_Limit(_float fTimeDelta, _float fStartTimer , _float fEndTimer)
	{
		m_fCurrTimer += fTimeDelta;
		
		if (Time_Limit(fTimeDelta, fStartTimer))
		{
			m_isTimeCheck = true;
		}

		if (m_isTimeCheck)
		{
			m_fPrevTimer += fTimeDelta;

			if (m_fPrevTimer <= fEndTimer)
			{
				return true;
			}
		}
		else
		{
			m_isTimeCheck = false;
			m_fPrevTimer = { 0 };
			m_fCurrTimer = { 0 };
			return false;
		}
	}
	
public:
	static CCalc_Timer* Create(LPDIRECT3DDEVICE9 pGraphic_DevicefTimeDelta);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float m_fTimer = { 0.f };
	_float m_fPrevTimer = { 0.f };
	_float m_fCurrTimer = { 0.f };
	_bool m_isTimeCheck = { false };

};

END