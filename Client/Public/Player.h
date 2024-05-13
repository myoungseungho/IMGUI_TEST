#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CComponent;
class CVIBuffer_Rect;
class CVIBuffer_Cube;
class CKeyState;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{	
private:
enum DIRECTION {DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFTUP, DIR_RIGHTUP, DIR_RIGHTDOWN, DIR_LEFTDOWN, DIR_END};
enum STATE {STATE_IDLE, STATE_ATTACK, STATE_END};

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
	virtual HRESULT Render() override;

private:	
	CTexture*				m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferRectCom = { nullptr };
	CVIBuffer_Cube*		m_pVIBufferCubeCom = { nullptr };
	CKeyState*				m_pKeyCom = { nullptr };

private:
	HRESULT Ready_Components();

private:
	HRESULT			Key_Input(_float fTimeDelta);

	void					Player_Attack(_float fTimeDelta);
	
	void					Set_Direction(DIRECTION _DIR) { m_PlayerDir = _DIR; }
	void					Set_State(STATE _STATE) { m_PlayerState = _STATE; }
	

private:
	_float fTimeAcc = {0.0f};
	_float3		forScaled;

	DIRECTION	m_PlayerDir = { DIR_END };
	STATE			m_PlayerState = { STATE_END };


public:
	/* 원형객체를 생성한다. */
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END