#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CBlendObject abstract : public CGameObject
{
protected:
	CBlendObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlendObject(const CBlendObject& Prototype);
	virtual ~CBlendObject() = default;

public:
	_float Get_ViewZ() const {
		return m_fViewSpaceZ;
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	_float				m_fViewSpaceZ = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END