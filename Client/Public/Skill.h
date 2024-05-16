#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CSkill abstract :public CGameObject
{
protected:
	CSkill(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkill(const CSkill& Prototype);
	virtual ~CSkill() = default;

public:
	virtual HRESULT Initialize_Prototype()					override;
	virtual HRESULT Initialize(void* pArg)					override;



public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END