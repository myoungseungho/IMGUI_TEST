#pragma once

#include "GameInstance.h"
#include "Effect.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CCalc_Timer;
class CAnimator;

END

BEGIN(Client)

class CEffect_Monster abstract :public CEffect
{
public:
	typedef struct
	{
		CTransform* pTargetTransform = { nullptr };
	}EFFECT_MONSTER__DESC;

protected:
	CEffect_Monster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEffect_Monster(const CEffect_Monster& Prototype);
	virtual ~CEffect_Monster() = default;

public:
	virtual HRESULT Initialize_Prototype()					override;
	virtual HRESULT Initialize(void* pArg)					override;

public:
	virtual HRESULT Ready_Components();

protected:
	EFFECT_MONSTER__DESC m_tEffect_Desc = {};

protected:
	CTexture* m_pTextureCom = { nullptr };
	CAnimator* m_pAnimCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCalc_Timer* m_pTimerCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	CTransform* m_pTargetTransform = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END