#include "stdafx.h"
#include "..\Public\Push_Stone.h"

#include "GameInstance.h"
#include <Effect_PushStone.h>
#include <Player.h>

CPush_Stone::CPush_Stone(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CPush_Stone::CPush_Stone(const CPush_Stone& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CPush_Stone::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CPush_Stone::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (m_bIsPasingObject)
	{
		FILEDATA* fileData = static_cast<FILEDATA*>(pArg);
		m_pTransformCom->Set_Scaled(_float3(fileData->scale.x, fileData->scale.y, fileData->scale.z));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(fileData->position.x, fileData->position.y, fileData->position.z));
	}


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

	return S_OK;
}

void CPush_Stone::Priority_Update(_float fTimeDelta)
{
}

void CPush_Stone::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CPush_Stone::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CPush_Stone::Render(_float fTimeDelta)
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

void CPush_Stone::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
}

void CPush_Stone::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;
	CPlayer* pCopyPlayer = dynamic_cast<CPlayer*>(otherObject);

	if (pCopyPlayer && pCopyPlayer->Get_Player_CurState() == 4)
	{

		if (m_bPushSoundOnce)
		{
			m_pGameInstance->Play_Sound(L"SFX_StonePushable_Push", LEVEL_STATIC, true);
			m_bPushSoundOnce = false;
		}
	}
}

void CPush_Stone::OnCollisionExit(class CCollider* other)
{
	m_pGameInstance->Stop_Sound(L"SFX_StonePushable_Push", LEVEL_STATIC);
	m_bPushSoundOnce = true;
}

HRESULT CPush_Stone::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Sprite_StonePushable"),
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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(30.f, 0.5f, 10.f));

	return S_OK;
}

void CPush_Stone::Push_Move(_float fTimeDelta, _uint ePlayerDir)
{
	switch (ePlayerDir)
	{
	case DIR_LEFT:
		m_pTransformCom->Go_Left(fTimeDelta);
		Creat_Dust(fTimeDelta, ePlayerDir);
		break;
	case DIR_UP:
		m_pTransformCom->Go_Straight(fTimeDelta);
		Creat_Dust(fTimeDelta, ePlayerDir);
		break;
	case DIR_RIGHT:
		m_pTransformCom->Go_Right(fTimeDelta);
		Creat_Dust(fTimeDelta, ePlayerDir);
		break;
	case DIR_DOWN:
		m_pTransformCom->Go_Backward(fTimeDelta);
		Creat_Dust(fTimeDelta, ePlayerDir);
		break;
	case DIR_LEFTUP:
		m_pTransformCom->Go_Left(fTimeDelta);
		Creat_Dust(fTimeDelta, ePlayerDir);
		break;
	case DIR_RIGHTUP:
		m_pTransformCom->Go_Right(fTimeDelta);
		Creat_Dust(fTimeDelta, ePlayerDir);
		break;
	case DIR_RIGHTDOWN:
		m_pTransformCom->Go_Right(fTimeDelta);
		Creat_Dust(fTimeDelta, ePlayerDir);
		break;
	case DIR_LEFTDOWN:
		m_pTransformCom->Go_Left(fTimeDelta);
		Creat_Dust(fTimeDelta, ePlayerDir);
		break;
	}
}

void CPush_Stone::Creat_Dust(_float fTimeDelta, _uint ePlayerDir)
{
	if (m_bMakeOnce)
	{
		CEffect_PushStone::EFFECT_PUSHSTONE_DESC EFFECTPUSHSTONE{};

		EFFECTPUSHSTONE.pTargetTransform = m_pTransformCom;
		EFFECTPUSHSTONE.pDirection = ePlayerDir;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_Effect_PushStone"), TEXT("Layer_Effect_PushStone"), &EFFECTPUSHSTONE);

		m_bMakeOnce = false;
	}

	m_DustTime += fTimeDelta;

	if (m_DustTime >= 1.f)
	{
		m_bMakeOnce = true;
		m_DustTime = 0.f;
	}
	
}

CPush_Stone* CPush_Stone::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPush_Stone* pInstance = new CPush_Stone(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPush_Stone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CPush_Stone::Clone(void* pArg)
{
	CPush_Stone* pInstance = new CPush_Stone(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPush_Stone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPush_Stone::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
