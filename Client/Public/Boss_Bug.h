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
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Animation();

	virtual HRESULT Begin_RenderState();
	virtual HRESULT End_RenderState();

private:
	void Warf(_int iPosX , _int iPosZ, _float fDistance, _float fAngle);
	void Skill_Dash(_float fTimeDelta);
	void Fly(_float fTimeDelta);
	void Land(_int iPosX , _int iPosZ, _float fTimeDelta);

	HRESULT Turtle_Create();
	HRESULT Bullet_Create();

private:
	void Mon_State(_float fTimeDelta);
	void State_Idle(float _fTimeDelta);
	void State_Dash(float _fTimeDelta);
	void State_Ready(float _fTimeDelta);
	void State_Bullet(float _fTimeDelta);
	void State_Fly(float _fTimeDelta);
	void State_Land(float _fTimeDelta);

private:
	CTransform* m_pTargetTransform = { nullptr };
	_float m_fAngle =	{ 0.f };
	_int m_iBulletCnt = { 0 };
	_bool m_isTurtle = { false };
	_bool m_isFlyEnd = { false };
	_bool m_isLand = { false };

public:
	static CBoss_Bug* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

