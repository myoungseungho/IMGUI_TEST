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

class CUn_Small_Orb final : public CEnviormentObject
{	
public:
	typedef struct
	{
		CTransform* pTargetTransform = { nullptr };
	}UN_SMALL_ORB_DESC;
	enum SMALLORB_DIR {DIR_DOWN, DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_END};
	enum COLLISION_LAZER {STATE_COL, STATE_NOTCOL, STATE_END};
private:
	CUn_Small_Orb(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CUn_Small_Orb(const CUn_Small_Orb& Prototype); /* 사본생성 시 */
	virtual ~CUn_Small_Orb() = default;

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


private:	
	CTexture*				m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CCalc_Timer*			m_pTimerCom = { nullptr };
	CCollider*				m_pColliderCom = { nullptr };

	CTransform*			m_pTargetTransform = { nullptr };
private:
	HRESULT Ready_Components();
	_bool	bTest = { true };

public:

private:
	SMALLORB_DIR			m_eDirection = { DIR_END };
	SMALLORB_DIR			m_ePreDirection = { DIR_END };
	COLLISION_LAZER		m_eCollisionLazer = { STATE_END };
public:
	/* 원형객체를 생성한다. */
	static CUn_Small_Orb* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END