#pragma once

#include "Skill_Monster.h"

BEGIN(Client)

class CSkill_Cannon_Ball final :public CSkill_Monster
{
public:
	typedef struct :public SKILL_MONSTER__DESC
	{
		CTransform* pPlayerTransform = { nullptr };
	}SKILL_CANNON_DESC;

private:
	CSkill_Cannon_Ball(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkill_Cannon_Ball(const CSkill_Cannon_Ball& Prototype);
	virtual ~CSkill_Cannon_Ball() = default;


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

public:
	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

private:
	void Move(_float fTimeDelta);
	void Destory(_float fTimeDelta);

private:
	CTransform* m_pPlayerTransform = { nullptr };

public:
	static CSkill_Cannon_Ball* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

