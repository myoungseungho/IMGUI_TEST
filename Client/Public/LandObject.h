#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Client)

class CLandObject abstract : public CGameObject
{
public:
	typedef struct
	{
		CTransform*			m_pTerrainTransform = { nullptr };
		CVIBuffer_Terrain*	m_pTerrainBuffer = { nullptr };
	}LANDOBJECT_DESC;

protected:
	CLandObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLandObject(const CLandObject& rhs);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void SetUp_OnTerrain(class CTransform* pTargetTransform, _float fOffset = 0.0f);

private:
	CTransform*			m_pTerrainTransform = { nullptr };
	CVIBuffer_Terrain*	m_pTerrainBuffer = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END