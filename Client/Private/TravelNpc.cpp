#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "GameInstance.h"
#include "..\Public\TravelNpc.h"

CTravelNpc::CTravelNpc(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CTravelNpc::CTravelNpc(const CTravelNpc& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CTravelNpc::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CTravelNpc::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	NPCDESC* npcDesc = reinterpret_cast<NPCDESC*>(pArg);
	m_wStrNpcName = npcDesc->npcName;

	m_pTransformCom->Set_Scaled(npcDesc->scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &npcDesc->position);

	return S_OK;
}

void CTravelNpc::Priority_Update(_float fTimeDelta)
{
}

void CTravelNpc::Update(_float fTimeDelta)
{

}


void CTravelNpc::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CTravelNpc::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	/* �簢������ �ø��� ���� �ױ��ĸ� �̸� ��ġ�� ���ε��Ѵ�.  */
	AnimState(fTimeDelta);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CTravelNpc::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	
}

void CTravelNpc::OnCollisionStay(CCollider* other, _float fTimeDelta)
{

}

void CTravelNpc::OnCollisionExit(class CCollider* other)
{

}

HRESULT CTravelNpc::Ready_Components()
{
	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;


	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};


	TransformDesc.fSpeedPerSec = 3.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(49.5f, 0.5f, 28.f));


	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 0.5f;
	ColliderDesc.MineGameObject = this;

	//�ݶ��̴� �纻�� ���鶧 Cube ���� �߰������ ��.
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//�ݶ��̴�������Ʈ �߰�
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_NPC, this);

	return S_OK;
}

HRESULT CTravelNpc::Ready_Animation()
{
	// Idle
	m_pAnimCom->Add_Animator(LEVEL_TACHO, TEXT("Prototype_Component_AnimTexture_Sprite_TutorialVillager_Pig_Idle"), TEXT("Npc_Idle"));
	// Reaction
	m_pAnimCom->Add_Animator(LEVEL_TACHO, TEXT("Prototype_Component_AnimTexture_Sprite_TutorialVillager_Pig_Bye"), TEXT("Npc_Reaction"));

	return S_OK;
}

HRESULT CTravelNpc::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CTravelNpc::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}


void CTravelNpc::AnimState(_float _fTimeDelta)
{
	switch (m_eAnimState)
	{
	case Client::CTravelNpc::ANIM_IDLE:
		m_pAnimCom->Play_Animator(TEXT("Npc_Idle"), 0.5f, _fTimeDelta, true);
		break;
	case Client::CTravelNpc::ANIM_REACTION:
		m_pAnimCom->Play_Animator(TEXT("Npc_Reaction"), 0.5f, _fTimeDelta, true);
		break;
	default:
		break;
	}
}

CTravelNpc* CTravelNpc::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTravelNpc* pInstance = new CTravelNpc(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTravelNpc"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CTravelNpc::Clone(void* pArg)
{
	CTravelNpc* pInstance = new CTravelNpc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTravelNpc"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTravelNpc::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pAnimCom);
	Safe_Release(m_pColliderCom);
	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
