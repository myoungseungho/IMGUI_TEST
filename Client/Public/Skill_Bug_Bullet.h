#pragma once

#include "Skill_Monster.h"

BEGIN(Client)

class CSkill_Bug_Bullet final:public CSkill_Monster
{
public:
	enum class BULLET_STATE {NORMAL , CIRCLE , RNADOM, BULLET_END};

public:
	typedef struct :public SKILL_MONSTER__DESC
	{
		BULLET_STATE iBulletType = {  };
	}BULLET_SKILL_DESC;

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

public:
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

private:
	void Destroy(_float fTimeDelta);
private:
	void Bullet_State();

	void BulletType_Normal();
	void BulletType_Circle();
private:
	_float3 m_vCreatePos = {};
	BULLET_STATE m_iBulletType = { };

	_bool m_bPlayerAttack = { false };
	_bool m_bBulletHit = { false };
	_bool m_bHitStop = { false };
	
public:
	static CSkill_Bug_Bullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END