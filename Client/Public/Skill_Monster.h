#pragma once

#include "Skill.h"
#include "GameInstance.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CCalc_Timer;

END

BEGIN(Client)

class CSkill_Monster abstract:public CSkill
{
public:
	typedef struct
	{
		CTransform* pTargetTransform = { nullptr };
		_int  iBulletCnt = { 0 };
	}SKILL_MONSTER__DESC;

protected:
	CSkill_Monster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkill_Monster(const CSkill_Monster& Prototype);
	virtual ~CSkill_Monster()= default;

public:
	virtual HRESULT Initialize_Prototype()					override;
	virtual HRESULT Initialize(void* pArg)					override;

public:
	virtual HRESULT Ready_Components();	

protected:
	SKILL_MONSTER__DESC m_tSkill_Desc = {};

	
protected:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCalc_Timer* m_pTimerCom = { nullptr };

	CTransform* m_pTargetTransform= { nullptr };

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END