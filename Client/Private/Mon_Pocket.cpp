#include "stdafx.h"
#include "..\Public\Mon_Pocket.h"

#include "GameInstance.h"

CMon_Pocket::CMon_Pocket(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CMon_Pocket::CMon_Pocket(const CMon_Pocket & Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CMon_Pocket::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CMon_Pocket::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	MON_POCKET_DESC*		pDesc = static_cast<MON_POCKET_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	Safe_AddRef(m_pTargetTransform);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(3.0f, 0.f, 0.f));
	
	return S_OK;
}

void CMon_Pocket::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CMon_Pocket::Update(_float fTimeDelta)
{
	m_pTransformCom->LookAt(m_pTargetTransform->Get_State(CTransform::STATE_POSITION));
	m_pTransformCom->Chase(m_pTargetTransform->Get_State(CTransform::STATE_POSITION), fTimeDelta, 1.f);
}

void CMon_Pocket::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CMon_Pocket::Render(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	/* 로컬 정점을 변환하기위한 상태행렬(월드, 뷰, 투영)들을 셋팅한다. 가라!!!!!!! */
	_float4x4		ViewMatrix, ProjMatrix;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CMon_Pocket::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
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

CMon_Pocket * CMon_Pocket::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMon_Pocket*		pInstance = new CMon_Pocket(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMon_Pocket::Clone(void * pArg)
{
	CMon_Pocket*		pInstance = new CMon_Pocket(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMon_Pocket::Free()
{
	__super::Free();

	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
