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

	_float3 m_vPosition = {};
	_float3 m_vInitialPosition = {};

	_float3 m_vInitialOffset = {};
	_float3 m_vCurrentOffset = {};


	_float m_fElapsedTime = {};
	_float m_fDuration = {};
	_float m_fDistance = {};

	_bool m_bIsMovingToTarget = { false };
	_bool m_bIsZoomCompleted = { false };
	_bool m_bIsReturning = { false };

	//Ω¶¿Ã≈∑
	_bool m_bIsShaking = { false };
	_float m_fShakeDuration = { 0.0f };
	_float m_fShakeElapsedTime = { 0.0f };
	_float m_fShakeMagnitude = { 0.0f };
	_float m_fShakeSpeed = { 0.0f };
	_float3 m_vOriginalPosition = {};

	//¡¯øµ
	_bool m_bCameraStop = { false };
	_float m_fTimer = { 0.f };
	_bool m_bEndStop = { false };
public:
	void MoveToTarget(_float fDuration, _float fDistance);
	void ReturnToOriginalPosition(_float fDuration);

private:
	void UpdateCameraPosition(_float fTimeDelta);
	void UpdateCameraPositionToTarget();
	_float3 Lerp(const _float3& start, const _float3& end, _float t);

	void UpdateShake(_float fTimeDelta);

public:
	void ShakeCamera(_float fDuration, _float fMagnitude, _float fSpeed);
	void Circle_Moving(_float3 vPos , _float fDistance , _float fAngle , _float Timer, _float fTimerDelta);

public:
	HRESULT Bind_PipeLines();

public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
