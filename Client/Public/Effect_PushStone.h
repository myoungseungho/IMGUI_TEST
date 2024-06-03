#pragma once

#include "Client_Defines.h"
#include "Effect.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollider;
class CCalc_Timer;
class CAnimator;
END

BEGIN(Client)

class CEffect_PushStone final : public CEffect
{	
public:
typedef struct
	{
		CTransform* pTargetTransform = { nullptr };
		_uint	pDirection = { 0 };
	}EFFECT_PUSHSTONE_DESC;

enum DIRECTION { DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFTUP, DIR_RIGHTUP, DIR_RIGHTDOWN, DIR_LEFTDOWN, DIR_END };

private:
	CEffect_PushStone(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CEffect_PushStone(const CEffect_PushStone& Prototype); /* 사본생성 시 */
	virtual ~CEffect_PushStone() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:	
	CTexture*				m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CCollider*				m_pColliderCom = { nullptr };
	CCalc_Timer*			m_pTimerCom = { nullptr };
	CAnimator*				 m_pAnimCom = { nullptr };


private:
	HRESULT		Ready_Components();

public:
	_bool		m_MakeOnce = { true };

private:
	CTransform* m_pTargetTransform = { nullptr };
	_uint				m_iDirection = { 0 };

	void			Move_Dust(_float fTimeDelta);

public:
	/* 원형객체를 생성한다. */
	static CEffect_PushStone* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END