#include "stdafx.h"
#include "..\Public\Sky.h"

#include "GameInstance.h"

CSky::CSky(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CSky::CSky(const CSky & Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CSky::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CSky::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;	

	return S_OK;
}

void CSky::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CSky::Update(_float fTimeDelta)
{

}

void CSky::Late_Update(_float fTimeDelta)
{
	_float4x4		ViewMatrix{};

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (_float3*)&ViewMatrix.m[3][0]);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);

	
}

HRESULT CSky::Render(_float fTimeDelta)
{	
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(3)))
		return E_FAIL;	

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;
	
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;	

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSky::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_EDIT, TEXT("Prototype_Component_Texture_Sky"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_EDIT, TEXT("Prototype_Component_VIBuffer_Cube"),
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

HRESULT CSky::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);

	return S_OK;
}

HRESULT CSky::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	return S_OK;
}

CSky * CSky::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSky*		pInstance = new CSky(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSky"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CSky::Clone(void * pArg)
{
	CSky*		pInstance = new CSky(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CSky"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSky::Free()
{
	__super::Free();

	

	Safe_Release(m_pTransformCom);

	Safe_Release(m_pVIBufferCom);

	Safe_Release(m_pTextureCom);
}
