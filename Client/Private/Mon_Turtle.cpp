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

	MON_TURTLE_DESC* pDesc = static_cast<MON_TURTLE_DESC*>(pArg);

	m_tMonsterDesc.iHp = pDesc->iHp;
	m_tMonsterDesc.iAttack = pDesc->iAttack;
	m_pTargetTransform = pDesc->pTargetTransform;

	Safe_AddRef(m_pTargetTransform);

	m_ColorTexTag = pDesc->ColorTexTag;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(rand() % 20 + 20, 3.f, rand() % 20 + 10));
	
	return S_OK;
}

void CMon_Turtle::Priority_Update(_float fTimeDelta)
{
}

void CMon_Turtle::Update(_float fTimeDelta)
{
	Mon_State(fTimeDelta);
	Move_Range(10.f, 10.f, 30.f, 30.f);
}

void CMon_Turtle::Late_Update(_float fTimeDelta)
{
	Distory(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CMon_Turtle::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	m_pAnimCom->Play_Animator(TEXT("MON_TURTLE_MOVE"), 0.5f, fTimeDelta, true);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CMon_Turtle::Mon_State(_float fTimeDelta)
{
	if (rand() % 10 > 2)
	{
		m_eMon_State = MON_STATE::MOVE;
	}
	else
	{
		m_eMon_State = MON_STATE::MOVE;
	}

	switch (m_eMon_State)
	{
	case MON_STATE::IDLE:
		Idle_Update(fTimeDelta);
		break;

	case MON_STATE::MOVE:
		Move_Update(fTimeDelta);
		break;
	}
}

void CMon_Turtle::Idle_Update(_float fTimeDelta)
{

}

void CMon_Turtle::Move_Update(_float fTimeDelta)
{
	m_pTransformCom->Away(m_pTargetTransform->Get_State(CTransform::STATE_POSITION), fTimeDelta , 10.f);
}

void CMon_Turtle::Distory(_float fTimeDelta)
{

}

void CMon_Turtle::Move_Range(_float fMinPosX, _float fMinPosZ, _float fMaxPosX, _float fMaxPosZ)
{
	_float fPosX = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
	_float fPosZ = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

	if ((fPosX <= fMinPosX || fPosX >= fMaxPosX) || (fPosZ <= fMinPosZ || fPosZ >= fMaxPosZ))
		m_pTransformCom->Set_Speed(0.f);
}

HRESULT CMon_Turtle::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_ColorTexTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 3.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMon_Turtle::Ready_Animation()
{
	if (FAILED(m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, m_ColorTexTag, TEXT("MON_TURTLE_MOVE"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMon_Turtle::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CMon_Turtle::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

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
	Safe_Release(m_pTargetTransform);

	__super::Free();
}
