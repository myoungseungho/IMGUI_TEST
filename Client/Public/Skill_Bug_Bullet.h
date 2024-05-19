#pragma once

#include "Skill_Monster.h"

BEGIN(Client)

class CSkill_Bug_Bullet final:public CSkill_Monster
{
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

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_Animation();

	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

	void Distroy(_float fTimeDelta);
	
public:
	static CSkill_Bug_Bullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END