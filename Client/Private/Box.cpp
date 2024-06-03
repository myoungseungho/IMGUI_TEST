#include "stdafx.h"
#include "..\Public\Box.h"

#include "GameInstance.h"
#include <Player.h>
#include <Item_Skill.h>


CBox::CBox(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CBox::CBox(const CBox& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CBox::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CBox::Initialize(void* pArg)
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

void CBox::Priority_Update(_float fTimeDelta)
{
}

void CBox::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CBox::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBox::Render(_float fTimeDelta)
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

void CBox::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{		
	CGameObject* otherObject = other->m_MineGameObject;
	CPlayer* pCopyPlayer = dynamic_cast<CPlayer*>(otherObject);

	if (pCopyPlayer&&m_bIsFirstSkill)
	{
		m_eAnimState = ANIM_MOVE;
		pCopyPlayer->m_bHaveSkill = true;
		pCopyPlayer->m_ePlayerCurState = pCopyPlayer->STATE_GET;
		pCopyPlayer->m_bAttack = true;
		m_bIsFirstSkill = false;

		CItem_Skill::ITEM_SKILL_DESC ITEMSKILLDESC{};

		ITEMSKILLDESC.pTargetTransform = m_pTransformCom;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_JUNGLE, TEXT("Prototype_GameObject_Item_Skill"), TEXT("Layer_Item_Skill"), &ITEMSKILLDESC);

		// 사운드 재생
		if (FAILED(m_pGameInstance->Play_Sound(L"SFX_QuestClear", LEVEL_STATIC, false)))
			return E_FAIL;
	}
}

void CBox::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CBox::OnCollisionExit(class CCollider* other)
{
}

HRESULT CBox::Ready_Components()
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

HRESULT CBox::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_JUNGLE, TEXT("Prototype_Component_AnimTexture_Box_Idle"), TEXT("AnimTexture_Box_Idle"));
	m_pAnimCom->Add_Animator(LEVEL_JUNGLE, TEXT("Prototype_Component_AnimTexture_Box_Open"), TEXT("AnimTexture_Box_Open"));

	return S_OK;
}

void CBox::AnimState(_float _fTimeDelta)
{
	switch (m_eAnimState)
	{
	case ANIMATION_STATE::ANIM_IDLE:
		m_pAnimCom->Play_Animator(TEXT("AnimTexture_Box_Idle"), 0.1f, _fTimeDelta, false);
		break;

	case ANIMATION_STATE::ANIM_MOVE:
		m_pAnimCom->Play_Animator(TEXT("AnimTexture_Box_Open"), 1.f, _fTimeDelta, false);
		break;
	}
}

CBox* CBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBox* pInstance = new CBox(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CBox::Clone(void* pArg)
{
	CBox* pInstance = new CBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBox::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pAnimCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();

}
