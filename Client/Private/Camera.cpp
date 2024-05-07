#include "stdafx.h"
#include "..\Public\Camera.h"

#include "GameInstance.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CCamera::CCamera(const CCamera & Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CCamera::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 카메라가 내 월드 공간에 어디에 존재하는지. */
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(0.f, 10.f, -5.f));
	m_pTransformCom->LookAt(_float3(0.f, 0.f, 0.f));	

	m_fFovy = D3DXToRadian(90.0f);
	m_fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;

	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CCamera::Update(_float fTimeDelta)
{
	if (GetKeyState('W') & 0x8000)
		m_pTransformCom->Go_Straight(fTimeDelta);
	if (GetKeyState('S') & 0x8000)
		m_pTransformCom->Go_Backward(fTimeDelta);
	if (GetKeyState('A') & 0x8000)
		m_pTransformCom->Go_Left(fTimeDelta);
	if (GetKeyState('D') & 0x8000)
		m_pTransformCom->Go_Right(fTimeDelta);

	Bind_PipeLines();
}

void CCamera::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	int a = 10;
}

HRESULT CCamera::Render()
{

	return S_OK;
}

HRESULT CCamera::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::Bind_PipeLines()
{

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransformCom->Get_WorldMatrixInverse());

	_float4x4		ProjMatrix = {};

	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_fFovy, m_fAspect, m_fNear, m_fFar));

	return S_OK;
}

CCamera * CCamera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera*		pInstance = new CCamera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCamera::Clone(void * pArg)
{
	CCamera*		pInstance = new CCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera::Free()
{
	__super::Free();

	

	Safe_Release(m_pTransformCom);

	
}
