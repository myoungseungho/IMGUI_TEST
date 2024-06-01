#include "stdafx.h"
#include "..\Public\Shop.h"

#include "UI_ItemTabIcon_Caution.h"
#include "UI_ItemTabIcon_Food.h"
#include "UI_ItemTabIcon_Hat.h"
#include "UI_ItemTabIcon_Leaf.h"
#include "UI_Cursor.h"
#include "UI_Inventory_BackGround.h"
#include "UI_Item.h"
#include "UI_Hat.h"
#include "Player.h"
#include "GameInstance.h"
#include "Level_UI.h"

CShop::CShop(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CShop::CShop(const CShop& Prototype)
	: CUIObject{ Prototype }
{
}

void CShop::Font_Initialize()
{
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	// 폰트 설정 - CurrentPlayerMoney
	tFontInfo.Height = 40;
	tFontInfo.Width = 30;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	wcscpy_s(tFontInfo.FaceName, LF_FACESIZE, TEXT("Cafe24 Ssurround air OTF Light"));

	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pCurrentPlayerMoney_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentPlayerMoney_Font Failed");
		return;
	}

	// 폰트 설정 - CurrentItemTitle
	tFontInfo.Height = 30;
	tFontInfo.Width = 20;
	tFontInfo.Weight = FW_HEAVY;
	// 이미 tFontInfo가 초기화되어 있으므로, 필요한 부분만 변경합니다.
	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pCurrentItemTitle_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentItemTitle_Font Failed");
		return;
	}

	// 폰트 설정 - CurrentItemExplain
	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pCurrentItemExplain_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentItemExplain_Font Failed");
		return;
	}

	////
		// 폰트 설정 - CurrentItemTitle
	tFontInfo.Height = 30;
	tFontInfo.Width = 20;
	tFontInfo.Weight = FW_HEAVY;
	// 이미 tFontInfo가 초기화되어 있으므로, 필요한 부분만 변경합니다.
	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pBuyCount_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentItemTitle_Font Failed");
		return;
	}

	// 폰트 설정 - CurrentItemTitle
	tFontInfo.Height = 30;
	tFontInfo.Width = 20;
	tFontInfo.Weight = FW_HEAVY;
	// 이미 tFontInfo가 초기화되어 있으므로, 필요한 부분만 변경합니다.
	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pBuyDecision_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentItemTitle_Font Failed");
		return;
	}

	// 폰트 설정 - CurrentItemTitle
	tFontInfo.Height = 30;
	tFontInfo.Width = 20;
	tFontInfo.Weight = FW_HEAVY;
	// 이미 tFontInfo가 초기화되어 있으므로, 필요한 부분만 변경합니다.
	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pBuyCancel_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentItemTitle_Font Failed");
		return;
	}
}


HRESULT CShop::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CShop::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Font_Initialize();

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.0f, 1.f);

	//UI오브젝트 받아서 VECTOR에 넣기
	auto AddUIObject = [&](const TCHAR* prototypeTag, const TCHAR* layerTag, void* pArg = nullptr, const _uint count = 0) -> HRESULT {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, prototypeTag, layerTag, pArg)))
			return E_FAIL;
		CUIObject* pUIObject = static_cast<CUIObject*>(m_pGameInstance->Get_GameObject(LEVEL_STATIC, layerTag, count));
		if (!pUIObject)
			return E_FAIL;
		Safe_AddRef(pUIObject);
		m_vecUIObject.push_back(pUIObject);
		return S_OK;
		};

	UIDATA slotData{};

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_FadeInOut"), TEXT("Layer_UI_FadeInOut"))))
		return E_FAIL;

	for (size_t i = 0; i < 2; i++)
	{
		switch (i)
		{
		case 0:
			slotData.position = { -430.f, 85.f };
			slotData.scale = { 160.f, 165.f };
			slotData.alpha = 255.f;
			break;
		case 1:
			slotData.position = { -430.f, 85.f };
			slotData.scale = { 160.f, 165.f };
			slotData.alpha = 255.f;
			break;
		default:
			break;
		}

		slotData.index = i;

		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Cursor"), TEXT("Layer_ZZUI_Cursor"), &slotData, i)))
			return E_FAIL;
	}

	CGameObject* cursor = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_ZZUI_Cursor"), 1);
	if (cursor)
	{
		CUIObject* cursorUI = static_cast<CUIObject*>(cursor);
		cursorUI->m_bIsOn = false;
	}


	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_DotLine"), TEXT("Layer_UI_Inventory_DotLine"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_Slot"), TEXT("Layer_UI_Inventory_Slot"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_VerticalDotLine"), TEXT("Layer_UI_Inventory_VerticalDotLine"))))

		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_ItemTabIcon_Caution"), TEXT("Layer_UI_ItemTabIcon_Caution"))))
			return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_ItemTabIcon_Food"), TEXT("Layer_UI_ItemTabIcon_Food"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_ItemTabIcon_Hat"), TEXT("Layer_UI_ItemTabIcon_Hat"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_ItemTabIcon_Leaf"), TEXT("Layer_UI_ItemTabIcon_Leaf"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Shop_ItemInfo_TextBox"), TEXT("Layer_UI_Shop_ItemInfo_TextBox"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Shop_PlayerCoin"), TEXT("Layer_UI_Shop_PlayerCoin"))))
		return E_FAIL;



	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_SlotBeigeBackground"), TEXT("Layer_UI_Inventory_SlotBeigeBackground"))))
		return E_FAIL;

	//if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_Button"), TEXT("Layer_UI_Inventory_Button"))))
	//	return E_FAIL;


	const float initialX = -430.0f; // 첫 번째 열의 초기 X 위치
	const float initialY = 85.0f; // 첫 번째 행의 초기 Y 위치
	const float deltaX = 145.0f; // 열 이동 시의 X 위치 증감분
	const float deltaY = -135.0f; // 행 이동 시의 Y 위치 증감분

	for (size_t i = 0; i < m_iInitHatCount; i++)
	{
		// 각 슬롯에 대한 위치와 크기 설정
		int row = (i / 5); // 두 번째 행부터 시작
		int col = i % 5;

		slotData.position = { initialX + col * deltaX, initialY + row * deltaY };
		slotData.scale = { 60.f, 70.f };
		slotData.alpha = 255.f;
		slotData.index = i;

		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Hat"), TEXT("Layer_ZUI_Hat"), &slotData, i)))
			return E_FAIL;

		// 아이템 정보 설정
		m_vecItemInfo.push_back(m_vecHatInfo[i]);
	}

	for (size_t i = 0; i < m_iInitItemCount; i++)
	{
		// 각 슬롯에 대한 위치와 크기 설정
		int row = (i / 5); // 두 번째 행부터 시작
		int col = i % 5;

		slotData.position = { initialX + col * deltaX, initialY + row * deltaY };
		slotData.scale = { 60.f, 70.f };
		slotData.alpha = 255.f;
		slotData.index = i;

		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Item"), TEXT("Layer_ZUI_Item"), &slotData, i)))
			return E_FAIL;

		m_vecItemInfo.push_back(m_vecItemInfo[i]);
	}


	PRICEUIDATA priceuiData{};
	const float priceTaginitialX = -390.0f; // 첫 번째 열의 초기 X 위치
	const float priceTaginitialY = 45.0f; // 첫 번째 행의 초기 Y 위치

	//모자 프라이스 태그
	for (size_t i = 0; i < m_iInitHatCount; i++)
	{
		// 각 슬롯에 대한 위치와 크기 설정
		int row = (i / 5); // 두 번째 행부터 시작
		int col = i % 5;

		priceuiData.position = { priceTaginitialX + col * deltaX, priceTaginitialY + row * deltaY };
		priceuiData.scale = { 65, 30.f };
		priceuiData.alpha = 255.f;
		priceuiData.index = i;
		priceuiData.price = m_vecHatPrice[i];

		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Shop_PriceTag"), TEXT("Layer_ZUI_Shop_PriceTag"), &priceuiData, i)))
			return E_FAIL;

		CGameObject* priceTag = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_ZUI_Shop_PriceTag"), i);
		m_vecHatPriceObject.push_back(static_cast<CUIObject*>(priceTag));
		Safe_AddRef(priceTag);
	}


	//아이템 프라이스 태그
	for (size_t i = 0; i < m_iInitItemCount; i++)
	{
		// 각 슬롯에 대한 위치와 크기 설정
		int row = (i / 5); // 두 번째 행부터 시작
		int col = i % 5;

		priceuiData.position = { priceTaginitialX + col * deltaX, priceTaginitialY + row * deltaY };
		priceuiData.scale = { 65, 30.f };
		priceuiData.alpha = 255.f;
		priceuiData.index = i + m_iInitHatCount;
		priceuiData.price = m_vecItemPrice[i];

		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Shop_PriceTag"), TEXT("Layer_ZUI_Shop_PriceTag"), &priceuiData, i)))
			return E_FAIL;

		CGameObject* priceTag = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_ZUI_Shop_PriceTag"), i + m_iInitHatCount);
		m_vecItemPriceObject.push_back(static_cast<CUIObject*>(priceTag));
		Safe_AddRef(priceTag);
	}

	for (size_t i = 0; i < 3; i++)
	{
		slotData.position = { -430.f, 80.f };
		slotData.scale = { 120.f, 50.f };
		slotData.alpha = 255.f;
		slotData.index = i;

		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_BackGround"), TEXT("Layer_ZUI_ZInventory_BackGround"), &slotData, i)))
			return E_FAIL;


		CGameObject* backGround = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_ZUI_ZInventory_BackGround"), i);
		m_vecBackGroundObject.push_back(static_cast<CUIObject*>(backGround));
		Safe_AddRef(backGround);
	}

	// 초기 상태 설정
	UpdateRow();
	return S_OK;
}

void CShop::Priority_Update(_float fTimeDelta)
{
}

void CShop::Update(_float fTimeDelta)
{
	if (!m_bIsOn)
		return;

	bool positionChanged = false;

	if (m_pKeyCom->Key_Down(VK_RETURN)) // 엔터 키를 눌렀을 때
	{
		// CUI_Inventory_BackGround 객체의 Y 위치를 차례로 증가시키기
		m_bBackgroundsActive = !m_bBackgroundsActive; // 백그라운드 활성화 상태로 변경
		SetBackGroundOnOff();
		ArrangeInventoryBackgrounds();

		if (!m_bBackgroundsActive)
		{
			m_iCurrentBackgroundIndex = 2;
			m_iCurrentBuyCount = 1;
			CGameObject* cursor = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_ZZUI_Cursor"), 1);
			if (cursor)
			{
				CUIObject* cursorUI = static_cast<CUIObject*>(cursor);
				cursorUI->m_bIsOn = false;
			}
		}
	}

	if (m_bBackgroundsActive)
	{
		// 백그라운드 활성화 상태에서 방향키 처리
		if (m_pKeyCom->Key_Down(VK_DOWN))
		{
			m_iCurrentBackgroundIndex = (m_iCurrentBackgroundIndex - 1 + 3) % 3;
			positionChanged = true;
		}
		else if (m_pKeyCom->Key_Down(VK_UP))
		{
			m_iCurrentBackgroundIndex = (m_iCurrentBackgroundIndex + 1) % 3;
			positionChanged = true;
		}

		switch (m_iCurrentBackgroundIndex)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:

			if (m_pKeyCom->Key_Down(VK_LEFT))
			{
				int itemIndex = (m_iCurrentRow - 1) * 5 + m_iCurrentCol;
				if (itemIndex < m_vecHatPrice.size())
				{
					_uint price = m_vecHatPrice[itemIndex];
					m_iMaxBuyCount = m_iCurrentMoney / price;
					m_iCurrentBuyCount = m_iMaxBuyCount;
				}
			}
			else if (m_pKeyCom->Key_Down(VK_RIGHT))
			{

			}
			break;
		}


		MoveCursorToBackground(m_iCurrentBackgroundIndex);
	}
	else
	{
		// 기본 상태에서 방향키 처리
		if (m_pKeyCom->Key_Down(VK_UP))
		{
			// 위쪽 방향키 입력 처리
			if (m_iCurrentRow > 1) {
				// 두 번째 행 이상일 때 처리
				m_iCurrentRow = (m_iCurrentRow - 1 + m_iMaxRows) % m_iMaxRows;
				m_iCurrentCol = min(m_iCurrentCol, getMaxCols(m_iCurrentRow) - 1);
				positionChanged = true;
			}
			else if (m_iCurrentRow == 1) {
				// 두 번째 행에서 첫 번째 행으로 이동 시
				m_iCurrentRow = 0;
				m_iCurrentCol = 0; // 첫 번째 행의 첫 번째 열로 이동
				positionChanged = true;
			}
		}
		if (m_pKeyCom->Key_Down(VK_DOWN))
		{
			// 아래쪽 방향키 입력 처리
			if (m_iCurrentRow == m_iMaxRows - 1) {
				// 마지막 행에서 아래쪽으로 이동 시 두 번째 행으로 이동
				m_iCurrentRow = 1;
				positionChanged = true;
			}
			else if (m_iCurrentRow < m_iMaxRows - 1) {
				m_iCurrentRow = (m_iCurrentRow + 1) % m_iMaxRows;
				positionChanged = true;
			}
			m_iCurrentCol = min(m_iCurrentCol, getMaxCols(m_iCurrentRow) - 1);
		}
		if (m_pKeyCom->Key_Down(VK_LEFT))
		{
			// 왼쪽 방향키 입력 처리
			int maxCols = getMaxCols(m_iCurrentRow);
			m_iCurrentCol = (m_iCurrentCol - 1 + maxCols) % maxCols;
			positionChanged = true;
		}
		if (m_pKeyCom->Key_Down(VK_RIGHT))
		{
			// 오른쪽 방향키 입력 처리
			int maxCols = getMaxCols(m_iCurrentRow);
			m_iCurrentCol = (m_iCurrentCol + 1) % maxCols;
			positionChanged = true;
		}

		if (positionChanged) {
			UpdateRow();
			m_iPreviousRow = m_iCurrentRow;
			m_iPreviousCol = m_iCurrentCol;
		}
	}

	if (m_pKeyCom->Key_Down('P'))
	{
		static_cast<CLevel_UI*>(m_pGameInstance->GetCurrentLevel())->m_bIsAllowInventory = true;
		// 플레이어 인풋 다시 풀어야함
		CPlayer* player = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Player")));
		player->m_bOpenShopAndInventory = false;

		SetInventoryOnOff();
		// 인벤토리 다시 나올 수 있게

		CGameObject* cursor = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_UI_ZZZCursor"), 1);
		if (cursor)
		{
			CUIObject* cursorUI = static_cast<CUIObject*>(cursor);
			cursorUI->m_bIsOn = false;
		}
	}

	// 현재 선택된 아이템의 인덱스를 계산
	int selectedIndex = 0;

	if (m_iCurrentRow > 0) {
		// 두 번째 행부터 현재 행까지의 인덱스를 계산
		for (int i = 1; i < m_iCurrentRow; ++i) {
			selectedIndex += getMaxCols(i);
		}
		selectedIndex += m_iCurrentCol;
	}
	else {
		// 첫 번째 행인 경우
		selectedIndex = m_iCurrentCol;
	}

	UpdateSelectedItemInfo();

	// 선택된 아이템 처리 (예: 하이라이트 표시)
	// HandleSelection(selectedIndex); // 가정된 함수
}

void CShop::UpdateRow()
{
	if (m_iCurrentRow == 0) {
		Control_FirstRow();
	}
	else {
		Control_OtherRow();
	}
}

void CShop::ShowHats()
{
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_Hat))
			iter->m_fAlpha = 255.f; // Hat 객체를 보이게 설정
		else if (typeid(*iter) == typeid(CUI_Item))
			iter->m_fAlpha = 0.f; // Item 객체를 숨김
	}

	if (m_bIsOn)
	{
		for (auto& iter : m_vecHatPriceObject)
		{
			iter->m_bIsOn = true;
		}

		for (auto& iter : m_vecItemPriceObject)
		{
			iter->m_bIsOn = false;
		}
	}
}

void CShop::ShowItems()
{
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_Hat))
			iter->m_fAlpha = 0.f; // Hat 객체를 보이게 설정
		else if (typeid(*iter) == typeid(CUI_Item))
			iter->m_fAlpha = 255.f; // Item 객체를 숨김
	}

	for (auto& iter : m_vecHatPriceObject)
	{
		iter->m_bIsOn = false;
	}

	for (auto& iter : m_vecItemPriceObject)
	{
		iter->m_bIsOn = true;
	}
}

void CShop::MoveCursorToBackground(int backgroundIndex)
{
	CGameObject* cursor = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_ZZUI_Cursor"), 1);
	if (cursor)
	{
		CUIObject* cursorUI = static_cast<CUIObject*>(cursor);
		if (cursorUI && backgroundIndex < m_vecBackGroundObject.size())
		{
			CUIObject* targetBackground = m_vecBackGroundObject[backgroundIndex];
			cursorUI->m_fX = targetBackground->m_fX;
			cursorUI->m_fY = targetBackground->m_fY;
			cursorUI->m_fSizeX = targetBackground->m_fSizeX;
			cursorUI->m_fSizeY = targetBackground->m_fSizeY;
			cursorUI->m_bIsOn = true;
		}
	}
}

void CShop::Control_FirstRow()
{
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_ItemTabIcon_Hat)) {
			iter->m_fAlpha = (m_iCurrentCol == 0) ? 255.f : 150.f;
		}
		else if (typeid(*iter) == typeid(CUI_ItemTabIcon_Food)) {
			iter->m_fAlpha = (m_iCurrentCol == 1) ? 255.f : 150.f;
		}
		else if (typeid(*iter) == typeid(CUI_ItemTabIcon_Leaf)) {
			iter->m_fAlpha = (m_iCurrentCol == 2) ? 255.f : 150.f;
		}
		else if (typeid(*iter) == typeid(CUI_ItemTabIcon_Caution)) {
			iter->m_fAlpha = (m_iCurrentCol == 3) ? 255.f : 150.f;
		}
	}

	// 첫 번째 행일 때 CUI_Cursor를 숨김
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_Cursor))
		{
			iter->m_fAlpha = 0.f;
		}

		if (typeid(*iter) == typeid(CUI_Inventory_BackGround))
		{
			iter->m_fAlpha = 0.f;
			//// 특정 조건에 맞는 CUI_Inventory_BackGround 객체를 선택
			//if (iter->m_iIndex == 1) {
			//	iter->m_fAlpha = 0.f;
			//}
		}
	}

	// 첫 번째 행에서 선택된 열 위치를 기억
	m_firstRowSelectedCol = m_iCurrentCol;

	// 두 번째 행 이후의 객체를 변경
	if (m_iCurrentCol == 0)
	{
		// Hat 객체를 보여줌
		ShowHats();
	}
	else if (m_iCurrentCol == 1)
	{
		// Item 객체를 보여줌
		ShowItems();
	}
}

void CShop::Control_OtherRow()
{
	// 첫 번째 행의 아이템들의 알파값을 150.f로 설정
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_ItemTabIcon_Hat) ||
			typeid(*iter) == typeid(CUI_ItemTabIcon_Food) ||
			typeid(*iter) == typeid(CUI_ItemTabIcon_Leaf) ||
			typeid(*iter) == typeid(CUI_ItemTabIcon_Caution)) {
			iter->m_fAlpha = 150.f;
		}
	}

	// CUI_Cursor 객체의 위치를 업데이트
	const float initialCursorX = -430.0f; // 첫 번째 열의 초기 X 위치
	const float initialCursorY = 85.0f; // 첫 번째 행의 초기 Y 위치
	const float deltaX = 145.0f; // 열 이동 시의 X 위치 증감분
	const float deltaY = -135.0f; // 행 이동 시의 Y 위치 증감분

	// CUI_Inventory_BackGround 객체의 위치를 업데이트
	const float initialInventoryX = -275.0f; // Inventory_BackGround의 초기 X 위치
	const float initialInventoryY = 55.0f; // Inventory_BackGround의 초기 Y 위치
	const float inventoryDeltaY = 50.0f; // Inventory_BackGround의 Y 위치 증감분

	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_Cursor))
		{
			if (iter->m_iIndex == 0)
			{
				iter->m_fX = initialCursorX + m_iCurrentCol * deltaX;
				iter->m_fY = initialCursorY + (m_iCurrentRow - 1) * deltaY; // 첫 번째 행 제외
				iter->m_fAlpha = 255.f; // 두 번째 행부터는 커서가 보이게 설정
			}
		}

		//if (typeid(*iter) == typeid(CUI_Inventory_BackGround))
		//{
		//	int index = iter->m_iIndex;
		//	iter->m_fX = initialInventoryX + m_iCurrentCol * deltaX;
		//	iter->m_fY = initialInventoryY + (m_iCurrentRow - 1) * deltaY + index * inventoryDeltaY; // 첫 번째 행 제외, index에 따라 y위치 증가
		//}
	}
}

void CShop::UpdateSelectedItemInfo()
{
	int selectedIndex = 0;

	if (m_iCurrentRow > 0) {
		// 두 번째 행부터 현재 행까지의 인덱스를 계산
		for (int i = 1; i < m_iCurrentRow; ++i) {
			selectedIndex += getMaxCols(i);
		}
		selectedIndex += m_iCurrentCol;

		// 인덱스를 통해 현재 선택된 아이템 정보를 업데이트
		if (selectedIndex >= 0 && selectedIndex < m_vecItemInfo.size()) {
			m_WstringTitle = m_vecItemInfo[selectedIndex].title;
			m_WstringExplain = m_vecItemInfo[selectedIndex].explain;
		}
	}
}

void CShop::ArrangeInventoryBackgrounds()
{
	const float deltaX = 145.0f; // 열 이동 시의 X 위치 증감분
	const float deltaY = -135.0f; // 행 이동 시의 Y 위치 증감분
	const float initialInventoryX = -275.0f; // Inventory_BackGround의 초기 X 위치
	const float initialInventoryY = 55.0f; // Inventory_BackGround의 초기 Y 위치
	const float inventoryDeltaY = 50.0f; // Inventory_BackGround의 Y 위치 증감분

	for (auto& iter : m_vecBackGroundObject)
	{
		int index = iter->m_iIndex;
		iter->m_fX = initialInventoryX + m_iCurrentCol * deltaX;
		iter->m_fY = initialInventoryY + (m_iCurrentRow - 1) * deltaY + index * inventoryDeltaY; // 첫 번째 행 제외, index에 따라 y위치 증가
		iter->m_fAlpha = 255.f;
	}
}

void CShop::Late_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn이 false이면 업데이트를 수행하지 않음

	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CShop::Render(_float fTimeDelta)
{
	if (!m_bIsOn) return S_OK; // m_bIsOn이 false이면 업데이트를 수행하지 않음

	__super::Begin_RenderState();

	// 텍스트 형식화 및 렌더링
	wchar_t text[256];

	// 텍스트 렌더링 - CurrentPlayerMoney
	swprintf_s(text, L"%d", m_iCurrentMoney);
	RECT rect;
	SetRect(&rect, static_cast<int>(1130.f), static_cast<int>(50.f), 0, 0); // 텍스트를 출력할 위치
	m_pCurrentPlayerMoney_Font->DrawText(
		NULL,
		text,
		-1,
		&rect,
		DT_NOCLIP,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);

	// 첫 번째 행인 경우 텍스트와 타이틀을 표시하지 않음
	if (m_iCurrentRow == 0)
	{
		__super::End_RenderState();
		return S_OK;
	}

	// 현재 선택된 아이템의 인덱스 계산
	int selectedIndex = 0;

	if (m_iCurrentRow > 0) {
		// 두 번째 행부터 현재 행까지의 인덱스를 계산
		for (int i = 1; i < m_iCurrentRow; ++i) {
			selectedIndex += getMaxCols(i);
		}
		selectedIndex += m_iCurrentCol;
	}

	// 선택된 아이템의 제목과 설명을 설정
	if (selectedIndex < m_vecItemInfo.size()) {
		if (m_firstRowSelectedCol == 0) {
			// Hat의 정보
			m_WstringTitle = m_vecItemInfo[selectedIndex + m_iInitHatCount].title; // Item 정보는 Hat 정보 이후에 있음
			m_WstringExplain = m_vecItemInfo[selectedIndex + m_iInitHatCount].explain;
		}
		else if (m_firstRowSelectedCol == 1) {
			// Item의 정보
			m_WstringTitle = m_vecItemInfo[selectedIndex].title;
			m_WstringExplain = m_vecItemInfo[selectedIndex].explain;
		}
	}

	// 텍스트 렌더링 - CurrentItemTitle
	swprintf_s(text, L"%s", m_WstringTitle.c_str());
	SetRect(&rect, static_cast<int>(950.f), static_cast<int>(390.f), 0, 0); // 텍스트를 출력할 위치 변경
	m_pCurrentItemTitle_Font->DrawText(
		NULL,
		text,
		-1,
		&rect,
		DT_NOCLIP,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);

	// 텍스트 렌더링 - CurrentItemExplain
	swprintf_s(text, L"%s", m_WstringExplain.c_str());
	SetRect(&rect, static_cast<int>(950.f), static_cast<int>(460.f), 0, 0); // 텍스트를 출력할 위치 변경
	m_pCurrentItemExplain_Font->DrawText(
		NULL,
		text,
		-1,
		&rect,
		DT_NOCLIP,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);

	if (m_bBackgroundsActive)
	{
		if (m_vecBackGroundObject.size() >= 3) {
			// 폰트 좌표계 보정
			int winCenterX = g_iWinSizeX / 2;
			int winCenterY = g_iWinSizeY / 2;
			// 텍스트 렌더링 - BuyCount
			swprintf_s(text, L"%d개", m_iCurrentBuyCount);
			SetRect(&rect, static_cast<int>(winCenterX + m_vecBackGroundObject[2]->m_fX) + m_TextPosX, static_cast<int>(winCenterY - m_vecBackGroundObject[2]->m_fY) + m_TextPosY, 0, 0); // 텍스트를 출력할 위치 변경
			m_pBuyCount_Font->DrawText(
				NULL,
				text,
				-1,
				&rect,
				DT_NOCLIP,
				D3DCOLOR_ARGB(255, 0, 0, 0)
			);

			// 텍스트 렌더링 - BuyDecision
			swprintf_s(text, L"사기");
			SetRect(&rect, static_cast<int>(winCenterX + m_vecBackGroundObject[1]->m_fX) + m_TextPosX, static_cast<int>(winCenterY - m_vecBackGroundObject[1]->m_fY) + m_TextPosY, 0, 0); // 텍스트를 출력할 위치 변경
			m_pBuyDecision_Font->DrawText(
				NULL,
				text,
				-1,
				&rect,
				DT_NOCLIP,
				D3DCOLOR_ARGB(255, 0, 0, 0)
			);

			// 텍스트 렌더링 - BuyCancel
			swprintf_s(text, L"닫기");
			SetRect(&rect, static_cast<int>(winCenterX + m_vecBackGroundObject[0]->m_fX) + m_TextPosX, static_cast<int>(winCenterY - m_vecBackGroundObject[0]->m_fY) + m_TextPosY, 0, 0); // 텍스트를 출력할 위치 변경
			m_pBuyCancel_Font->DrawText(
				NULL,
				text,
				-1,
				&rect,
				DT_NOCLIP,
				D3DCOLOR_ARGB(255, 0, 0, 0)
			);
		}
	}

	__super::End_RenderState();

	return S_OK;
}

HRESULT CShop::Ready_Components()
{
	/* For.Com_KeyState */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Key"),
		TEXT("Com_KeyState"), reinterpret_cast<CComponent**>(&m_pKeyCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}


CShop* CShop::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CShop* pInstance = new CShop(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Inventory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShop::Clone(void* pArg)
{
	CShop* pInstance = new CShop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Inventory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShop::Free()
{
	for (auto& pUIObject : m_vecUIObject)
	{
		Safe_Release(pUIObject);
	}
	m_vecUIObject.clear();

	for (auto& pUIObject : m_vecBackGroundObject)
	{
		Safe_Release(pUIObject);
	}
	m_vecBackGroundObject.clear();

	for (auto& pUIObject : m_vecHatPriceObject)
	{
		Safe_Release(pUIObject);

	}
	m_vecHatPriceObject.clear();

	for (auto& pUIObject : m_vecItemPriceObject)
	{
		Safe_Release(pUIObject);
	}
	m_vecItemPriceObject.clear();

	m_vecHatPriceObject.clear();
	Safe_Release(m_pKeyCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCurrentPlayerMoney_Font);
	Safe_Release(m_pCurrentItemTitle_Font);
	Safe_Release(m_pCurrentItemExplain_Font);
	Safe_Release(m_pBuyCount_Font);
	Safe_Release(m_pBuyDecision_Font);
	Safe_Release(m_pBuyCancel_Font);

	__super::Free();
}

