#include "stdafx.h"

#include "Skill_Monster.h"

CSkill_Monster::CSkill_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill{ pGraphic_Device }
{
}

CSkill_Monster::CSkill_Monster(const CSkill_Monster& Prototype)
	:CSkill{ Prototype }
{
}

HRESULT CSkill_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Monster::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CSkill_Monster::Ready_Components()
{
	/* For.Com_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;



	return S_OK;
}

void CSkill_Monster::Free()
{
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pVIBufferCom);
	
	__super::Free();
}
