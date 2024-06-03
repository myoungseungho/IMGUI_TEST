#pragma once

#include "Client_Defines.h"
#include "Level_UI.h"
#include "LandObject.h"

BEGIN(Client)

class CLevel_Edit final : public CLevel_UI
{
private:
	CLevel_Edit(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Edit() = default;

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

	HRESULT Ready_LandObjects();

private:
	void Level_Edit_Start1();
	void Level_Edit_Start2();

private:
	_float m_fElapsedTime = 0.0f; // 경과 시간
	_float m_fTalkElapsedTime = -1.f; // 유효하지 않은 상태로 초기화
	_bool m_bStart1Called = false; // Level_Tacho_Start1 함수 호출 여부
	_bool m_bShakeCalled = false;

	HRESULT ParseInitialize();

public:
	static CLevel_Edit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END