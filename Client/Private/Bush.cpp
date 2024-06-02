#include "stdafx.h"
#include "..\Public\Bush.h"

#include "GameInstance.h"
#include <Player.h>
#include <Effect_Bush_1.h>

CBush::CBush(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CBush::CBush(const CBush& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CBush::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CBush::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (m_bIsPasingObject)
	{
		FILEDATA* fileData = static_cast<FILEDATA*>(pArg);
		m_pTransformCom->Set_Scaled(_float3(fileData->scale.x, fileData->scale.y, fileData->scale.z));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(fileData->position.x, fileData->position.y, fileData->position.z));
	}
	/*else
	{
		CBush::BUSHDESC* bushDesc = static_cast<CBush::BUSHDESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &bushDesc->startPosition);
	}*/

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

void CBush::Priority_Update(_float fTimeDelta)
{
}

void CBush::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (GetAsyncKeyState('G') & 0x8000)
	{
		m_eAnimState = ANIMATION_STATE::ANIM_MOVE;
	}
	else if(GetAsyncKeyState('H') & 0x8000)
	{
		m_eAnimState = ANIMATION_STATE::ANIM_IDLE;
	}
}

void CBush::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBush::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	AnimState(fTimeDelta);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

void CBush::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	m_eAnimState = ANIMATION_STATE::ANIM_MOVE;

	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CPlayer*>(otherObject))
	{
		CPlayer* pCopyPlayer = dynamic_cast<CPlayer*>(otherObject);

		if (pCopyPlayer->Get_Player_CurState() == 2)
		{
			CEffect_Bush_1::EFFECT_BUSH_DESC EFFECTBUSH{};

			EFFECTBUSH.pTargetTransform = m_pTransformCom;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Bush_1"), TEXT("Layer_Effect_Bush_1"), &EFFECTBUSH);

			Delete_Object();

		}
	}
		
}

void CBush::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CPlayer*>(otherObject))
	{
		CPlayer* pCopyPlayer = dynamic_cast<CPlayer*>(otherObject);

		if (pCopyPlayer->Get_Player_CurState() == 2)
		{
			Delete_Object();
		}
	}
}

void CBush::OnCollisionExit(class CCollider* other)
{
	m_eAnimState = ANIMATION_STATE::ANIM_IDLE;
}

HRESULT CBush::Ready_Components()
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

	return S_OK;
}

HRESULT CBush::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Sprite_Bush_Idle"), TEXT("AnimTexture_Bush_Idle"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Sprite_Bush_Move"), TEXT("AnimTexture_Bush_Move"));

	return S_OK;
}

void CBush::AnimState(_float _fTimeDelta)
{
	switch (m_eAnimState)
	{
	case ANIMATION_STATE::ANIM_IDLE:
		m_pAnimCom->Play_Animator(TEXT("AnimTexture_Bush_Idle"), 0.3f, _fTimeDelta, false);
		break;

	case ANIMATION_STATE::ANIM_MOVE:
		m_pAnimCom->Play_Animator(TEXT("AnimTexture_Bush_Move"), 0.3f, _fTimeDelta, false);
		break;
	}
}

CBush* CBush::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBush* pInstance = new CBush(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBush"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CBush::Clone(void* pArg)
{
	CBush* pInstance = new CBush(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBush"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBush::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pAnimCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();

}
