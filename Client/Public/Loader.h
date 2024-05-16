#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 3. 다음 장면(레벨)에 필요한 자원도 로드하낟. */

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(LEVELID eNextLevelID);
	HRESULT Loading();

public:
	_bool isFinished() const {
		return m_isFinished;
	}

	void Draw_Text() {
		SetWindowText(g_hWnd, m_szLoadingText);
	}

private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	LEVELID						m_eLevelID = {};
	_bool						m_isFinished = { false };
	_tchar						m_szLoadingText[MAX_PATH] = {};

private:
	HANDLE						m_hThread = {};
	CRITICAL_SECTION			m_Critical_Section = {};
	CGameInstance*				m_pGameInstance = { nullptr };


private:
	HRESULT Loading_For_LogoLevel();	
	HRESULT Loading_For_GamePlayLevel();

public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVELID eNextLevelID);
	virtual void Free() override;
};

END