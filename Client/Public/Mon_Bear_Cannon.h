#pragma once

#include "Monster.h"

BEGIN(Client)

class CMon_Bear_Cannon final :public CMonster
{
public:
	enum class ANIM_STATE { IDLE, MOVE, ATTACK, HIT, STUN, ANIM_END };
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

private:
	ANIM_STATE m_eAnim_State = {};

public:
	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

public:
	static CMon_Bear_Cannon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
