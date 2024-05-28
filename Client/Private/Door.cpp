#include "stdafx.h"
#include "..\Public\Door.h"

#include "GameInstance.h"
#include <End_Orb.h>



CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CDoor::CDoor(const CDoor& Prototype)
	: CEnviormentObject{ Prototype }

{
}

HRESULT CDoor::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CDoor::Initialize(void* pArg)
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

	//�ݶ��̴� �纻�� ���鶧 Cube ���� �߰������ ��.
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//�ݶ��̴�������Ʈ �߰�
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_STATIC, this);

	return S_OK;
}

void CDoor::Priority_Update(_float fTimeDelta)
{
}

void CDoor::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (CEnd_Orb::m_eClearState ==1)
		m_eAnimState = ANIMATION_STATE::ANIM_BLOCK;
	else
		m_eAnimState = ANIMATION_STATE::ANIM_UNBLOCK;

}

void CDoor::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CDoor::Render(_float fTimeDelta)
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

void CDoor::AnimState(_float _fTimeDelta)
{

	switch (m_eAnimState)
	{
	case ANIMATION_STATE::ANIM_IDLE:
		m_pAnimCom->Play_Animator(TEXT("AnimTexture_Block_Idle"), 0.5f, _fTimeDelta, false);
		break;

	case ANIMATION_STATE::ANIM_UNIDLE:
		m_pAnimCom->Play_Animator(TEXT("AnimTexture_Block_UnIdle"), 0.5f, _fTimeDelta, false);
		break;

	case ANIMATION_STATE::ANIM_BLOCK:
		m_pAnimCom->Play_Animator(TEXT("AnimTexture_UnBlock"), 0.5f, _fTimeDelta, false);
		break;

	case ANIMATION_STATE::ANIM_UNBLOCK:
		m_pAnimCom->Play_Animator(TEXT("AnimTexture_Block"), 0.5f, _fTimeDelta, false);
		break;
	}
}


void CDoor::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
}

void CDoor::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CDoor::OnCollisionExit(class CCollider* other)
{
}

HRESULT CDoor::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Block_Idle"),
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

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CDoor::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Block_Idle"), TEXT("AnimTexture_Block_Idle"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Block_UnIdle"), TEXT("AnimTexture_Block_UnIdle"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_UnBlock"), TEXT("AnimTexture_UnBlock"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Block"), TEXT("AnimTexture_Block"));

	return S_OK;
}


CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDoor* pInstance = new CDoor(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CDoor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CDoor::Clone(void* pArg)
{
	CDoor* pInstance = new CDoor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CDoor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoor::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pAnimCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
