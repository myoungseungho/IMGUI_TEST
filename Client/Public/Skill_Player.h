#pragma once

#include "Skill.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CCalc_Timer;
class CCollider;
class CKeyState;
END

BEGIN(Client)

class CSkill_Player final:public CSkill
{
public:
	typedef struct
	{
		CTransform* pTargetTransform = { nullptr};
		_uint m_iCurrentSkillCount = { 0 };
		_float3  m_SkillDir = { 0.f, 0.f, 0.f };
	}SKILL_PLAYER_DESC;

private:
	CSkill_Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkill_Player(const CSkill_Player& Prototype);
	virtual ~CSkill_Player() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:

private:
	_uint		m_iSkillCount = { 0 };
	_float3		m_SkillDir = { 0.f, 0.f, 0.f };


protected:
	virtual HRESULT Ready_Components();

private:
	CTransform* m_pTargetTransform = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CKeyState* m_pKeyCom = { nullptr };
	CCalc_Timer* m_pTimerCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };


private:
	_bool		m_bSkillAttack = { false};
public:
	static CSkill_Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END