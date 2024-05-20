#pragma once

#include"Skill_Monster.h"

BEGIN(Client)

class CSkill_Koofu_Bubble final:public CSkill_Monster
{
private:
	CSkill_Koofu_Bubble(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkill_Koofu_Bubble(const CSkill_Koofu_Bubble& pPrototype);
	virtual ~CSkill_Koofu_Bubble() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

protected:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_Animation();

private:
	void Destroy(_float fTimeDelta);

private:
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();


public:
	static CSkill_Koofu_Bubble* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END