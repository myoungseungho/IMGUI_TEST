#pragma once

#include "Skill_Monster.h"

BEGIN(Client)

class CSkill_Koofu_Rolling final: public CSkill_Monster
{
private:
	CSkill_Koofu_Rolling(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkill_Koofu_Rolling(const CSkill_Koofu_Rolling& pPrototype);
	virtual ~CSkill_Koofu_Rolling() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CSkill_Koofu_Rolling* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

