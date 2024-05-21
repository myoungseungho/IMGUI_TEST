#include "stdafx.h"
#include "..\Public\Camera.h"

#include "GameInstance.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CCamera::CCamera(const CCamera& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CCamera::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	Safe_AddRef(m_pTargetTransform);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 카메라가 내 월드 공간에 어디에 존재하는지. */
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(0.f, 10.f, -5.f));
	//m_pTransformCom->LookAt(_float3(0.f, 0.f, 0.f));	


	_float Targetx = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).x;
	_float Targety = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).y;
	_float Targetz = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).z;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(Targetx, Targety + 5.f, Targetz - 10.f));
	m_pTransformCom->LookAt(_float3(m_pTargetTransform->Get_State(CTransform::STATE_POSITION)));

	m_fFovy = D3DXToRadian(60.0f);
	m_fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;


	//GetCursorPos(&m_OldMousePos);

	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CCamera::Update(_float fTimeDelta)
{
	//Key_Input(fTimeDelta);

	_float Targetx = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).x;
	_float Targety = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).y;
	_float Targetz = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).z;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(Targetx, Targety + 5.f, Targetz - 10.f));

	Bind_PipeLines();

}

void CCamera::Late_Update(_float fTimeDelta)
{

}

HRESULT CCamera::Render(_float fTimeDelta)
{

	return S_OK;
}

HRESULT CCamera::Key_Input(_float fTimeDelta)
{
	POINT			ptMouse{};
	GetCursorPos(&ptMouse);
	long		MouseMoveX = {}, MouseMoveY = {};

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		if (GetKeyState('W') & 0x8000)
			m_pTransformCom->Go_Straight(fTimeDelta);
		if (GetKeyState('S') & 0x8000)
			m_pTransformCom->Go_Backward(fTimeDelta);
		if (GetKeyState('A') & 0x8000)
			m_pTransformCom->Go_Left(fTimeDelta);
		if (GetKeyState('D') & 0x8000)
			m_pTransformCom->Go_Right(fTimeDelta);
		if (GetKeyState('E') & 0x8000)
			m_pTransformCom->Go_Up(fTimeDelta);
		if (GetKeyState('Q') & 0x8000)
			m_pTransformCom->Go_Down(fTimeDelta);

		if (MouseMoveX = ptMouse.x - m_OldMousePos.x)
		{
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMoveX * m_fMouseSensor);
		}

		if (MouseMoveY = ptMouse.y - m_OldMousePos.y)
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMoveY * m_fMouseSensor);
		}
	}

	m_OldMousePos = ptMouse;

	return S_OK;
}

HRESULT CCamera::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 10.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(60.0f);


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

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera* pInstance = new CCamera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CCamera::Clone(void* pArg)
{
	CCamera* pInstance = new CCamera(*this);

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

	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pKeyCom);
}
