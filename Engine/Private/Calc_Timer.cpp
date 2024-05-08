#include "Calc_Timer.h"

CCalc_Timer::CCalc_Timer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent { pGraphic_Device }
{
}

CCalc_Timer::CCalc_Timer(const CCalc_Timer& Prototype)
	:CComponent{ Prototype }
{
}

HRESULT CCalc_Timer::Initialize_Prototype(_float fTimeDelta)
{

	return S_OK;
}

HRESULT CCalc_Timer::Initialize(void* pArg)
{
	return S_OK;
}

CCalc_Timer* CCalc_Timer::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fTimeDelta)
{
	CCalc_Timer* pInstance = new CCalc_Timer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(fTimeDelta)))
	{
		MSG_BOX(TEXT("Failed to Created : Calc_Timer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCalc_Timer::Clone(void* pArg)
{
	CCalc_Timer* pInstance = new CCalc_Timer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Calc_Timer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCalc_Timer::Free()
{
	__super::Free();
}
