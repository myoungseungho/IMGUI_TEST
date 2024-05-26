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

class CInventory final : public CUIObject
{
private:
	CInventory(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CInventory(const CInventory& Prototype); /* 사본생성 시 */
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
	void ShowHats();
	void ShowItems();
	_uint getMaxCols(_uint row) const {
		return (row == 0) ? m_iFirstRowCols : m_iOtherRowsCols;
	}

	void EquipHat();
	void EquipItem();
	void AddToQuickInventory(_uint slot);
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

private:
	_bool m_bIsOn = { true };

	_uint m_iCurrentRow = { 0 };// 현재 선택된 아이템의 행
	_uint m_iCurrentCol = { 0 }; // 현재 선택된 아이템의 열
	const _uint m_iFirstRowCols = { 4 }; // 첫 번째 행의 열 수
	const _uint m_iOtherRowsCols = { 5 }; // 나머지 행의 열 수
	const _uint m_iMaxRows = { 4 }; // 인벤토리의 행 수
	_uint m_iPreviousRow = -1; // 이전 선택된 아이템의 행
	_uint m_iPreviousCol = -1; // 이전 선택된 아이템의 열
	const _uint m_iInitHatCount = { 15 };
	const _uint m_iInitItemCount = { 15 };

	_float4x4			m_ViewMatrix = {};
	_float4x4			m_ProjMatrix = {};

private:
	_uint m_firstRowSelectedCol = { 0 }; // 첫 번째 행에서 선택된 열 위치
	CUIObject* m_currentEquipHat = { nullptr }; // 현재 착용된 Hat
	CUIObject* m_currentEquipItem = { nullptr }; // 현재 착용된 Item
	vector<CUIObject*> m_quickInventory; // 간이 인벤토리 (최대 4개의 슬롯)
	vector<CUIObject*> m_vecUIObject; // 플레이어 인벤토리에서 만든 모든 UI 오브젝트 ESC용

private:
	_uint m_iCurrentMoney = { 5000};
	_float m_TextPosX = 50.0f; // 텍스트 x 좌표
	_float m_TextPosY = 50.0f; // 텍스트 y 좌표
public:
	/* 원형객체를 생성한다. */
	static CInventory* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END