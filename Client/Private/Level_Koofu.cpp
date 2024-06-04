#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\Level_Koofu.h"

#include "GameInstance.h"
#include "LandObject.h"
#include <Camera.h>
#include "TachoShop_Tile.h"
#include "Bush.h"
#include "Boss_Koofu.h"
#include "TravelNpc.h"

CLevel_Koofu::CLevel_Koofu(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel_UI{ pGraphic_Device }
{
}

void CLevel_Koofu::Font_Initialize()
{
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	// ��Ʈ ���� - CurrentPlayerMoney
	tFontInfo.Height = 40;
	tFontInfo.Width = 30;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	wcscpy_s(tFontInfo.FaceName, LF_FACESIZE, TEXT("Cafe24 Ssurround air OTF Light"));

	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pBoss_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentPlayerMoney_Font Failed");
		return;
	}
}

HRESULT CLevel_Koofu::Initialize()
{
	m_iLevelIndex = LEVEL_KOOFU;

	Font_Initialize();

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_TravelNpc(TEXT("Layer_TravelNpc"))))
		return E_FAIL;

	__super::Initialize();

	if (FAILED(ParseInitialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Boss_Koofu(TEXT("Layer_Boss_Koofu"))))
		return E_FAIL;

	// ���� ���
	m_pGameInstance->Play_Sound(L"BGM_SnowFinalBoss", LEVEL_STATIC, true);
	// ���� ����
	m_pGameInstance->Set_Volume(L"BGM_SnowFinalBoss", LEVEL_STATIC, 0.2f);

	return S_OK;
}

void CLevel_Koofu::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	// �ð� ����� ����
	m_ElapsedTime += fTimeDelta;

	// ������ �����ϰ� ���� ȣ����� �ʾҴٸ�, �޼��带 ȣ���ϰ� �÷��׸� �����մϴ�.
	if (m_ElapsedTime >= m_DisplayDelay && !m_bBossHpUILayerReady)
	{
		if (FAILED(Ready_Layer_Boss_Koofu_Hp_UI(TEXT("Layer_Boss_Koofu_Hp_UI"))))
		{
		}
		m_bBossHpUILayerReady = true; // �� �� ȣ�� �� �÷��� ����
	}

	//// Ű �Է¿� ���� �ؽ�Ʈ ��ġ ����
	//if (GetAsyncKeyState(VK_UP) & 0x8000) {
	//	m_TextPosY -= 1.f;
	//}
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
	//	m_TextPosY += 1.f;
	//}
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
	//	m_TextPosX -= 1.f;
	//}
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
	//	m_TextPosX += 1.f;
	//}
}

HRESULT CLevel_Koofu::Render()
{
	// ���� �ð��� ������� �ʾ����� ��� ��ȯ
	if (m_ElapsedTime < m_DisplayDelay)
		return S_OK;

	// ���İ��� ��� (��� �ð� �������� 0���� 255�� ����)
	float alphaTime = m_ElapsedTime - m_DisplayDelay; // ���İ� ���� �ð�
	float alphaDuration = 1.0f; // ���İ��� 0���� 255�� �����ϴµ� �ɸ��� �ð� (��: 3��)
	int alpha = static_cast<int>((alphaTime / alphaDuration) * 255);
	if (alpha > 255) alpha = 255; // �ִ밪 255�� ����

	// �ؽ�Ʈ ����ȭ �� ������
	wchar_t text[256];

	// �ؽ�Ʈ ������ - ��Ǫ
	swprintf_s(text, L"%s", L"��Ǫ");
	RECT rect;
	SetRect(&rect, static_cast<int>(g_iWinSizeX * 0.5f + m_TextPosX), static_cast<int>(100.f + m_TextPosY), 0, 0); // �ؽ�Ʈ�� ����� ��ġ
	m_pBoss_Font->DrawText(
		NULL,
		text,
		-1,
		&rect,
		DT_NOCLIP,
		D3DCOLOR_ARGB(alpha, 255, 255, 255)
	);

	return S_OK;
}
HRESULT CLevel_Koofu::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_Terrain"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Koofu::Ready_Layer_Sky(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"), strLayerTag)))
		return E_FAIL;
}

HRESULT CLevel_Koofu::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::CAMERA_DESC			CameraDesc{};

	CameraDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_KOOFU, TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_Camera"), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Koofu::Ready_Layer_Tile(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles)
{
	CTachoShop_Tile::TILEDESC tileDesc{};

	float startX = 17.64f;
	float startY = 0.006f;
	float startZ = 29.510f;
	float spacing = 1.0f; // �� Ÿ�� ������ ����

	for (int z = 0; z < verticalTiles; ++z)
	{
		for (int x = 0; x < horizontalTiles; ++x)
		{
			tileDesc.startPosition = _float3(startX + x * spacing, startY, startZ + z * spacing);
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_TachoGround_Tile"), strLayerTag, &tileDesc)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CLevel_Koofu::Ready_Layer_Bush(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles)
{
	CBush::BUSHDESC bushDecs{};

	float startX = 23.944f;
	float startY = 0.600f;
	float startZ = 25.468f;
	float spacing = 0.8f; // �� Ÿ�� ������ ����

	for (int z = 0; z < verticalTiles; ++z)
	{
		for (int x = 0; x < horizontalTiles; ++x)
		{
			bushDecs.startPosition = _float3(startX + x * spacing, startY, startZ + z * spacing);
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_Bush"), strLayerTag, &bushDecs)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CLevel_Koofu::Ready_Layer_Boss_Koofu(const _wstring& strLayerTag)
{
	CBoss_Koofu::BOSS_KOOFU_DESC			Bosskoofu{};

	Bosskoofu.iCurrentHp = 10;
	Bosskoofu.iMaxHp = 10;
	Bosskoofu.iAttack = 1;
	Bosskoofu.m_pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_KOOFU, TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_Boss_Koofu"), strLayerTag, &Bosskoofu)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Koofu::Ready_Layer_Boss_Koofu_Hp_UI(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_UI_HP_Green_Enemy"), strLayerTag)))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_UI_HP_Enemy"), strLayerTag)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Koofu::Ready_LandObjects()
{
	/*CLandObject::LANDOBJECT_DESC	Desc{};

	Desc.m_pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"), 0));
	Desc.m_pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0));

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"), Desc)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_Koofu::Ready_Layer_Player(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_Player"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Koofu::Ready_Layer_TravelNpc(const _wstring& strLayerTag)
{
	CTravelNpc::NPCDESC desc{};
	desc.position = _float3(62.5f, 1.0f, 31.151f);
	desc.scale = _float3(2.f, 2.f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_KOOFU, TEXT("Prototype_GameObject_TravelNpc"), strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Koofu::ParseInitialize()
{
	if (FAILED(__super::ParseInitialize()))
		return E_FAIL;
}


CLevel_Koofu* CLevel_Koofu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Koofu* pInstance = new CLevel_Koofu(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Koofu"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Koofu::Free()
{

	__super::Free();
}
