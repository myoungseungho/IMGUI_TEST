#include "stdafx.h"
#include "Mon_Turtle.h"

#include "GameInstance.h"

CMon_Turtle::CMon_Turtle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster{ pGraphic_Device }
{
}

CMon_Turtle::CMon_Turtle(const CMon_Turtle& Prototype)	
	:CMonster{ Prototype }
{
}

HRESULT CMon_Turtle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMon_Turtle::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);

	m_tMonsterDesc.iHp = pDesc->iHp;
	m_tMonsterDesc.iAttack = pDesc->iAttack;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(rand() % 20, 0.f, rand() % 20));
	

	return S_OK;
}

void CMon_Turtle::Priority_Update(_float fTimeDelta)
{
}

void CMon_Turtle::Update(_float fTimeDelta)
{
	if(m_pTimerCom->Time_Limit(fTimeDelta,10.f))
		Death();

	m_pGameInstance->Delete_GaemObject(LEVEL_GAMEPLAY ,TEXT("Layer_Monster_Turtle"));
}

void CMon_Turtle::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CMon_Turtle::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CMon_Turtle::Death()
{

	m_isDeath = true;
	
}

HRESULT CMon_Turtle::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
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

CMon_Turtle* CMon_Turtle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMon_Turtle* pInstance = new CMon_Turtle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMon_Turtle"));
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject* CMon_Turtle::Clone(void* pArg)
{
	CMon_Turtle* pInstance = new CMon_Turtle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMon_Turtle"));
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CMon_Turtle::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTimerCom);

	__super::Free();
}
