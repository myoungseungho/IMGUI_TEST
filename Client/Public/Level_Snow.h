#pragma once

#include "Client_Defines.h"
#include "Level_UI.h"

#include "LandObject.h"

BEGIN(Client)

class CLevel_Snow final : public CLevel_UI
{
private:
	CLevel_Snow(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Snow() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	/* 내 게임내에 배경을 구성하는 객체들을 추가한다. */
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Sky(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster_Trash_Slime(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster_Bear_Solider(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster_Bear_Cannon(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Tile(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles);
	HRESULT Ready_Layer_Bush(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles);
	HRESULT Ready_Layer_TravelNpc(const _wstring& strLayerTag);

	HRESULT Ready_LandObjects();

	HRESULT ParseInitialize();

	

public:
	static CLevel_Snow* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END