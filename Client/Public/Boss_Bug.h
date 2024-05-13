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
		CSkill_Bug_Bullet* pBullet = { nullptr };
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
	HRESULT  KeyInput(_float fTimeDelta);

	void Dash(_float fTimeDelta);
	void Warf(_int fMaxPosX, _int fMaxPosY , _float fTimeDelta);

private:
	CSkill_Bug_Bullet* m_pBullet = { nullptr };
	CTransform* m_pTargetTransform = { nullptr };
	bool	m_isDash = { false };


public:
	static CBoss_Bug* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

