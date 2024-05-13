#include "stdafx.h"

#include "Boss_Bug.h"
#include "GameInstance.h"


CBoss_Bug::CBoss_Bug(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster{ pGraphic_Device }
{
}

CBoss_Bug::CBoss_Bug(const CBoss_Bug& Prototype)
	:CMonster{ Prototype }
{
}

HRESULT CBoss_Bug::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Bug::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

 	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBoss_Bug::Priority_Update(_float fTimeDelta)
{

}

void CBoss_Bug::Update(_float fTimeDelta)
{
	KeyInput(fTimeDelta);
}

void CBoss_Bug::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBoss_Bug::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	_float4x4		ViewMatrix, ProjMatrix;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CBoss_Bug::Ready_Components()
{
	/* For.Com_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Key"),
		TEXT("Com_Key"), reinterpret_cast<CComponent**>(&m_pKeyCom))))
		return E_FAIL;

	/* For.Com_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Bug::KeyInput(_float fTimeDelta)
{
	if (m_pKeyCom->Key_Down('1'))
	{
		CSkill_Bug_Bullet::SKILL_BUG_BULLET_DESC	SkillDesc{};
		SkillDesc.pTargetTransform = m_pTransformCom;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Bug_Bullet"), TEXT("Layer_Skill_Bug_Bullet"), &SkillDesc)))
			return E_FAIL;
	}

	if (m_pKeyCom->Key_Down('2') || m_isDash)
	{
		Dash(fTimeDelta);
	}

	if (m_pKeyCom->Key_Down('3') || m_isScale)
	{
		ScaleUp(fTimeDelta);
	}

	if (m_pKeyCom->Key_Down('4') || m_isScale)
	{
		ScaleDown(fTimeDelta);
	}
	

	return S_OK;
}

void CBoss_Bug::Dash(_float fTimeDelta)
{
	if (!m_pTimerCom->Time_Limit(fTimeDelta, 3.5f))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_isDash = true;
	}
	else {
		m_isDash = false;
	}
}

void CBoss_Bug::ScaleUp(_float fTimeDelta)
{
	if (!m_pTimerCom->Time_Limit(fTimeDelta, 2.5f))
	{
		m_pTransformCom->Set_Scaled(_float3(20.f, 20.f, 20.f) * fTimeDelta);
		m_isScale = true;
	}
	else
	{
		m_isScale = false;
	}
}

void CBoss_Bug::ScaleDown(_float fTimeDelta)
{
	if (!m_pTimerCom->Time_Limit(fTimeDelta, 2.5f))
	{
		m_pTransformCom->Set_Scaled(_float3(10.f, 10.f, 10.f) * fTimeDelta);
	}
	else
	{
		m_isScale = false;
	}
}

CBoss_Bug* CBoss_Bug::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoss_Bug* pInstance = new CBoss_Bug(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Boss_Bug"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Bug::Clone(void* pArg)
{
	CBoss_Bug* pInstance = new CBoss_Bug(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Boss_Bug"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Bug::Free()
{
	__super::Free();

	Safe_Release(m_pKeyCom);
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
