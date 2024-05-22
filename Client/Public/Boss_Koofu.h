#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)

class CBoss_Koofu final :public CMonster
{
private:
	enum class ANIM_STATE {IDLE ,WALK , CAST , READY , STUN ,THROW , DEADTH ,ANIM_END };
public:
	typedef struct:public MONSTER_DESC
	{
		CTransform* m_pTargetTransform = {nullptr};
		_bool isClone = { false };
	
	}BOSS_KOOFU_DESC;

private:
	CBoss_Koofu(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBoss_Koofu(const CBoss_Koofu& Prototype);
	virtual ~CBoss_Koofu() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	void MonState(_float fTimeDelta);
	void AnimState(_float fTimeDelta);

	void State_Idle(_float fTimeDelta);
	void State_Ready(_float fTimeDelta);
	void State_Bullet(_float fTimeDelta);
	void State_Bullet_B(_float fTimeDelta);
	void State_Bullet_C(_float fTimeDelta);
	void State_Stan(_float fTimeDelta);
	void State_Cast(_float fTimeDelta);

private:
	void Move_Dir();
	void Move();

	void Key_Input(_float fTimeDelta);
	void BillBoarding();

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Animation();

	virtual HRESULT Begin_RenderState();
	virtual HRESULT End_RenderState();


public:
	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other);
	virtual void OnCollisionExit(class CCollider* other);

private:

	void ScaleUp(_float fTimeDelta);
	void Warf(_int fRangePosX , _int fRangePosZ, _int fMaxPosX, _int fMaxPosZ);
	void Warf(_int iPosX, _int iPosZ, _float fDistance);
	void Destory();

	HRESULT RollingCreate();
	HRESULT FuitCreate();
	HRESULT CloneCreate();
	HRESULT CircleCreate();

private:
	ANIM_STATE m_eAnim_State = {};
	MON_STATE m_ePrev_State = {};
	CTransform* m_pTargetTransform = { nullptr };

private:
	_bool m_isClone = { false };
	_bool m_isClone_Create = { false };

	_bool m_isAttack = { false };
	_bool m_isBullet = { false };

	_float3 m_vTargetDir = {};

public:
	static CBoss_Koofu* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

