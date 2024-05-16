#include "stdafx.h"

#include "Boss_Koofu.h"
#include "GameInstance.h"


CBoss_Koofu::CBoss_Koofu(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster{ pGraphic_Device }
{
}

CBoss_Koofu::CBoss_Koofu(const CBoss_Koofu& Prototype)
	:CMonster{ Prototype }
{
}

HRESULT CBoss_Koofu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Koofu::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

 	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(3.0f, 0.f, 0.f));

	m_eMon_State = MON_STATE::WARF;

	return S_OK;
}

void CBoss_Koofu::Priority_Update(_float fTimeDelta)
{

}

void CBoss_Koofu::Update(_float fTimeDelta)
{
	MonState(fTimeDelta);
}

void CBoss_Koofu::Late_Update(_float fTimeDelta)
{ 
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBoss_Koofu::Render()
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

void CBoss_Koofu::MonState(_float fTimeDelta)
{
	switch (m_eMon_State)
	{
	case MON_STATE::IDLE:
		State_Idle(fTimeDelta);
		break;

	case MON_STATE::WARF:
		State_Warf(fTimeDelta);
		break;

	case MON_STATE::READY:
		State_Ready(fTimeDelta);
		break;

	case MON_STATE::BULLET:
		State_Bullet(fTimeDelta);
		break;
	}
}

void CBoss_Koofu::State_Idle(_float fTimeDelta)
{
}

void CBoss_Koofu::State_Warf(_float fTimeDelta)
{
	if(m_pTimerCom->Time_Limit(fTimeDelta, 3.f))
		Wafe(10,10 , 20,20);
}

void CBoss_Koofu::State_Ready(_float fTimeDelta)
{
}

void CBoss_Koofu::State_Bullet(_float fTimeDelta)
{

}

HRESULT CBoss_Koofu::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
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

void CBoss_Koofu::ScaleUp(_float fTimeDelta)
{
	if (!m_pTimerCom->Time_Limit(fTimeDelta,1.f ,3.5f))
		m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f) * (fTimeDelta + 1));
}

void CBoss_Koofu::Wafe(_int fMinPosX, _int fMinPosZ , _int fMaxPosX , _int fMaxPosZ)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(rand() % fMaxPosX - fMinPosX, 0.f, rand() % fMaxPosZ - fMinPosZ));
}


CBoss_Koofu* CBoss_Koofu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoss_Koofu* pInstance = new CBoss_Koofu(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Boss_Koofu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Koofu::Clone(void* pArg)
{
	CBoss_Koofu* pInstance = new CBoss_Koofu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{

		MSG_BOX(TEXT("Failed to Cloned : Boss_Koofu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Koofu::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
