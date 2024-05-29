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
    m_vInitialOffset = _float3(0.f, 10.f, -10.f); // Fixed offset
    m_vCurrentOffset = m_vInitialOffset;
    m_vInitialPosition = targetPos + m_vInitialOffset;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_vInitialPosition);
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
    _float3 targetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
    _float3 newPos = targetPos + m_vCurrentOffset;
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, &newPos);
}

void CCamera::UpdateCameraPosition(_float fTimeDelta)
{
    m_fElapsedTime += fTimeDelta;
    _float t = m_fElapsedTime / m_fDuration;

    if (t >= 1.0f)
    {
        t = 1.0f;
        if (m_bIsMovingToTarget)
        {
            m_bIsMovingToTarget = false;
            m_bAtTargetPosition = true;
        }
        else if (m_bIsReturning)
        {
            m_bIsReturning = false;
            m_bAtTargetPosition = false;
            m_vCurrentOffset = m_vInitialOffset; // 원래 오프셋으로 복귀
        }
    }

    if (m_bIsMovingToTarget)
    {
        _float3 targetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
        _float3 vCurrentPosition = Lerp(m_vOriginalPosition, targetPos + m_vTargetOffset, t);
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vCurrentPosition);
        m_vCurrentOffset = Lerp(m_vInitialOffset, m_vTargetOffset, t);
    }
    else if (m_bIsReturning)
    {
        _float3 vCurrentPosition = Lerp(m_vTargetPosition, m_vOriginalPosition, t);
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vCurrentPosition);
        m_vCurrentOffset = Lerp(m_vTargetOffset, m_vInitialOffset, t);
    }
}


void CCamera::MoveToTarget(_float fDuration, _float fDistance)
{
    m_vOriginalPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _float3 targetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);

    // 카메라와 타겟 간의 방향 벡터 계산
    _float3 direction = targetPos - m_vOriginalPosition;
    D3DXVec3Normalize(&direction, &direction);

    // 방향 벡터를 기반으로 오프셋 조정
    m_vTargetOffset = direction * fDistance;

    m_vTargetPosition = targetPos + m_vTargetOffset;

    m_fElapsedTime = 0.0f;
    m_fDuration = fDuration;
    m_bIsMovingToTarget = true;
    m_bIsReturning = false;
    m_bAtTargetPosition = false;
}


void CCamera::ReturnToOriginalPosition(_float fDuration)
{
    m_fElapsedTime = 0.0f;
    m_fDuration = fDuration;
    m_bIsMovingToTarget = false;
    m_bIsReturning = true;
    m_bAtTargetPosition = false;
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
