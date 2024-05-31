 
 

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

	_float3 targetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	m_vInitialOffset = _float3(0.f, 7.5f, -10.f); // Fixed offset
	m_vCurrentOffset = m_vInitialOffset;
	m_vPosition = targetPos + m_vInitialOffset;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_vPosition);
	m_pTransformCom->LookAt(targetPos);

	m_fFovy = D3DXToRadian(45.0f);
	m_fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;

	GetCursorPos(&m_OldMousePos);

	Ready_UI_Layer((LEVELID)m_pGameInstance->GetLoadingLevelIndex());

	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
}

void CCamera::Update(_float fTimeDelta)
{
	Key_Input(fTimeDelta);

	if (m_bIsMovingToTarget || m_bIsReturning)
	{
		UpdateCameraPosition(fTimeDelta);
	}
	else
	{
		UpdateCameraPositionToTarget();
	}

	Bind_PipeLines();
}

void CCamera::UpdateCameraPositionToTarget()
{
	if (m_bIsZoomCompleted)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_vPosition);
	}
	else
	{
		_float3 targetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
		m_vPosition = targetPos + m_vCurrentOffset;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_vPosition);
	}
}

void CCamera::UpdateCameraPosition(_float fTimeDelta)
{
	m_fElapsedTime += fTimeDelta;
	_float t = m_fElapsedTime / m_fDuration;

	// 매 프레임 타겟의 위치를 가져옵니다.
	_float3 targetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);

	// 확대 또는 축소 방향 벡터 계산
	_float3 direction;
	if (m_bIsMovingToTarget)
	{
		direction = targetPos - m_vInitialPosition;  // 초기 위치로부터 타겟 위치로 향하는 벡터
	}
	else if (m_bIsReturning)
	{
		direction = m_vInitialPosition - targetPos;  // 타겟 위치로부터 초기 위치로 향하는 벡터
	}
	D3DXVec3Normalize(&direction, &direction);

	// 방향 벡터를 기반으로 오프셋 조정
	_float3 offset = direction * m_fDistance;

	// 목표 위치 계산
	_float3 goalPosition;
	if (m_bIsMovingToTarget)
	{
		goalPosition = targetPos + offset;
	}
	else if (m_bIsReturning)
	{
		goalPosition = m_vInitialPosition;  // 축소 시 초기 위치로 돌아갑니다.
	}

	if (t >= 1.0f)
	{
		t = 1.0f;
		if (m_bIsMovingToTarget)
		{
			m_vPosition = goalPosition;
			m_bIsZoomCompleted = true;
			m_bIsMovingToTarget = false;
		}
		else if (m_bIsReturning)
		{
			m_vPosition = goalPosition;
			m_bIsReturning = false;
		}
	}
	else
	{
		_float3 vCurrentPosition = Lerp(m_vPosition, goalPosition, t);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vCurrentPosition);
	}
}

void CCamera::ReturnToOriginalPosition(_float fDuration)
{
	m_fDuration = fDuration;
	m_fElapsedTime = 0.0f;
	m_bIsMovingToTarget = false;
	m_bIsReturning = true;
	m_bIsZoomCompleted = false;
}



void CCamera::MoveToTarget(_float fDuration, _float fDistance)
{
	m_vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vInitialPosition = m_vPosition;
	m_fDistance = fDistance;
	m_fDuration = fDuration;
	m_fElapsedTime = 0.0f;
	m_bIsMovingToTarget = true;
	m_bIsReturning = false;
	m_bIsZoomCompleted = false;
}


_float3 CCamera::Lerp(const _float3& start, const _float3& end, _float t)
{
	return _float3{
		start.x + t * (end.x - start.x),
		start.y + t * (end.y - start.y),
		start.z + t * (end.z - start.z)
	};
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
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	long MouseMoveX = {}, MouseMoveY = {};

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

HRESULT CCamera::Ready_UI_Layer(LEVELID m_eNextLevelID)
{
	UIDATA slotData{};

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(m_eNextLevelID, TEXT("Prototype_GameObject_UI_Heart_Player"), TEXT("Layer_UI_Heart_Player"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(m_eNextLevelID, TEXT("Prototype_GameObject_UI_Icon_KeyBoard"), TEXT("Layer_UI_Icon_KeyBoard"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(m_eNextLevelID, TEXT("Prototype_GameObject_UI_HP_Player"), TEXT("Layer_UI_HP_Player"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(m_eNextLevelID, TEXT("Prototype_GameObject_UI_HP_Green_Player"), TEXT("Layer_UI_HP_Green_Player"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(m_eNextLevelID, TEXT("Prototype_GameObject_UI_HP_GlueEffect_Player"), TEXT("Layer_UI_HP_ZEffect_GlueEffect_Player"))))
		return E_FAIL;

	for (size_t i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			slotData.position = { -530.f, 250.f };
			slotData.scale = { 50.f, 50.f };
			break;
		case 1:
			slotData.position = { -480.f, 250.f };
			slotData.scale = { 50.f, 50.f };
			break;
		case 2:
			slotData.position = { -430.f, 250.f };
			slotData.scale = { 50.f, 50.f };
			break;
		case 3:
			slotData.position = { -380.f, 250.f };
			slotData.scale = { 50.f, 50.f };
			break;
		}

		slotData.alpha = 255.f;
		slotData.index = i;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(m_eNextLevelID, TEXT("Prototype_GameObject_UI_PlayerEquipSlot"), TEXT("Layer_UI_PlayerEquipSlot"), &slotData)))
			return E_FAIL;
	}

	for (size_t i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			slotData.position = { -530.f, 250.f };
			slotData.scale = { 50.f, 50.f };
			break;
		case 1:
			slotData.position = { -480.f, 250.f };
			slotData.scale = { 50.f, 50.f };
			break;
		case 2:
			slotData.position = { -430.f, 250.f };
			slotData.scale = { 50.f, 50.f };
			break;
		case 3:
			slotData.position = { -380.f, 250.f };
			slotData.scale = { 50.f, 50.f };
			break;
		}

		slotData.alpha = 57.f;
		slotData.index = i;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(m_eNextLevelID, TEXT("Prototype_GameObject_UI_PlayerEquipSlot_BackGround"), TEXT("Layer_UI_PlayerEquipSlot_BackGround"), &slotData)))
			return E_FAIL;
	}
}

HRESULT CCamera::Ready_Components()
{
	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 10.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(60.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_KeyState */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Key"),
		TEXT("Com_KeyState"), reinterpret_cast<CComponent**>(&m_pKeyCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CCamera::Bind_PipeLines()
{
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransformCom->Get_WorldMatrixInverse());

	_float4x4 ProjMatrix = {};
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
