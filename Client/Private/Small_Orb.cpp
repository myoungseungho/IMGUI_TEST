#include "stdafx.h"
#include "..\Public\Small_Orb.h"

#include "GameInstance.h"
#include <Player.h>
#include <Laser.h>


CSmall_Orb::CSmall_Orb(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CSmall_Orb::CSmall_Orb(const CSmall_Orb& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CSmall_Orb::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CSmall_Orb::Initialize(void* pArg)
{
	SMALL_ORB_DESC* pDesc = static_cast<SMALL_ORB_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//if (m_bIsPasingObject)
	//{
	//	FILEDATA* fileData = static_cast<FILEDATA*>(pArg);
	//	m_pTransformCom->Set_Scaled(_float3(fileData->scale.x, fileData->scale.y, fileData->scale.z));
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(fileData->position.x, fileData->position.y, fileData->position.z));
	//}


	_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x, vTargetPos.y +0.1f, vTargetPos.z - 0.02f));
	m_pTransformCom->Set_Scaled(_float3(0.5f, 0.5f, 0.5f));

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

	m_eDirection = DIR_DOWN;


	return S_OK;
}

void CSmall_Orb::Priority_Update(_float fTimeDelta)
{
}

void CSmall_Orb::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_eDirection == DIR_DOWN)
	{
		_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x, vTargetPos.y + 0.1f, vTargetPos.z - 0.02f));
	}
	else if (m_eDirection == DIR_LEFT)
	{
		_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x-0.5f, vTargetPos.y + 0.1f, vTargetPos.z - 0.02f));
	}
	else if (m_eDirection == DIR_UP)
	{
		_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x, vTargetPos.y + 0.1f, vTargetPos.z + 0.02f));
	}
	else if (m_eDirection == DIR_RIGHT)
	{
		_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x+0.5f, vTargetPos.y + 0.1f, vTargetPos.z - 0.02f));
	}



	while (bTest)
	{

		CLaser::LASER_DESC			LASERDESC{};

		LASERDESC.pTargetTransform = m_pTransformCom;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_Laser"), TEXT("Layer_Laser"), &LASERDESC);

		bTest = false;
	}

}

void CSmall_Orb::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSmall_Orb::Render(_float fTimeDelta)
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

void CSmall_Orb::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CPlayer*>(otherObject))
	{
		CPlayer* pCopyPlayer = dynamic_cast<CPlayer*>(otherObject);

		if (pCopyPlayer->Get_Player_State() == 2)
		{
			if (m_eDirection == DIR_DOWN )
			{
				m_eDirection = DIR_LEFT;
				return;
			}

			if (m_eDirection == DIR_LEFT)
			{
				m_eDirection = DIR_UP;
				return;
			}

			if (m_eDirection == DIR_UP)
			{
				m_eDirection = DIR_RIGHT;
				return;
			}

			if (m_eDirection == DIR_RIGHT)
			{
				m_eDirection = DIR_DOWN;
				return;
			}
		}
	}
}

void CSmall_Orb::OnCollisionStay(CCollider* other, _float fTimeDelta)
{

}

void CSmall_Orb::OnCollisionExit(class CCollider* other)
{

}

HRESULT CSmall_Orb::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_SmallOrb"),
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

CSmall_Orb* CSmall_Orb::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSmall_Orb* pInstance = new CSmall_Orb(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSmall_Orb"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CSmall_Orb::Clone(void* pArg)
{
	CSmall_Orb* pInstance = new CSmall_Orb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CSmall_Orb"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSmall_Orb::Free()
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
