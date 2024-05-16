#pragma once

#include "Skill.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CCalc_Timer;
class CCollider;

END

BEGIN(Client)

class CSkill_Player final:public CSkill
{
public:
	typedef struct
	{
		CTransform* pTargetTransform = { nullptr};
	}SKILL_PLAYER_DESC;

private:
	CSkill_Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkill_Player(const CSkill_Player& Prototype);
	virtual ~CSkill_Player() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other);
	virtual void OnCollisionExit(class CCollider* other);

public:


protected:
	virtual HRESULT Ready_Components();

private:
	CTransform* m_pTargetTransform = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCalc_Timer* m_pTimerCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CCalc_Timer* m_pCal_Timercom = { nullptr };

public:
	static CSkill_Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END