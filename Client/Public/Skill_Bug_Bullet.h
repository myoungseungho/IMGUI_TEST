#pragma once

#include "Skill_Monster.h"

BEGIN(Client)

class CSkill_Bug_Bullet final:public CSkill_Monster
{
public:
	typedef struct
	{
		CTransform* pTargetTransform = { nullptr };
		_int  iBulletCnt = { 0 };
	}SKILL_BUG_BULLET_DESC;

private:
	CSkill_Bug_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkill_Bug_Bullet(const CSkill_Bug_Bullet& Prototype);
	virtual ~CSkill_Bug_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	CTransform* m_pTargetTransform = { nullptr };
	
public:
	static CSkill_Bug_Bullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END