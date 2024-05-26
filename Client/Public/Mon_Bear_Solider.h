#pragma once

#include "Monster.h"

BEGIN(Client)

class CMon_Bear_Solider final :public CMonster
{
public:
	enum class ANIM_STATE { IDLE, MOVE,ATTACK,HIT , STUN ,ANIM_END };
private:
	CMon_Bear_Solider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMon_Bear_Solider(const CMon_Bear_Solider& Prototype);
	virtual ~CMon_Bear_Solider() = default;

public:
	virtual HRESULT Initialize_Prototype()					override;
	virtual HRESULT Initialize(void* pArg)					override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Animation();

	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

private:
	void Anim_State(_float fTimeDelta);
	void Mon_State(_float fTimeDelta);

	void State_Idle(_float fTimeDelta);
	void State_Move(_float fTimeDelta);
	void State_Attack(_float fTimeDelta);
	void State_Hit(_float fTimeDelta);
	void State_Stun(_float fTimeDelta);

private:
	void Destroy();

private:
	ANIM_STATE m_eAnim_State = {};
	MON_STATE m_ePrev_State = {};
 private:
	_float m_fMoveRange = { 0.f };
	_float m_fAttackRange = { 0.f };
	_float3 m_vTargetDistance = { };

public:
	static CMon_Bear_Solider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
