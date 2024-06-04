#include "stdafx.h"
#include "..\Public\Soil_3.h"

#include "GameInstance.h"

CSoil_3::CSoil_3(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CSoil_3::CSoil_3(const CSoil_3& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CSoil_3::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CSoil_3::Initialize(void* pArg)
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

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	return S_OK;
}

void CSoil_3::Priority_Update(_float fTimeDelta)
{
}

void CSoil_3::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CSoil_3::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSoil_3::Render(_float fTimeDelta)
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

HRESULT CSoil_3::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Soil_3"),
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

CSoil_3* CSoil_3::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSoil_3* pInstance = new CSoil_3(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSoil_3"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CSoil_3::Clone(void* pArg)
{
	CSoil_3* pInstance = new CSoil_3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CSoil_3"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSoil_3::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}

