#pragma once

#include "Client_Defines.h"
#include "Level.h"

#include "LandObject.h"

BEGIN(Client)

class CLevel_Koofu final : public CLevel
{
private:
	CLevel_Koofu(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Koofu() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	/* �� ���ӳ��� ����� �����ϴ� ��ü���� �߰��Ѵ�. */
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Sky(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Tile(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles);
	HRESULT Ready_Layer_Bush(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles);
	HRESULT Ready_Layer_Boss_Koofu(const _wstring& strLayerTag);

	HRESULT Ready_LandObjects();

	HRESULT ParseInitialize();

public:
	static CLevel_Koofu* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END