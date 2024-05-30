#pragma once

#include "Client_Defines.h"
#include "UIObject.h"
#include "UI_FadeInOut.h"
#include "UI_Cursor.h"
#include "UI_Inventory_BackGround.h"
#include "UI_Inventory_DotLine.h"
#include "UI_Inventory_Slot.h"
#include "UI_Inventory_VerticalDotLine.h"
#include "UI_ItemTabIcon_Caution.h"
#include "UI_ItemTabIcon_Food.h"
#include "UI_ItemTabIcon_Hat.h"
#include "UI_ItemTabIcon_Leaf.h"
#include "UI_Shop_ItemInfo_TextBox.h"
#include "UI_Shop_PlayerCoin.h"
#include "UI_Shop_PriceTag.h"


BEGIN(Engine)
class CKeyState;
class CTransform;
END


BEGIN(Client)

class CShop final : public CUIObject
{
	struct ItemInfo {
		std::wstring title;
		std::wstring explain;
	};

private:
	CShop(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CShop(const CShop& Prototype); /* �纻���� �� */
	virtual ~CShop() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	HRESULT Ready_Components();

	void Font_Initialize();
	void UpdateSelectedItemInfo();

	void Control_FirstRow();
	void Control_OtherRow();
	void UpdateAlphaValues();
	void ShowHats();
	void ShowItems();
	_uint getMaxCols(_uint row) const {
		return (row == 0) ? m_iFirstRowCols : m_iOtherRowsCols;
	}

public:
	void SetInventoryOnOff()
	{
		m_bIsOn = !m_bIsOn;

		for (auto& iter : m_vecUIObject)
		{
			iter->m_bIsOn = m_bIsOn;
		}
	}
private:
	CTransform* m_pTransformCom = { nullptr };
	CKeyState* m_pKeyCom = { nullptr };
	LPD3DXFONT m_pCurrentPlayerMoney_Font = { nullptr };
	LPD3DXFONT m_pCurrentItemTitle_Font = { nullptr };
	LPD3DXFONT m_pCurrentItemExplain_Font = { nullptr };

private:
	_bool m_bIsOn = { false };

	_uint m_iCurrentRow = { 0 };// ���� ���õ� �������� ��
	_uint m_iCurrentCol = { 0 }; // ���� ���õ� �������� ��
	const _uint m_iFirstRowCols = { 4 }; // ù ��° ���� �� ��
	const _uint m_iOtherRowsCols = { 5 }; // ������ ���� �� ��
	const _uint m_iMaxRows = { 4 }; // �κ��丮�� �� ��
	_uint m_iPreviousRow = -1; // ���� ���õ� �������� ��
	_uint m_iPreviousCol = -1; // ���� ���õ� �������� ��
	const _uint m_iInitHatCount = { 15 };
	const _uint m_iInitItemCount = { 15 };

	_float4x4			m_ViewMatrix = {};
	_float4x4			m_ProjMatrix = {};

private:
	_uint m_firstRowSelectedCol = { 0 }; // ù ��° �࿡�� ���õ� �� ��ġ
	vector<CUIObject*> m_vecUIObject; // �÷��̾� �κ��丮���� ���� ��� UI ������Ʈ ESC��

private:
	_uint m_iCurrentMoney = { 5000 };
	_float m_TextPosX = 50.0f; // �ؽ�Ʈ x ��ǥ
	_float m_TextPosY = 50.0f; // �ؽ�Ʈ y ��ǥ
	wstring m_WstringTitle = { TEXT("Ÿ��Ʋ �׽�Ʈ") };
	wstring m_WstringExplain = { TEXT("���� �׽�Ʈ") };

	vector<ItemInfo> m_vecHatInfo = {
   {L"��¤����", L"�޺��� �����ִ�\n����� ��¤�����Դϴ�."},
	{L"�ε�� ����� ����", L"�ε�� �����\n��� ���� �����Դϴ�."},
	{L"�ε�� ������ ����", L"�ε�� ���常��\n�� �� �ִ� Ư���� �����Դϴ�."},
	{L"������ ����", L"�̱׷��� ����������\n������� �����Դϴ�."},
	{L"�ξ��� ����", L"�ξ����� ���з�\n��ĵ� ��Ư�� �����Դϴ�."},
	{L"�ε�� ������ ����", L"�ε�� ���� �� ������\n���� ������ �����˴ϴ�."},
	{L"���� ����", L"��� �ӿ����� ���� ���ϴ�\n�ǿ����� ���� �����Դϴ�."},
	{L"���ȣ�� ����", L"���ȣ�� ��� ����\n�����Դϴ�. ��ǳ�������ϴ�."},
	{L"������� ����", L"����� ������ ������\n��� �ִ� �ź�ο� �����Դϴ�."},
	{L"����� ����", L"�Ϳ��� ����� �����\n�޷� �ִ� ����ִ� �����Դϴ�."},
	{L"������ ����", L"������� ���� ��\n�����ߴ� ����� �����Դϴ�."},
	{L"�ֹ����� ����", L"�ֹ��� �����\n�Ϳ��� �����Դϴ�."},
	{L"������ ����", L"�����ο� �������\n���� �������� �����Դϴ�."},
	{L"ȭȯ", L"ȭ���� ������ ��ĵ�\n�Ƹ��ٿ� ȭȯ �����Դϴ�."},
	{L"��� ����", L"�ż��� ��� �����\n��ŭ�� �����Դϴ�."},
	};

	vector<ItemInfo> m_vecItemInfo = {
	{L"���� ¥�� ���", L"�� �� ���� ����\n���� ����Դϴ�."},
	{L"���", L"�ż��ϰ�\n������ ����Դϴ�."},
	{L"������������ ���", L"�����θ� ����\nħ�� ���̴� ����Դϴ�."},
	{L"������", L"�̱׷���\n�ʷϺ� �������Դϴ�."},
	{L"ǳ���� ������", L"�����ϰ� �ڶ�\n�ǰ��� �������Դϴ�."},
	{L"������ ������", L"���� ������\n�����ϴ� �ź��� �������Դϴ�."},
	{L"���ĸ�", L"�۰� �Ϳ���\n���ĸ� ����Դϴ�."},
	{L"�����", L"ưư�� ������\n������� ������Դϴ�."},
	{L"�罿����", L"������ ���Ը�\n���� �罿�����Դϴ�."},
	{L"���ǳ����", L"��ī�ο� ���� ����\n�����ִ� ���ǳ�����Դϴ�."},
	{L"������", L"����ִ� �����\n�ٻ��� �������Դϴ�."},
	{L"�������", L"��ο� ���� ������\n�ǿ����� ��������Դϴ�."},
	{L"������� ����", L"������� ������\n�����ߴ� �ź��� �����Դϴ�."},
	{L"�����", L"������ ���带\n�����ϴ� ������Դϴ�."},
	{L"�տ���", L"������ �տ�����\n���� �����Դϴ�."}
	};

public:
	/* ������ü�� �����Ѵ�. */
	static CShop* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END