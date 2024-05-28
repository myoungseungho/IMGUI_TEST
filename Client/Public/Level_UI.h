#pragma once

#include "Client_Defines.h"
#include "Level.h"

#include "LandObject.h"

BEGIN(Client)

class CLevel_UI abstract : public CLevel
{
protected:
	CLevel_UI(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_UI() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	_bool Key_Down(_uint _iKey);
	HRESULT Ready_Layer_Inventory(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Npc_Talk(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Map_Guide(const _wstring& strLayerTag);

private:
	_bool m_bKeyState[VK_MAX];
public:
	virtual void Free() override;
};

END