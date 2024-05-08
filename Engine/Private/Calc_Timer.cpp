#include "Calc_Timer.h"

CCalc_Timer::CCalc_Timer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent { pGraphic_Device }
{
}

CCalc_Timer::CCalc_Timer(const CCalc_Timer& Prototype)
	:CComponent{ Prototype }
{
}

HRESULT CCalc_Timer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCalc_Timer::Initialize(void* pArg)
{
	return S_OK;
}

void CCalc_Timer::Update()
{
}

CCalc_Timer* CCalc_Timer::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fTimeDelta)
{
	return nullptr;
}

CComponent* CCalc_Timer::Clone(void* pArg)
{
	return nullptr;
}

void CCalc_Timer::Free()
{
}
