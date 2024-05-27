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
	/* �� ���ӳ��� ����� �����ϴ� ��ü���� �߰��Ѵ�. */
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Sky(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Tile(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles);
	HRESULT Ready_Layer_Bush(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles);
	HRESULT Ready_Layer_Boss_Bug(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Test(const _wstring& strLayerTag);

	HRESULT Ready_LandObjects();

	HRESULT ParseInitialize();

public:
	static CLevel_Bug* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END