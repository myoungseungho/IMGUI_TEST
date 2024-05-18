#pragma once

#include "Client_Defines.h"
#include "LandObject.h"



BEGIN(Engine)
class CTexture;
class CTransform;
class CComponent;
class CVIBuffer_Rect;
class CCollider;
class CKeyState;
END

BEGIN(Client)

class CPlayer final : public CLandObject
{	
private:
enum DIRECTION {DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFTUP, DIR_RIGHTUP, DIR_RIGHTDOWN, DIR_LEFTDOWN, DIR_END};
enum STATE {STATE_IDLE, STATE_ATTACK, STATE_SKILL, STATE_PUSH, STATE_END};

private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CPlayer(const CPlayer& Prototype); /* 사본생성 시 */
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other);
	virtual void OnCollisionExit(class CCollider* other);
private:	
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };
	CKeyState*			m_pKeyCom = { nullptr };

private:
	HRESULT Ready_Components();

private:
	HRESULT			Key_Input(_float fTimeDelta);

	void					Player_Attack(_float fTimeDelta);
	void					Player_Skill(_float fTimeDelta);
	
	void					Set_Direction(DIRECTION _DIR) { m_PlayerDir = _DIR; }
	void					Set_State(STATE _STATE) { m_PlayerState = _STATE; }
	

private:
	_float3		m_forScaled;

	DIRECTION	m_PlayerDir = { DIR_END };
	STATE			m_PlayerState = { STATE_END };

private:
	_bool m_bMoveRight = false;
	_bool m_bMoveLeft = false;
	_bool m_bMoveUp = false;
	_bool m_bMoveDown = false;

	_bool m_bCanMoveRight = true;
	_bool m_bCanMoveLeft = true;
	_bool m_bCanMoveForward = true;
	_bool m_bCanMoveBackward = true;

public:
	/* 원형객체를 생성한다. */
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END