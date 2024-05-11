#include "stdafx.h"
#include "..\Public\Tree.h"

#include "GameInstance.h"

CTree::CTree(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject{ pGraphic_Device }
{
}

CTree::CTree(const CTree& Prototype)
	: CLandObject{ Prototype }
{
}

HRESULT CTree::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CTree::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(0.5f, 0.5f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(3.0f, 0.f, 0.f));

	return S_OK;
}

void CTree::Priority_Update(_float fTimeDelta)
{
}

void CTree::Update(_float fTimeDelta)
{
	SetUp_OnTerrain(m_pTransformCom, 0.f);
}

void CTree::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CTree::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/* �簢������ �ø��� ���� �ױ��ĸ� �̸� ��ġ�� ���ε��Ѵ�.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	/* ���� ������ ��ȯ�ϱ����� �������(����, ��, ����)���� �����Ѵ�. ����!!!!!!! */
	_float4x4		ViewMatrix, ProjMatrix;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CTree::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Tree"),
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

CTree* CTree::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTree* pInstance = new CTree(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTree"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CTree::Clone(void* pArg)
{
	CTree* pInstance = new CTree(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTree"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTree::Free()
{
	__super::Free();

	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
