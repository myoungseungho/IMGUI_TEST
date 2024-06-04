#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "BlendObject.h"
#include "Effect_Monster.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CCalc_Timer;
class CKeyState;
class CAnimator;
class CCollider;

END

BEGIN(Client)

class CMonster abstract : public CBlendObject
{
protected:
	enum class MON_STATE { IDLE, MOVE, ATTACK, DASH , READY, BULLET , BULLET_B,BULLET_C, FLY, LAND, STUN,HIT , REGEN,CAST, DEATH,MON_END };
	enum class MON_DIR { DIR_D, DIR_L, DIR_LD, DIR_LU, DIR_R, DIR_RD, DIR_RU, DIR_U, DIR_END };
public:
	typedef struct : public CLandObject::LANDOBJECT_DESC
	{
		_uint iMaxHp = { 0 };
		_uint iCurrentHp = { 0 };
		_uint iAttack = { 0 };
		_uint iSpawnNum = { 0 };
		CTransform* pTargetTransform = { nullptr };
	}MONSTER_DESC;

protected:
	CMonster(LPDIRECT3DDEVICE9 _pGraphicDevice);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype()					override;
	virtual HRESULT Initialize(void* pArg)					override;

protected:
	virtual HRESULT Ready_Components();

protected:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCalc_Timer* m_pTimerCom = { nullptr };
	CKeyState* m_pKeyCom = { nullptr };
	CAnimator* m_pAnimCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	CTransform* m_pPlayerTransform = { nullptr };

public:
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

public:
	_bool HitCheck() { return m_bHitCheck; }

public:
	void Move_Dir(_float _floatTimeDelta);
public:
	virtual void Damaged() {
		--m_tMonsterDesc.iCurrentHp;
		m_bHitCheck = true;

		CEffect_Monster::EFFECT_MONSTER__DESC	Desc{};

		Desc.pTargetTransform = m_pTransformCom;
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_SNOW, TEXT("Prototype_GameObject_Effect_Hit"), TEXT("Layer_Hit"), &Desc);
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BUG, TEXT("Prototype_GameObject_Effect_Hit"), TEXT("Layer_Hit"), &Desc);
			
		m_pGameInstance->Play_Sound(L"SFX_BearWhiteGuard_Hit", LEVEL_STATIC, false);
	};

	virtual void Set_Dead()
	{
		if (m_tMonsterDesc.iCurrentHp <= 0)
			m_bDead = true;
	};

	virtual _bool Get_Dead() { return m_bDead; }

public:
	MON_STATE m_eMon_State = {};
	MON_DIR	  m_eMon_Dir = {};

public:
	_bool m_bHitCheck = { false };
	_float m_fDirAngle = { 0.f };

protected:
	_bool m_bDead = false;

public:
	MONSTER_DESC m_tMonsterDesc;

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END