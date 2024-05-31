#include "stdafx.h"
#include "..\Public\Effect_Player.h"

#include "GameInstance.h"
#include <Player.h>

CEffect_Player::CEffect_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
}

CEffect_Player::CEffect_Player(const CEffect_Player& Prototype)
	: CEffect{ Prototype }
{
}

HRESULT CEffect_Player::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CEffect_Player::Initialize(void* pArg)
{
	EFFECT_PLAYER_DESC* pDesc = static_cast<EFFECT_PLAYER_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	m_pTagetDirection = pDesc->pTargetDirection;

	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x, vTargetPos.y, vTargetPos.z + 0.01f));
	m_pTransformCom->Rotation(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), D3DXToRadian(270.f));
	m_pTransformCom->Set_Scaled(_float3(3.f, 3.f, 1.f));

	if (m_pTagetDirection == 6)
	{
		m_pTransformCom->Rotation((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), D3DXToRadian(45.f));
	}
	else if (m_pTagetDirection == 3)
	{
		m_pTransformCom->Rotation((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), D3DXToRadian(90.f));
	}
	else if (m_pTagetDirection == 7)
	{
		m_pTransformCom->Rotation((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), D3DXToRadian(135.f));
	}
	else if (m_pTagetDirection == 0)
	{
		m_pTransformCom->Rotation((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), D3DXToRadian(180.f));
	}
	else if (m_pTagetDirection == 4)
	{
		m_pTransformCom->Rotation((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), D3DXToRadian(225.f));
	}
	else if (m_pTagetDirection == 1)
	{
		m_pTransformCom->Rotation((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), D3DXToRadian(270.f));
	}
	else if (m_pTagetDirection == 5)
	{
		m_pTransformCom->Rotation((m_pTransformCom->Get_State(CTransform::STATE_LOOK)), D3DXToRadian(315.f));
	}

	return S_OK;
}

void CEffect_Player::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Player::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	_uint level = m_pGameInstance->GetCurrentLevelIndex();
	CPlayer* pCopyPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(level, TEXT("Layer_Player")));

	if (pCopyPlayer->Get_Player_CurState() != pCopyPlayer->STATE_ATTACK)
	{
		m_Died = true;
	}


}

void CEffect_Player::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CEffect_Player::Render(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	__super::Begin_Blend_RenderState();

	Effect_Player_AnimState(fTimeDelta);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_Blend_RenderState();

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CEffect_Player::Ready_Components()
{
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

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;

	/* For.Com_Calc_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Calc_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Player::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Effect_Player"), TEXT("Effect_Player_Swing_Right"));

	return S_OK;
}

void CEffect_Player::Effect_Player_AnimState(_float _fTimeDelta)
{
	m_pAnimCom->Play_Animator(TEXT("Effect_Player_Swing_Right"), 1.0f, _fTimeDelta, false);
}

CEffect_Player* CEffect_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Player* pInstance = new CEffect_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CEffect_Player::Clone(void* pArg)
{
	CEffect_Player* pInstance = new CEffect_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Player::Free()
{
	Safe_Release(m_pAnimCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}

