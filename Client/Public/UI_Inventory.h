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
END


BEGIN(Client)

class CInventory final : public CGameObject
{
private:
	CInventory(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CInventory(const CInventory& Prototype); /* �纻���� �� */
	virtual ~CInventory() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	HRESULT Ready_Components();

	void Control_FirstRow();
	void Control_OtherRow();
	void UpdateAlphaValues();

	_uint getMaxCols(_uint row) const {
		return (row == 0) ? m_firstRowCols : m_otherRowsCols;
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
	CKeyState* m_pKeyCom = { nullptr };

private:
	_bool m_bIsOn = { true };

	_uint m_currentRow = { 0 };// ���� ���õ� �������� ��
	_uint m_currentCol = { 0 }; // ���� ���õ� �������� ��
	const _uint m_firstRowCols = { 4 }; // ù ��° ���� �� ��
	const _uint m_otherRowsCols = { 5 }; // ������ ���� �� ��
	_uint m_maxRows = { 4 }; // �κ��丮�� �� ��
	int m_previousRow = -1; // ���� ���õ� �������� ��
	int m_previousCol = -1; // ���� ���õ� �������� ��

	_float4x4			m_ViewMatrix = {};
	_float4x4			m_ProjMatrix = {};

private:
	vector<CUIObject*> m_vecUIObject;
	vector<CUIObject*> m_vecHatObject;
	vector<CUIObject*> m_vecItemObject;

public:
	/* ������ü�� �����Ѵ�. */
	static CInventory* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END