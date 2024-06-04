#pragma once

#include "Client_Defines.h"
#include "Monster.h"

#include "Skill_Bug_Bullet.h"

BEGIN(Client)

class CBoss_Bug final :public CMonster
{
private:

public:
	typedef struct :public CMonster::MONSTER_DESC
	{
		CTransform* pTargetTransform = { nullptr };
	}BOSS_BUG_DESC;

private:
	CBoss_Bug(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBoss_Bug(const CBoss_Bug& Prototype);
	virtual ~CBoss_Bug() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Animation();

	virtual HRESULT Begin_RenderState();
	virtual HRESULT End_RenderState();

public:
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

private:
	void Warf(_int iPosX, _int iPosZ, _float fDistance, _float fAngle);
	void Skill_Dash(_float fTimeDelta);
	void Fly(_float fTimeDelta);
	void Land(_int iPosX, _int iPosZ, _float fTimeDelta);

private:
	HRESULT Turtle_Create();
	HRESULT Bullet_Create(_uint iBulletNum , CSkill_Bug_Bullet::BULLET_STATE iBulletType);
	HRESULT Wave_Create();

private:
	virtual void Mon_State(_float fTimeDelta);
	virtual void Mon_AnimState(_float _fTimeDelta);

	void State_Idle(_float _fTimeDelta);
	void State_Dash(_float _fTimeDelta);
	void State_Ready(_float _fTimeDelta);
	void State_Bullet(_float _fTimeDelta);
	void State_Fly(_float _fTimeDelta);
	void State_Land(_float _fTimeDelta);
	void State_Regen(_float _fTimeDelta);
	void State_Stan(_float fTimeDelta);
	void State_Death(_float fTimeDelta);

private:
	_float m_fAngle = { 0.f };
	_int m_iBulletCnt = { 0 };
	_bool m_isTurtle = { false };
	_bool m_isFlyEnd = { false };
	_bool m_isLand = { false };
	_bool m_isReady = { false };
	_int m_iPhaseCnt = { 1 };

	_float m_fDashBulletTimer = { 0.f };
	_float m_fWaveTimer = { 0.f };

	_bool m_bPosRange = {true};
	_bool m_bStartDash = { false };

	_bool m_bDown = { false };

	MON_STATE m_ePrev_State = {};

	_bool m_bPlayerAttack = { false };

	_float m_fDashEffectTimer = { 0.f };

	_float m_fDashTimer = { 0.f };

	_float m_fHitTimer = { 0.f };

	_float m_fAlphaTimer = { 0.f };
	_float m_fAlpha = { 0.f };

	_float m_fCameraTimer = { 0.f };
	_bool m_bCameraStop = { false };
public:
	static CBoss_Bug* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

