#pragma once

#include "Client_Defines.h"
#include "LandObject.h"


BEGIN(Engine)

class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CCalc_Timer;
class CKeyState;
class CAnimator;

END

BEGIN(Client)

class CMonster abstract : public CGameObject
{
protected:
	enum class MON_STATE { IDLE, WALK, ATTACK, DASH , READY, BULLET, FLY, LAND, STAN, DEATH, WARF,MON_END };

public:
	typedef struct : public CLandObject::LANDOBJECT_DESC
	{
		_uint iHp = { 0 };
		_uint iAttack = { 0 };
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

protected:
	MON_STATE m_eMon_State = {};
	
protected:
	MONSTER_DESC m_tMonsterDesc = {};

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END