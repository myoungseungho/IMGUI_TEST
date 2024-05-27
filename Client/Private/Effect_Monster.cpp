#include "stdafx.h"

#include "Effect_Monster.h"

CEffect_Monster::CEffect_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect{ pGraphic_Device }
{
}

CEffect_Monster::CEffect_Monster(const CEffect_Monster& Prototype)
	:CEffect{ Prototype }
{
}

HRESULT CEffect_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Monster::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CEffect_Monster::Ready_Components()
{
	/* For.Com_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;

	return S_OK;
}

void CEffect_Monster::Free()
{
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pAnimCom);

	__super::Free();
}
