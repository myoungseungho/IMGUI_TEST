#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Client)

class CUIObject abstract : public CGameObject
{
protected:
	CUIObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUIObject(const CUIObject& rhs);
	virtual ~CUIObject() = default;

protected:
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

protected:
	void BillBoarding();
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

protected:
	_float offsetX = { 0.f };
	_float offsetY = { 0.f };
	_float offsetZ = { 12.f };

};

END