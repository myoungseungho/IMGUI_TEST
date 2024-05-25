#include "stdafx.h"
#include "..\Public\UI_Inventory.h"

#include "UI_ItemTabIcon_Caution.h"
#include "UI_ItemTabIcon_Food.h"
#include "UI_ItemTabIcon_Hat.h"
#include "UI_ItemTabIcon_Leaf.h"
#include "UI_Cursor.h"
#include "UI_Inventory_BackGround.h"
#include "GameInstance.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CInventory::CInventory(const CInventory& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CInventory::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.0f, 1.f);


	LEVELID currentLevel = (LEVELID)m_pGameInstance->GetLoadingLevelIndex();

	//UI������Ʈ �޾Ƽ� VECTOR�� �ֱ�
	auto AddUIObject = [&](const TCHAR* prototypeTag, const TCHAR* layerTag, void* pArg = nullptr, const _uint count = 0) -> HRESULT {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, prototypeTag, layerTag, pArg)))
			return E_FAIL;
		CUIObject* pUIObject = static_cast<CUIObject*>(m_pGameInstance->Get_GameObject(currentLevel, layerTag, count));
		if (!pUIObject)
			return E_FAIL;
		Safe_AddRef(pUIObject);
		m_vecUIObject.push_back(pUIObject);
		return S_OK;
		};

	//UI������Ʈ �޾Ƽ� HatVector�� �ֱ�
	auto AddHatUIObject = [&](const TCHAR* prototypeTag, const TCHAR* layerTag, void* pArg = nullptr, const _uint count = 0) -> HRESULT {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, prototypeTag, layerTag, pArg)))
			return E_FAIL;
		CUIObject* pUIObject = static_cast<CUIObject*>(m_pGameInstance->Get_GameObject(currentLevel, layerTag, count));
		if (!pUIObject)
			return E_FAIL;
		Safe_AddRef(pUIObject);
		m_vecHatObject.push_back(pUIObject);
		return S_OK;
		};

	//UI������Ʈ �޾Ƽ� ItemVector�� �ֱ�
	auto AddItemUIObject = [&](const TCHAR* prototypeTag, const TCHAR* layerTag, void* pArg = nullptr, const _uint count = 0) -> HRESULT {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, prototypeTag, layerTag, pArg)))
			return E_FAIL;
		CUIObject* pUIObject = static_cast<CUIObject*>(m_pGameInstance->Get_GameObject(currentLevel, layerTag, count));
		if (!pUIObject)
			return E_FAIL;
		Safe_AddRef(pUIObject);
		m_vecItemObject.push_back(pUIObject);
		return S_OK;
		};

	UIDATA slotData{};

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_FadeInOut"), TEXT("Layer_UI_FadeInOut"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Cursor"), TEXT("Layer_UI_ZCursor"))))
		return E_FAIL;


	for (size_t i = 0; i < 2; i++)
	{
		// �� ���Կ� ���� ��ġ�� ũ�� ����
		switch (i)
		{
		case 0:
			slotData.position = { 455.f, 130.f };
			slotData.scale = { 320.f, 225.f };
			slotData.alpha = 66.f;
			break;
		case 1:
			slotData.position = { -430.f, 80.f };
			slotData.scale = { 140.f, 145.f };
			slotData.alpha = 104.f;
			break;
		}

		slotData.index = i;

		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_BackGround"), TEXT("Layer_UI_Inventory_XBackGround"), &slotData, i)))
			return E_FAIL;
	}

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_DotLine"), TEXT("Layer_UI_Inventory_DotLine"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_Slot"), TEXT("Layer_UI_Inventory_Slot"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_VerticalDotLine"), TEXT("Layer_UI_Inventory_VerticalDotLine"))))
		return E_FAIL;

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

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Shop_PriceTag"), TEXT("Layer_UI_Shop_PriceTag"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_SlotBeigeBackground"), TEXT("Layer_UI_Inventory_SlotBeigeBackground"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_Button"), TEXT("Layer_UI_Inventory_Button"))))
		return E_FAIL;


	for (size_t i = 0; i < 5; i++)
	{
		// �� ���Կ� ���� ��ġ�� ũ�� ����
		switch (i)
		{
		case 0:
			slotData.position = { -430.f, 80.f };
			slotData.scale = { 160.f, 165.f };
			slotData.alpha = 66.f;
			break;
		case 1:
			slotData.position = { -430.f, 80.f };
			slotData.scale = { 160.f, 165.f };
			slotData.alpha = 104.f;
			break;
		case 2:
			slotData.position = { -430.f, 80.f };
			slotData.scale = { 160.f, 165.f };
			slotData.alpha = 104.f;
			break;
		case 3:
			slotData.position = { -430.f, 80.f };
			slotData.scale = { 160.f, 165.f };
			slotData.alpha = 104.f;
			break;
		case 4:
			slotData.position = { -430.f, 80.f };
			slotData.scale = { 160.f, 160.f };
			slotData.alpha = 104.f;
			break;
		}

		slotData.index = i;


		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Hat"), TEXT("Layer_UI_Hat"), &slotData, i)))
			return E_FAIL;
		if (FAILED(AddHatUIObject(TEXT("Prototype_GameObject_UI_Hat"), TEXT("Layer_UI_Hat"), &slotData, i)))
			return E_FAIL;
	}


	// �ʱ� ���� ����
	UpdateAlphaValues();
	return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
}

void CInventory::Update(_float fTimeDelta)
{
	if (!m_bIsOn)
		return;

	bool positionChanged = false;

	if (m_pKeyCom->Key_Down(VK_UP))
	{
		// ���� ����Ű �Է� ó��
		if (m_currentRow > 1) {
			// �� ��° �� �̻��� �� ó��
			m_currentRow = (m_currentRow - 1 + m_maxRows) % m_maxRows;
			m_currentCol = min(m_currentCol, getMaxCols(m_currentRow) - 1);
			positionChanged = true;
		}
		else if (m_currentRow == 1) {
			// �� ��° �࿡�� ù ��° ������ �̵� ��
			m_currentRow = 0;
			m_currentCol = 0; // ù ��° ���� ù ��° ���� �̵�
			positionChanged = true;
		}
	}
	if (m_pKeyCom->Key_Down(VK_DOWN))
	{
		// �Ʒ��� ����Ű �Է� ó��
		if (m_currentRow == m_maxRows - 1) {
			// ������ �࿡�� �Ʒ������� �̵� �� �� ��° ������ �̵�
			m_currentRow = 1;
			positionChanged = true;
		}
		else if (m_currentRow < m_maxRows - 1) {
			m_currentRow = (m_currentRow + 1) % m_maxRows;
			positionChanged = true;
		}
		m_currentCol = min(m_currentCol, getMaxCols(m_currentRow) - 1);
	}
	if (m_pKeyCom->Key_Down(VK_LEFT))
	{
		// ���� ����Ű �Է� ó��
		int maxCols = getMaxCols(m_currentRow);
		m_currentCol = (m_currentCol - 1 + maxCols) % maxCols;
		positionChanged = true;
	}
	if (m_pKeyCom->Key_Down(VK_RIGHT))
	{
		// ������ ����Ű �Է� ó��
		int maxCols = getMaxCols(m_currentRow);
		m_currentCol = (m_currentCol + 1) % maxCols;
		positionChanged = true;
	}

	if (positionChanged) {
		UpdateAlphaValues();
		m_previousRow = m_currentRow;
		m_previousCol = m_currentCol;
	}

	// ���� ���õ� �������� �ε����� ���
	int selectedIndex = 0;

	if (m_currentRow > 0) {
		// �� ��° ����� ���� ������� �ε����� ���
		for (int i = 1; i < m_currentRow; ++i) {
			selectedIndex += getMaxCols(i);
		}
		selectedIndex += m_currentCol;
	}
	else {
		// ù ��° ���� ���
		selectedIndex = m_currentCol;
	}

	// ���õ� ������ ó�� (��: ���̶���Ʈ ǥ��)
	// HandleSelection(selectedIndex); // ������ �Լ�
}

void CInventory::UpdateAlphaValues()
{
	if (m_currentRow == 0) {
		Control_FirstRow();
	}
	else {
		Control_OtherRow();
	}
}

void CInventory::Control_FirstRow()
{
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_ItemTabIcon_Hat)) {
			iter->m_fAlpha = (m_currentCol == 0) ? 255.f : 150.f;
		}
		else if (typeid(*iter) == typeid(CUI_ItemTabIcon_Food)) {
			iter->m_fAlpha = (m_currentCol == 1) ? 255.f : 150.f;
		}
		else if (typeid(*iter) == typeid(CUI_ItemTabIcon_Leaf)) {
			iter->m_fAlpha = (m_currentCol == 2) ? 255.f : 150.f;
		}
		else if (typeid(*iter) == typeid(CUI_ItemTabIcon_Caution)) {
			iter->m_fAlpha = (m_currentCol == 3) ? 255.f : 150.f;
		}
	}

	// ù ��° ���� �� CUI_Cursor�� ����
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_Cursor))
		{
			iter->m_fAlpha = 0.f;
		}
		else if (typeid(*iter) == typeid(CUI_Inventory_BackGround))
		{
			// Ư�� ���ǿ� �´� CUI_Inventory_BackGround ��ü�� ����
			if (iter->m_iIndex == 1) {
				iter->m_fAlpha = 0.f;
			}
		}

	}
}

void CInventory::Control_OtherRow()
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

	// CUI_Cursor ��ü�� CUI_Inventory_BackGround ��ü�� ��ġ�� ������Ʈ
	const float initialX = -430.0f; // ù ��° ���� �ʱ� X ��ġ
	const float initialY = 85.0f; // ù ��° ���� �ʱ� Y ��ġ
	const float deltaX = 145.0f; // �� �̵� ���� X ��ġ ������
	const float deltaY = -130.0f; // �� �̵� ���� Y ��ġ ������

	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_Cursor))
		{
			iter->m_fX = initialX + m_currentCol * deltaX;
			iter->m_fY = initialY + (m_currentRow - 1) * deltaY; // ù ��° �� ����
			iter->m_fAlpha = 255.f; // �� ��° ����ʹ� Ŀ���� ���̰� ����
		}
		else if (typeid(*iter) == typeid(CUI_Inventory_BackGround))
		{
			// Ư�� ���ǿ� �´� CUI_Inventory_BackGround ��ü�� ����
			if (iter->m_iIndex == 1) {
				iter->m_fX = initialX + m_currentCol * deltaX;
				iter->m_fY = initialY + (m_currentRow - 1) * deltaY; // ù ��° �� ����
				iter->m_fAlpha = 104.f; // �� ��° ����ʹ� Ŀ���� ���̰� ����
			}
		}
	}
}


void CInventory::Late_Update(_float fTimeDelta)
{

}

HRESULT CInventory::Render(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CInventory::Ready_Components()
{
	/* For.Com_KeyState */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Key"),
		TEXT("Com_KeyState"), reinterpret_cast<CComponent**>(&m_pKeyCom))))
		return E_FAIL;

	return S_OK;
}


CInventory* CInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInventory* pInstance = new CInventory(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Inventory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CInventory::Clone(void* pArg)
{
	CInventory* pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Inventory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory::Free()
{
	for (auto& pUIObject : m_vecUIObject)
	{
		Safe_Release(pUIObject);
	}
	m_vecUIObject.clear();

	for (auto& pUIObject : m_vecHatObject)
	{
		Safe_Release(pUIObject);
	}
	m_vecHatObject.clear();

	for (auto& pUIObject : m_vecItemObject)
	{
		Safe_Release(pUIObject);
	}
	m_vecItemObject.clear();

	Safe_Release(m_pKeyCom);
	__super::Free();
}

