#pragma once

#include "Monster.h"

BEGIN(Client)

class CMon_Trash_Slime final:public CMonster
{
public:
	enum class ANIM_STATE { IDLE, MOVE  , ANIM_END};
private:
	CMon_Trash_Slime(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMon_Trash_Slime(const CMon_Trash_Slime& Prototype);
	virtual ~CMon_Trash_Slime() = default;

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
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

private:
	void Anim_State(_float fTimeDelta);
	void Mon_State(_float fTimeDelta);

	void State_Idle(_float fTimeDelta);
	void State_Move(_float fTimeDelta);

private:
	void Move(_float fTimeDelta);
	void Destory();
private:
	ANIM_STATE m_eAnim_State = {};

private:
	_float m_fAttackRange = { 0.f };
	_float3 m_vTargetDistance = { };

	_float m_fAlphaTimer = { 0.f };
	_float m_fAlpha = { 255.f };

	_bool m_bMoveStop = { false };
	_bool m_bHit = { false };

	_float m_fMovetTimer = { 0.f };
public:
	static CMon_Trash_Slime* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END