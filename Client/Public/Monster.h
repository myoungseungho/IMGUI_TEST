#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer_Rect;
class CTransform;

END

BEGIN(Client)

class CMonster abstract : public CGameObject
{
protected:
	enum MON_STATE { IDLE, WALK, ATTACK };

public:
	typedef struct 
	{
		_uint iHp = {0};
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
	virtual HRESULT Ready_Component();

protected:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

	MON_STATE m_eMon_State;

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END