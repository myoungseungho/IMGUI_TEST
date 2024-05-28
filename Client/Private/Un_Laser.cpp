#include "stdafx.h"
#include "..\Public\Un_Laser.h"

#include "GameInstance.h"
#include <Player.h>
#include "UnRotation_Orb.h"
#include <End_Orb.h>
#include <Rotation_Orb.h>
#include <UnRotation_Stand.h>
#include <Un_Small_Orb.h>

CUn_Laser::CUn_Laser(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CUn_Laser::CUn_Laser(const CUn_Laser& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CUn_Laser::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUn_Laser::Initialize(void* pArg)
{
	UN_LASER_DESC* pDesc = static_cast<UN_LASER_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	m_eDirection = (DIRECTION)pDesc->iLaserDir;

	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x, vTargetPos.y, vTargetPos.z));
	m_pTransformCom->Set_Scaled(_float3(0.6f, 0.6f, 0.6f));


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
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_LASER, this);

	return S_OK;
}

void CUn_Laser::Priority_Update(_float fTimeDelta)
{
}

void CUn_Laser::Update(_float fTimeDelta)
{

	if (m_pTimerCom->Time_Limit(fTimeDelta, 2.0f))
	{
		m_Died = true;

		return;
	}

	__super::Update(fTimeDelta);

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (m_eDirection == DIR_LEFT)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vPos.x - 0.2f, vPos.y, vPos.z));
		}
		else if (m_eDirection == DIR_UP)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vPos.x, vPos.y, vPos.z + 0.2f));
		}
		else if (m_eDirection == DIR_RIGHT)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vPos.x + 0.2f, vPos.y, vPos.z));
		}
		else if (m_eDirection == DIR_DOWN)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vPos.x, vPos.y, vPos.z - 0.2f));
		}

	
}

void CUn_Laser::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CUn_Laser::Render(_float fTimeDelta)
{
	__super::Begin_Blend_RenderState();

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_Blend_RenderState();

	return S_OK;
}

void CUn_Laser::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CEnd_Orb*>(otherObject))
	{
		m_Died = true;
		
		return;
	}
	else if (dynamic_cast<CRotation_Orb*>(otherObject))
	{
		m_Died = true;

		return;
	}
	else if (dynamic_cast<CUn_Small_Orb*>(otherObject) && m_pTimerCom->Time_Limit(fTimeDelta, 0.1f))
	{
		m_Died = true;

		return;
	}
}

void CUn_Laser::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CUn_Laser::OnCollisionExit(CCollider* other)
{
}

HRESULT CUn_Laser::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Laser"),
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

CUn_Laser* CUn_Laser::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUn_Laser* pInstance = new CUn_Laser(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUn_Laser"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUn_Laser::Clone(void* pArg)
{
	CUn_Laser* pInstance = new CUn_Laser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUn_Laser"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUn_Laser::Free()
{
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}

