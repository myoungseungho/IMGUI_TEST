#include "stdafx.h"

#include "Monster.h"
#include "GameInstance.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 _pGraphicDevice)
	:CGameObject{ _pGraphicDevice }
{
}

CMonster::CMonster(const CMonster& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CMonster::Ready_Components()
{
	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;

	/* For.Com_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Key"),
		TEXT("Com_Key"), reinterpret_cast<CComponent**>(&m_pKeyCom))))
		return E_FAIL;

	/* For.Com_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
		return E_FAIL;

	return S_OK;
}

void CMonster::OnCollisionEnter(CCollider* other)
{
}

void CMonster::OnCollisionStay(CCollider* other)
{
}

void CMonster::OnCollisionExit(CCollider* other)
{
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pKeyCom);
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pAnimCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);
}
