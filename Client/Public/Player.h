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
class CCalc_Timer;
class CAnimator;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
enum DIRECTION {DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFTUP, DIR_RIGHTUP, DIR_RIGHTDOWN, DIR_LEFTDOWN, DIR_END};
enum PLAYER_STATE {STATE_IDLE, STATE_WALK, STATE_ATTACK, STATE_SKILL, STATE_PUSH, STATE_HIT, STATE_END};

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
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

public:
	PLAYER_STATE		Get_Player_State() {
		return m_ePlayerCurState;
	}

	_uint		Get_Player_Hp()
	{
		return m_iPlayerHp;
	}

	_uint		Get_Player_MaxHp()
	{
		return m_iMaxHp;
	}

	_uint		Set_Player_Hp(_uint _hp)
	{
		if (m_iPlayerHp + _hp >= m_iMaxHp)
			return m_iPlayerHp = m_iMaxHp;
		else
			return m_iPlayerHp += _hp;
	}

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CKeyState* m_pKeyCom = { nullptr };
	CCalc_Timer* m_pCal_Timercom = { nullptr };
	CAnimator* m_pAnimCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Ready_Animation();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

private:
	HRESULT			Key_Input(_float fTimeDelta);

	void					Player_Attack(_float fTimeDelta);
	HRESULT			Player_Skill();

	void              Set_Direction(DIRECTION _DIR) { m_ePlayerDir = _DIR; }
	void              Set_State(PLAYER_STATE _STATE) { m_ePlayerCurState = _STATE; }

	void				BillBoarding();

	void				 Player_AnimState(_float _fTimeDelta);

	void				For_Attack_State(_float fTimeDelta);
	void				For_Damage_State(_float fTimeDelta);

	void				Player_Damaged();

private:
	_float3		m_forScaled;

	DIRECTION	m_ePlayerDir = { DIR_END };
	PLAYER_STATE			m_ePlayerCurState = { STATE_END };
	PLAYER_STATE			m_ePlayerPreState = { STATE_IDLE };

	_float			m_fAttackTime = { 0.0f };
	_float			m_fDamageTime = { 0.0f };

	_uint			m_iPlayerHp = { 10 };
	_uint			m_iMaxHp = { 10 };

	_bool			m_bCanDamaged = { true };
	_bool			m_bForTestDamaged = { false };

private:
	_bool m_bMoveRight = false;
	_bool m_bMoveLeft = false;
	_bool m_bMoveUp = false;
	_bool m_bMoveDown = false;

	_bool m_bCanMoveRight = true;
	_bool m_bCanMoveLeft = true;
	_bool m_bCanMoveForward = true;
	_bool m_bCanMoveBackward = true;

	_bool m_bPush = false;

	_float		fTimeAcc = { 0.0f };
	_float3		m_SkillDir = { 0.f, 0.f, 0.f };

	_float		m_bAttack = { false };
private:
	_uint m_iCurrentSkillCount = { 0 };

public:

	/* 원형객체를 생성한다. */
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END