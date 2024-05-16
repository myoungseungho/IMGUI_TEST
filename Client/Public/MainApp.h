#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

private:
	CGameInstance* m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };

private:
	HRESULT SetUp_DefaultState();
	HRESULT Open_Level(LEVELID eLevelID);

	HRESULT Ready_Prototype_Components();
private:
	HRESULT ShowLevels();
	HRESULT Show_PrototypeObjects();
	HRESULT SpawnObjectAtZero(const std::string& type);
	HRESULT Show_LayerObjects();
	HRESULT Save_Button_Pressed(bool*, bool*);
	HRESULT Load_Button_Pressed();
	HRESULT Click_Collider_Toggle(bool isChecked);
	HRESULT OnCollisionCheckIntervalChanged(float);
public:
	static CMainApp* Create();
	virtual void Free() override;
};

END