#include "stdafx.h"
#include "..\Public\Outside_PotHorizontal2.h"

#include "GameInstance.h"

COutside_PotHorizontal2::COutside_PotHorizontal2(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

COutside_PotHorizontal2::COutside_PotHorizontal2(const COutside_PotHorizontal2& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT COutside_PotHorizontal2::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT COutside_PotHorizontal2::Initialize(void* pArg)
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

	//�ݶ��̴� �纻�� ���鶧 Cube ���� �߰������ ��.
	if (FAILED(__super::Add_Component(LEVEL_TACHO, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//�ݶ��̴�������Ʈ �߰�
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_STATIC, this);

	return S_OK;
}

void COutside_PotHorizontal2::Priority_Update(_float fTimeDelta)
{
}

void COutside_PotHorizontal2::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void COutside_PotHorizontal2::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT COutside_PotHorizontal2::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	/* �簢������ �ø��� ���� �ױ��ĸ� �̸� ��ġ�� ���ε��Ѵ�.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

HRESULT COutside_PotHorizontal2::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_Outside_PotHorizontal2"),
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

	return S_OK;
}

COutside_PotHorizontal2* COutside_PotHorizontal2::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	COutside_PotHorizontal2* pInstance = new COutside_PotHorizontal2(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Outside_PotHorizontal2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* COutside_PotHorizontal2::Clone(void* pArg)
{
	COutside_PotHorizontal2* pInstance = new COutside_PotHorizontal2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Outside_PotHorizontal2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COutside_PotHorizontal2::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}