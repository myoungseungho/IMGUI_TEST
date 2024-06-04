#include "stdafx.h"

#include "Monster.h"
#include "GameInstance.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 _pGraphicDevice)
	:CBlendObject{ _pGraphicDevice }
{
}

CMonster::CMonster(const CMonster& Prototype)
	:CBlendObject{ Prototype }
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
		return E_FAIL;

	return S_OK;
}

void CMonster::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
}

void CMonster::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CMonster::OnCollisionExit(class CCollider* other)
{
}

void CMonster::Move_Dir(_float _floatTimeDelta)
{
	m_fDirAngle = m_pTransformCom->Target_Dir_Degree(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	if (m_fDirAngle >= 337.5f || m_fDirAngle <= 22.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_U;
	}
	else if (m_fDirAngle <= 67.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_RU;
	}
	else if (m_fDirAngle <= 112.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_R;
	}
	else if (m_fDirAngle <= 157.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_RD;
	}
	else if (m_fDirAngle <= 202.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_D;
	}
	else if (m_fDirAngle <= 247.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_LD;
	}
	else if (m_fDirAngle <= 292.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_L;
	}
	else if (m_fDirAngle < 337.5f)
	{
		m_eMon_Dir = MON_DIR::DIR_LU;
	}
	else
	{
		MSG_BOX(TEXT("Dir_Error"));
	}
}

void CMonster::Free()
{
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pAnimCom);

	__super::Free();
}
