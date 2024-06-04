#include "stdafx.h"
#include "..\Public\End_Orb.h"

#include "GameInstance.h"
#include <Player.h>
#include <Un_Laser.h>
#include <Effect_Orb.h>
#include <Camera.h>

_uint CEnd_Orb::m_eClearState = CEnd_Orb::STATE_UNCLEAR;

CEnd_Orb::CEnd_Orb(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CEnd_Orb::CEnd_Orb(const CEnd_Orb& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CEnd_Orb::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CEnd_Orb::Initialize(void* pArg)
{
	END_ORB_DESC* pDesc = static_cast<END_ORB_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x, vTargetPos.y + 0.5f, vTargetPos.z - 0.01f));


	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 1.f;
	ColliderDesc.MineGameObject = this;

	//콜라이더 사본을 만들때 Cube 정보 추가해줘야 함.
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//콜라이더오브젝트 추가
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_STATIC, this);

	m_eClearState = STATE_UNCLEAR;

	return S_OK;
}

void CEnd_Orb::Priority_Update(_float fTimeDelta)
{
}

void CEnd_Orb::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	m_eClearState = m_eClearState;
}

void CEnd_Orb::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CEnd_Orb::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

void CEnd_Orb::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CUn_Laser*>(otherObject))
	{
		CEnd_Orb::m_eClearState = CEnd_Orb::STATE_CLEAR;

		if (m_bOrbClearSoundOnce)
		{
			CGameObject* cameraObject = m_pGameInstance->Get_GameObject(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Camera"));
			static_cast<CCamera*>(cameraObject)->ShakeCamera(2.f, 0.1f, 0.1f);

			m_pGameInstance->Play_Sound(L"SFX_99_MazePuzzle_Complete", LEVEL_STATIC, false);
			m_pGameInstance->Set_Volume(L"SFX_99_MazePuzzle_Complete", LEVEL_STATIC, 1.f);
			m_bOrbClearSoundOnce = false;

			CEffect_Orb::EFFECT_ORB_DESC			EFFECTORB{};

			EFFECTORB.pTargetTransform = m_pTransformCom;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_Effect_Orb"), TEXT("Layer_Effect_Orb"), &EFFECTORB);
		}

		return;
	}
}

void CEnd_Orb::OnCollisionStay(CCollider* other, _float fTimeDelta)
{

}

void CEnd_Orb::OnCollisionExit(class CCollider* other)
{
}

HRESULT CEnd_Orb::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_EndOrb"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Timer */
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

CEnd_Orb* CEnd_Orb::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnd_Orb* pInstance = new CEnd_Orb(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEnd_Orb"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CEnd_Orb::Clone(void* pArg)
{
	CEnd_Orb* pInstance = new CEnd_Orb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEnd_Orb"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnd_Orb::Free()
{
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
