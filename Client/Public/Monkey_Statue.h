#pragma once

#include "Client_Defines.h"
#include "EnviormentObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollider;
class CCalc_Timer;
END

BEGIN(Client)

class CMonkey_Statue final : public CEnviormentObject
{	
private:
	enum MONKEY_STATE
	{
		STATE_UP,
		STATE_DOWN,
		STATE_END
	};

private:
	CMonkey_Statue(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CMonkey_Statue(const CMonkey_Statue& Prototype); /* 사본생성 시 */
	virtual ~CMonkey_Statue() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

public:
	void		Change_bIsChangeOnceTrue()
	{
		bIsChangeOnce = { true };
	}


private:	
	CTexture*				m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CCalc_Timer*			m_pTimerCom = { nullptr };
	CCollider*				m_pColliderCom = { nullptr };

private:
	HRESULT Ready_Components();

private:
	_float3				PrePos = { 0.f, 0.f, 0.f };
	_bool		bIsChangeOnce = { true };

public:
	/* 원형객체를 생성한다. */
	static CMonkey_Statue* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END