#pragma once

#include "Client_Defines.h"
#include "Level_UI.h"


#include "LandObject.h"


BEGIN(Client)

class CLevel_Bug final : public CLevel_UI
{
private:
	CLevel_Bug(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Bug() = default;

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
	HRESULT Ready_Layer_Tile(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles);
	HRESULT Ready_Layer_Bush(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles);
	HRESULT Ready_Layer_Boss_Bug(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Effect_Light(const _wstring& strLayerTag);
	HRESULT Ready_Layer_TravelNpc(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Boss_Bug_Hp_UI(const _wstring& strLayerTag);

	HRESULT Ready_LandObjects();

	void Font_Initialize();

	HRESULT ParseInitialize();

private:
	LPD3DXFONT			m_pBoss_Font = { nullptr };
	_float m_TextPosX = { -57.f };
	_float m_TextPosY = { -43.f };
	_float m_ElapsedTime = 0.0f;
	_float m_DisplayDelay = 10.f;
	_bool m_bBossHpUILayerReady = false; // 플래그 변수 추가

public:
	static CLevel_Bug* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END