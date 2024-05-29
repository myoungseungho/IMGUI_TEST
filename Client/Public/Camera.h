#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CKeyState;
END

BEGIN(Client)

class CCamera final : public CGameObject
{
public:
    typedef struct
    {
        CTransform* pTargetTransform = { nullptr };
    } CAMERA_DESC;

private:
    CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
    CCamera(const CCamera& Prototype);
    virtual ~CCamera() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Update(_float fTimeDelta) override;
    virtual void Update(_float fTimeDelta) override;
    virtual void Late_Update(_float fTimeDelta) override;
    virtual HRESULT Render(_float fTimeDelta) override;

private:
    HRESULT Ready_UI_Layer(LEVELID m_eNextLevelID);
    HRESULT Ready_Components();
    HRESULT Key_Input(_float fTimeDelta);


private:
    CTransform* m_pTransformCom = { nullptr };
    CTransform* m_pTargetTransform = { nullptr };
    CKeyState* m_pKeyCom = { nullptr };

    _float m_fFovy = {};
    _float m_fAspect = {};
    _float m_fNear = {};
    _float m_fFar = {};

    POINT m_OldMousePos = {};
    _float m_fMouseSensor = { 0.2f };

    _float3 m_vInitialPosition = {};
    _float3 m_vOriginalPosition = {};
    _float3 m_vTargetPosition = {};
    _float3 m_vInitialOffset = {};
    _float3 m_vCurrentOffset = {};
    _float3 m_vTargetOffset = {};

    _float m_fElapsedTime = {};
    _float m_fDuration = {};

    _bool m_bIsMovingToTarget = { false };
    _bool m_bIsReturning = { false };
    _bool m_bAtTargetPosition = { false };

public:
    void MoveToTarget(_float fDuration, _float fDistance);
    void ReturnToOriginalPosition(_float fDuration);

private:
    void UpdateCameraPosition(_float fTimeDelta);
    void UpdateCameraPositionToTarget();
    _float3 Lerp(const _float3& start, const _float3& end, _float t);

public:
    HRESULT Bind_PipeLines();

public:
    static CCamera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg = nullptr) override;
    virtual void Free() override;
};

END
