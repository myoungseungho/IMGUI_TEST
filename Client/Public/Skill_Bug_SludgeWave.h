#pragma once


#include"Skill_Monster.h"

BEGIN(Client)

class CSkill_Bug_SludgeWave :public CSkill_Monster
{
private:
	CSkill_Bug_SludgeWave(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkill_Bug_SludgeWave(const CSkill_Bug_SludgeWave& pPrototype);
	virtual ~CSkill_Bug_SludgeWave() = default;

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
	void Create_Position(_float fPosX, _float fPosZ, _float fDistance);
	void Move(_float fTimeDelta);

	void Destroy(_float fTimeDelta);

private:
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();



public:
	static CSkill_Bug_SludgeWave* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END