#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Client)

class CEnviormentObject abstract : public CGameObject
{
protected:
	CEnviormentObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnviormentObject(const CEnviormentObject& rhs);
	virtual ~CEnviormentObject() = default;

protected:
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();
	HRESULT Begin_Blend_RenderState();
	HRESULT End_Blend_RenderState();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END