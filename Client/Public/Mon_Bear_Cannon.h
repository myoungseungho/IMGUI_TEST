#pragma once

#include "Monster.h"

BEGIN(Client)

class CMon_Bear_Cannon final :public CMonster
{
public:
	enum class ANIM_STATE { IDLE, ATTACK, STUN, ANIM_END };
private:
	CMon_Bear_Cannon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMon_Bear_Cannon(const CMon_Bear_Cannon& Prototype);
	virtual ~CMon_Bear_Cannon() = default;

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
	void Mon_State(_float fTimeDelta);
	void Anim_State(_float fTimeDelta);

	void State_Idle(_float fTimeDelta);
	void State_Attack(_float fTimeDelta);
	void State_Stun(_float fTimeDelta);

private:
	ANIM_STATE m_eAnim_State = {};
	MON_STATE m_ePrev_State = {};
private:
	_float m_fMoveRange = { 0.f };
	_float m_fAttackRange = { 0.f };
	_float3 m_vTargetDistance = { };

public:
	static CMon_Bear_Cannon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
