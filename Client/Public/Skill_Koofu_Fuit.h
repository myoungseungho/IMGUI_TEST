#pragma once

#include "Skill_Monster.h"

class CSkill_Koofu_Fuit final: public CSkill_Monster
{
public:
	typedef struct :public SKILL_MONSTER__DESC
	{
		CTransform* pPlayerTransform = { nullptr };
	}SKILL_FUIT_DESC;

private:
	CSkill_Koofu_Fuit(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkill_Koofu_Fuit(const CSkill_Koofu_Fuit& Prototype);
	virtual ~CSkill_Koofu_Fuit() = default;


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
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

private:
	void Bounce(_float _LandPosY);
	void Distroy();
	void BillBoarding();

private:
	CTransform* m_pPlayerTransform = { nullptr };

	_uint m_iBounceCnt = { 0 };
public:
	static CSkill_Koofu_Fuit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

