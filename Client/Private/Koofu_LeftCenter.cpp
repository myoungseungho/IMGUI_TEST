#include "stdafx.h"
#include "..\Public\Koofu_LeftCenter.h"

#include "GameInstance.h"

CKoofu_LeftCenter::CKoofu_LeftCenter(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CKoofu_LeftCenter::CKoofu_LeftCenter(const CKoofu_LeftCenter& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CKoofu_LeftCenter::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CKoofu_LeftCenter::Initialize(void* pArg)
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

	m_pTransformCom->Rotation(_float3(1, 0, 0), 3.14f / 2.f);

	return S_OK;
}

void CKoofu_LeftCenter::Priority_Update(_float fTimeDelta)
{
}

void CKoofu_LeftCenter::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CKoofu_LeftCenter::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CKoofu_LeftCenter::Render(_float fTimeDelta)
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

HRESULT CKoofu_LeftCenter::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Left"),
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

CKoofu_LeftCenter* CKoofu_LeftCenter::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CKoofu_LeftCenter* pInstance = new CKoofu_LeftCenter(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CKoofu_LeftCenter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CKoofu_LeftCenter::Clone(void* pArg)
{
	CKoofu_LeftCenter* pInstance = new CKoofu_LeftCenter(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CKoofu_LeftCenter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKoofu_LeftCenter::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
