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

	// ��Ʈ ���� - CurrentPlayerMoney
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

	// ��Ʈ ���� - CurrentItemTitle
	tFontInfo.Height = 30;
	tFontInfo.Width = 20;
	tFontInfo.Weight = FW_HEAVY;
	// �̹� tFontInfo�� �ʱ�ȭ�Ǿ� �����Ƿ�, �ʿ��� �κи� �����մϴ�.
	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pCurrentItemTitle_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentItemTitle_Font Failed");
		return;
	}

	// ��Ʈ ���� - CurrentItemExplain
	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pCurrentItemExplain_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentItemExplain_Font Failed");
		return;
	}

	////
		// ��Ʈ ���� - CurrentItemTitle
	tFontInfo.Height = 30;
	tFontInfo.Width = 20;
	tFontInfo.Weight = FW_HEAVY;
	// �̹� tFontInfo�� �ʱ�ȭ�Ǿ� �����Ƿ�, �ʿ��� �κи� �����մϴ�.
	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pBuyCount_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentItemTitle_Font Failed");
		return;
	}

	// ��Ʈ ���� - CurrentItemTitle
	tFontInfo.Height = 30;
	tFontInfo.Width = 20;
	tFontInfo.Weight = FW_HEAVY;
	// �̹� tFontInfo�� �ʱ�ȭ�Ǿ� �����Ƿ�, �ʿ��� �κи� �����մϴ�.
	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pBuyDecision_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentItemTitle_Font Failed");
		return;
	}

	// ��Ʈ ���� - CurrentItemTitle
	tFontInfo.Height = 30;
	tFontInfo.Width = 20;
	tFontInfo.Weight = FW_HEAVY;
	// �̹� tFontInfo�� �ʱ�ȭ�Ǿ� �����Ƿ�, �ʿ��� �κи� �����մϴ�.
	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pBuyCancel_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentItemTitle_Font Failed");
		return;
	}
}


HRESULT CShop::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

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

	//UI������Ʈ �޾Ƽ� VECTOR�� �ֱ�
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


	const float initialX = -430.0f; // ù ��° ���� �ʱ� X ��ġ
	const float initialY = 85.0f; // ù ��° ���� �ʱ� Y ��ġ
	const float deltaX = 145.0f; // �� �̵� ���� X ��ġ ������
	const float deltaY = -135.0f; // �� �̵� ���� Y ��ġ ������

	for (size_t i = 0; i < m_iInitHatCount; i++)
	{
		// �� ���Կ� ���� ��ġ�� ũ�� ����
		int row = (i / 5); // �� ��° ����� ����
		int col = i % 5;

		slotData.position = { initialX + col * deltaX, initialY + row * deltaY };
		slotData.scale = { 60.f, 70.f };
		slotData.alpha = 255.f;
		slotData.index = i;

		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Hat"), TEXT("Layer_ZUI_Hat"), &slotData, i)))
			return E_FAIL;

		// ������ ���� ����
		m_vecItemInfo.push_back(m_vecHatInfo[i]);
	}

	for (size_t i = 0; i < m_iInitItemCount; i++)
	{
		// �� ���Կ� ���� ��ġ�� ũ�� ����
		int row = (i / 5); // �� ��° ����� ����
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
	const float priceTaginitialX = -390.0f; // ù ��° ���� �ʱ� X ��ġ
	const float priceTaginitialY = 45.0f; // ù ��° ���� �ʱ� Y ��ġ

	//���� �����̽� �±�
	for (size_t i = 0; i < m_iInitHatCount; i++)
	{
		// �� ���Կ� ���� ��ġ�� ũ�� ����
		int row = (i / 5); // �� ��° ����� ����
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


	//������ �����̽� �±�
	for (size_t i = 0; i < m_iInitItemCount; i++)
	{
		// �� ���Կ� ���� ��ġ�� ũ�� ����
		int row = (i / 5); // �� ��° ����� ����
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

	// �ʱ� ���� ����
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

	if (m_pKeyCom->Key_Down(VK_RETURN)) // ���� Ű�� ������ ��
	{
		// CUI_Inventory_BackGround ��ü�� Y ��ġ�� ���ʷ� ������Ű��
		m_bBackgroundsActive = !m_bBackgroundsActive; // ��׶��� Ȱ��ȭ ���·� ����
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
		// ��׶��� Ȱ��ȭ ���¿��� ����Ű ó��
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
		// �⺻ ���¿��� ����Ű ó��
		if (m_pKeyCom->Key_Down(VK_UP))
		{
			// ���� ����Ű �Է� ó��
			if (m_iCurrentRow > 1) {
				// �� ��° �� �̻��� �� ó��
				m_iCurrentRow = (m_iCurrentRow - 1 + m_iMaxRows) % m_iMaxRows;
				m_iCurrentCol = min(m_iCurrentCol, getMaxCols(m_iCurrentRow) - 1);
				positionChanged = true;
			}
			else if (m_iCurrentRow == 1) {
				// �� ��° �࿡�� ù ��° ������ �̵� ��
				m_iCurrentRow = 0;
				m_iCurrentCol = 0; // ù ��° ���� ù ��° ���� �̵�
				positionChanged = true;
			}
		}
		if (m_pKeyCom->Key_Down(VK_DOWN))
		{
			// �Ʒ��� ����Ű �Է� ó��
			if (m_iCurrentRow == m_iMaxRows - 1) {
				// ������ �࿡�� �Ʒ������� �̵� �� �� ��° ������ �̵�
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
			// ���� ����Ű �Է� ó��
			int maxCols = getMaxCols(m_iCurrentRow);
			m_iCurrentCol = (m_iCurrentCol - 1 + maxCols) % maxCols;
			positionChanged = true;
		}
		if (m_pKeyCom->Key_Down(VK_RIGHT))
		{
			// ������ ����Ű �Է� ó��
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
		// �÷��̾� ��ǲ �ٽ� Ǯ�����
		CPlayer* player = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Player")));
		player->m_bOpenShopAndInventory = false;

		SetInventoryOnOff();
		// �κ��丮 �ٽ� ���� �� �ְ�

		CGameObject* cursor = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_UI_ZZZCursor"), 1);
		if (cursor)
		{
			CUIObject* cursorUI = static_cast<CUIObject*>(cursor);
			cursorUI->m_bIsOn = false;
		}
	}

	// ���� ���õ� �������� �ε����� ���
	int selectedIndex = 0;

	if (m_iCurrentRow > 0) {
		// �� ��° ����� ���� ������� �ε����� ���
		for (int i = 1; i < m_iCurrentRow; ++i) {
			selectedIndex += getMaxCols(i);
		}
		selectedIndex += m_iCurrentCol;
	}
	else {
		// ù ��° ���� ���
		selectedIndex = m_iCurrentCol;
	}

	UpdateSelectedItemInfo();

	// ���õ� ������ ó�� (��: ���̶���Ʈ ǥ��)
	// HandleSelection(selectedIndex); // ������ �Լ�
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
			iter->m_fAlpha = 255.f; // Hat ��ü�� ���̰� ����
		else if (typeid(*iter) == typeid(CUI_Item))
			iter->m_fAlpha = 0.f; // Item ��ü�� ����
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
			iter->m_fAlpha = 0.f; // Hat ��ü�� ���̰� ����
		else if (typeid(*iter) == typeid(CUI_Item))
			iter->m_fAlpha = 255.f; // Item ��ü�� ����
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

	// ù ��° ���� �� CUI_Cursor�� ����
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_Cursor))
		{
			iter->m_fAlpha = 0.f;
		}

		if (typeid(*iter) == typeid(CUI_Inventory_BackGround))
		{
			iter->m_fAlpha = 0.f;
			//// Ư�� ���ǿ� �´� CUI_Inventory_BackGround ��ü�� ����
			//if (iter->m_iIndex == 1) {
			//	iter->m_fAlpha = 0.f;
			//}
		}
	}

	// ù ��° �࿡�� ���õ� �� ��ġ�� ���
	m_firstRowSelectedCol = m_iCurrentCol;

	// �� ��° �� ������ ��ü�� ����
	if (m_iCurrentCol == 0)
	{
		// Hat ��ü�� ������
		ShowHats();
	}
	else if (m_iCurrentCol == 1)
	{
		// Item ��ü�� ������
		ShowItems();
	}
}

void CShop::Control_OtherRow()
{
	// ù ��° ���� �����۵��� ���İ��� 150.f�� ����
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_ItemTabIcon_Hat) ||
			typeid(*iter) == typeid(CUI_ItemTabIcon_Food) ||
			typeid(*iter) == typeid(CUI_ItemTabIcon_Leaf) ||
			typeid(*iter) == typeid(CUI_ItemTabIcon_Caution)) {
			iter->m_fAlpha = 150.f;
		}
	}

	// CUI_Cursor ��ü�� ��ġ�� ������Ʈ
	const float initialCursorX = -430.0f; // ù ��° ���� �ʱ� X ��ġ
	const float initialCursorY = 85.0f; // ù ��° ���� �ʱ� Y ��ġ
	const float deltaX = 145.0f; // �� �̵� ���� X ��ġ ������
	const float deltaY = -135.0f; // �� �̵� ���� Y ��ġ ������

	// CUI_Inventory_BackGround ��ü�� ��ġ�� ������Ʈ
	const float initialInventoryX = -275.0f; // Inventory_BackGround�� �ʱ� X ��ġ
	const float initialInventoryY = 55.0f; // Inventory_BackGround�� �ʱ� Y ��ġ
	const float inventoryDeltaY = 50.0f; // Inventory_BackGround�� Y ��ġ ������

	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_Cursor))
		{
			if (iter->m_iIndex == 0)
			{
				iter->m_fX = initialCursorX + m_iCurrentCol * deltaX;
				iter->m_fY = initialCursorY + (m_iCurrentRow - 1) * deltaY; // ù ��° �� ����
				iter->m_fAlpha = 255.f; // �� ��° ����ʹ� Ŀ���� ���̰� ����
			}
		}

		//if (typeid(*iter) == typeid(CUI_Inventory_BackGround))
		//{
		//	int index = iter->m_iIndex;
		//	iter->m_fX = initialInventoryX + m_iCurrentCol * deltaX;
		//	iter->m_fY = initialInventoryY + (m_iCurrentRow - 1) * deltaY + index * inventoryDeltaY; // ù ��° �� ����, index�� ���� y��ġ ����
		//}
	}
}

void CShop::UpdateSelectedItemInfo()
{
	int selectedIndex = 0;

	if (m_iCurrentRow > 0) {
		// �� ��° ����� ���� ������� �ε����� ���
		for (int i = 1; i < m_iCurrentRow; ++i) {
			selectedIndex += getMaxCols(i);
		}
		selectedIndex += m_iCurrentCol;

		// �ε����� ���� ���� ���õ� ������ ������ ������Ʈ
		if (selectedIndex >= 0 && selectedIndex < m_vecItemInfo.size()) {
			m_WstringTitle = m_vecItemInfo[selectedIndex].title;
			m_WstringExplain = m_vecItemInfo[selectedIndex].explain;
		}
	}
}

void CShop::ArrangeInventoryBackgrounds()
{
	const float deltaX = 145.0f; // �� �̵� ���� X ��ġ ������
	const float deltaY = -135.0f; // �� �̵� ���� Y ��ġ ������
	const float initialInventoryX = -275.0f; // Inventory_BackGround�� �ʱ� X ��ġ
	const float initialInventoryY = 55.0f; // Inventory_BackGround�� �ʱ� Y ��ġ
	const float inventoryDeltaY = 50.0f; // Inventory_BackGround�� Y ��ġ ������

	for (auto& iter : m_vecBackGroundObject)
	{
		int index = iter->m_iIndex;
		iter->m_fX = initialInventoryX + m_iCurrentCol * deltaX;
		iter->m_fY = initialInventoryY + (m_iCurrentRow - 1) * deltaY + index * inventoryDeltaY; // ù ��° �� ����, index�� ���� y��ġ ����
		iter->m_fAlpha = 255.f;
	}
}

void CShop::Late_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����

	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CShop::Render(_float fTimeDelta)
{
	if (!m_bIsOn) return S_OK; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����

	__super::Begin_RenderState();

	// �ؽ�Ʈ ����ȭ �� ������
	wchar_t text[256];

	// �ؽ�Ʈ ������ - CurrentPlayerMoney
	swprintf_s(text, L"%d", m_iCurrentMoney);
	RECT rect;
	SetRect(&rect, static_cast<int>(1130.f), static_cast<int>(50.f), 0, 0); // �ؽ�Ʈ�� ����� ��ġ
	m_pCurrentPlayerMoney_Font->DrawText(
		NULL,
		text,
		-1,
		&rect,
		DT_NOCLIP,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);

	// ù ��° ���� ��� �ؽ�Ʈ�� Ÿ��Ʋ�� ǥ������ ����
	if (m_iCurrentRow == 0)
	{
		__super::End_RenderState();
		return S_OK;
	}

	// ���� ���õ� �������� �ε��� ���
	int selectedIndex = 0;

	if (m_iCurrentRow > 0) {
		// �� ��° ����� ���� ������� �ε����� ���
		for (int i = 1; i < m_iCurrentRow; ++i) {
			selectedIndex += getMaxCols(i);
		}
		selectedIndex += m_iCurrentCol;
	}

	// ���õ� �������� ����� ������ ����
	if (selectedIndex < m_vecItemInfo.size()) {
		if (m_firstRowSelectedCol == 0) {
			// Hat�� ����
			m_WstringTitle = m_vecItemInfo[selectedIndex + m_iInitHatCount].title; // Item ������ Hat ���� ���Ŀ� ����
			m_WstringExplain = m_vecItemInfo[selectedIndex + m_iInitHatCount].explain;
		}
		else if (m_firstRowSelectedCol == 1) {
			// Item�� ����
			m_WstringTitle = m_vecItemInfo[selectedIndex].title;
			m_WstringExplain = m_vecItemInfo[selectedIndex].explain;
		}
	}

	// �ؽ�Ʈ ������ - CurrentItemTitle
	swprintf_s(text, L"%s", m_WstringTitle.c_str());
	SetRect(&rect, static_cast<int>(950.f), static_cast<int>(390.f), 0, 0); // �ؽ�Ʈ�� ����� ��ġ ����
	m_pCurrentItemTitle_Font->DrawText(
		NULL,
		text,
		-1,
		&rect,
		DT_NOCLIP,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);

	// �ؽ�Ʈ ������ - CurrentItemExplain
	swprintf_s(text, L"%s", m_WstringExplain.c_str());
	SetRect(&rect, static_cast<int>(950.f), static_cast<int>(460.f), 0, 0); // �ؽ�Ʈ�� ����� ��ġ ����
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
			// ��Ʈ ��ǥ�� ����
			int winCenterX = g_iWinSizeX / 2;
			int winCenterY = g_iWinSizeY / 2;
			// �ؽ�Ʈ ������ - BuyCount
			swprintf_s(text, L"%d��", m_iCurrentBuyCount);
			SetRect(&rect, static_cast<int>(winCenterX + m_vecBackGroundObject[2]->m_fX) + m_TextPosX, static_cast<int>(winCenterY - m_vecBackGroundObject[2]->m_fY) + m_TextPosY, 0, 0); // �ؽ�Ʈ�� ����� ��ġ ����
			m_pBuyCount_Font->DrawText(
				NULL,
				text,
				-1,
				&rect,
				DT_NOCLIP,
				D3DCOLOR_ARGB(255, 0, 0, 0)
			);

			// �ؽ�Ʈ ������ - BuyDecision
			swprintf_s(text, L"���");
			SetRect(&rect, static_cast<int>(winCenterX + m_vecBackGroundObject[1]->m_fX) + m_TextPosX, static_cast<int>(winCenterY - m_vecBackGroundObject[1]->m_fY) + m_TextPosY, 0, 0); // �ؽ�Ʈ�� ����� ��ġ ����
			m_pBuyDecision_Font->DrawText(
				NULL,
				text,
				-1,
				&rect,
				DT_NOCLIP,
				D3DCOLOR_ARGB(255, 0, 0, 0)
			);

			// �ؽ�Ʈ ������ - BuyCancel
			swprintf_s(text, L"�ݱ�");
			SetRect(&rect, static_cast<int>(winCenterX + m_vecBackGroundObject[0]->m_fX) + m_TextPosX, static_cast<int>(winCenterY - m_vecBackGroundObject[0]->m_fY) + m_TextPosY, 0, 0); // �ؽ�Ʈ�� ����� ��ġ ����
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

