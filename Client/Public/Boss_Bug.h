#pragma once

#include "Client_Defines.h"
#include "Monster.h"

#include "Skill_Bug_Bullet.h"

BEGIN(Client)

class CBoss_Bug final :public CMonster
{
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

	void Warf(_int iPosX , _int iPosZ, _float fDistance, _float fAngle);
	void Skill_Dash(_float fTimeDelta);
	void Fly(_float fTimeDelta);
	void Land(_int iPosX , _int iPosZ, _float fTimeDelta);

	HRESULT Desh_Stop(_float fTimeDelta);
	HRESULT Turtle_Create();
	HRESULT Bullet_Create();

private:
	void Hp_State(_float fTimeDelta);

private:
	CTransform* m_pTargetTransform = { nullptr };
	_float m_fAngle =	{ 0.f };
	_bool m_isTurtleDead = { false };
	_bool m_isDesh = { false };
	_bool m_isUp = { false };
	_bool m_iLand = { false };
	_bool m_iCon = { false };
public:
	static CBoss_Bug* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

