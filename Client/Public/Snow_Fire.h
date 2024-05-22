#pragma once

#include "Client_Defines.h"
#include "EnviormentObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
class CAnimator;
END

BEGIN(Client)

class CSnow_Fire final : public CEnviormentObject
{	
public:
	typedef struct
	{
		_float3 startPosition;
	}BUSHDESC;

private:
	enum ANIMATION_STATE {
		ANIM_IDLE,
	};

private:
	CSnow_Fire(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CSnow_Fire(const CSnow_Fire& Prototype); /* 사본생성 시 */
	virtual ~CSnow_Fire() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:	
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CAnimator* m_pAnimCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Ready_Animation();
	void AnimState(_float _fTimeDelta);

private:
	ANIMATION_STATE m_eAnimState = ANIM_IDLE;

public:
	/* 원형객체를 생성한다. */
	static CSnow_Fire* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END