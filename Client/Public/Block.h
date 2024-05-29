#pragma once

#include "Client_Defines.h"
#include "EnviormentObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollider;
class CAnimator;
END

BEGIN(Client)

class CBlock final : public CEnviormentObject
{	
private:
	CBlock(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CBlock(const CBlock& Prototype); /* 사본생성 시 */
	virtual ~CBlock() = default;

	enum ANIMATION_STATE {
		ANIM_IDLE,
		ANIM_UNIDLE,
		ANIM_UNBLOCK,
		ANIM_BLOCK
	};

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

private:	
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CAnimator* m_pAnimCom = { nullptr };


private:
	HRESULT Ready_Components();
	HRESULT Ready_Animation();
	void AnimState(_float _fTimeDelta);

private:
	ANIMATION_STATE m_eAnimState = ANIM_IDLE;

public:
	/* 원형객체를 생성한다. */
	static CBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END