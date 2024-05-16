#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Clear_Resources();
	_uint GetLevelIndex() { return m_iLevelIndex; };
protected:
	LPDIRECT3DDEVICE9				m_pGraphic_Device = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };
	_uint							m_iLevelIndex = { 0 };

public:
	virtual void Free() override;
};

END