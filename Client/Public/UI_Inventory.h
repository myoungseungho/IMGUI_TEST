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
	struct ItemInfo {
		wstring title;
		wstring explain;
		_uint count; // 추가
	};

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

	void Font_Initialize();
	void UpdateSelectedItemInfo();

	void Control_FirstRow();
	void Control_OtherRow();
	void UpdateAlphaValues();
	void ShowHats();
	void ShowItems();
	void HideHats();
	void HideItems();
	void InitializeInventoryDisplay();
		_uint getMaxCols(_uint row) const {
		return (row == 0) ? m_iFirstRowCols : m_iOtherRowsCols;
	}

	void EquipHat();
	void EquipItem();
	void AddToQuickInventory(_uint slot);
	void UseQuickInventory_Item(_uint slot);
public:
	void SetInventoryOnOff();


	void AddItemToInventory(const wstring& itemName, const wstring& hatoritem, _uint count);
	void SetMoney(_uint money);
private:
	CTransform* m_pTransformCom = { nullptr };
	CKeyState* m_pKeyCom = { nullptr };
	LPD3DXFONT m_pCurrentPlayerMoney_Font = { nullptr };
	LPD3DXFONT m_pCurrentItemTitle_Font = { nullptr };
	LPD3DXFONT m_pCurrentItemExplain_Font = { nullptr };
	LPD3DXFONT m_pItemCountFont = { nullptr };
public:
	_bool m_bIsOn = { false };
private:

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
	vector<CUIObject*> m_vecQuickInventory; // 간이 인벤토리 (최대 4개의 슬롯)
	vector<CUIObject*> m_vecUIObject; // 플레이어 인벤토리에서 만든 모든 UI 오브젝트 ESC용
	vector<CUIObject*> m_vecCurrentHaveHat;
	vector<CUIObject*> m_vecCurrentHaveItem;
	unordered_map<_uint, _uint> m_hatIndexMap; // Hat의 selectedIndex와 itemIndex를 매핑
	unordered_map<_uint, _uint> m_itemIndexMap; // Item의 selectedIndex와 itemIndex를 매핑

private:
	_uint m_iCurrentMoney = { 10000 };
	_float m_TextPosX = 31.f; // 텍스트 x 좌표
	_float m_TextPosY = -36.f; // 텍스트 y 좌표
	wstring m_WstringTitle = { TEXT("타이틀 테스트") };
	wstring m_WstringExplain = { TEXT("설명 테스트") };

	vector<ItemInfo> m_vecHatInfo = {
   {L"밀짚모자", L"햇볕을 가려주는\n평범한 밀짚모자입니다."},
	{L"인디언 평민의 모자", L"인디언 평민이\n즐겨 쓰던 모자입니다."},
	{L"인디언 족장의 모자", L"인디언 족장만이\n쓸 수 있는 특별한 모자입니다."},
	{L"나뭇잎 모자", L"싱그러운 나뭇잎으로\n만들어진 모자입니다."},
	{L"부엉이 모자", L"부엉이의 깃털로\n장식된 독특한 모자입니다."},
	{L"인디언 괴물의 모자", L"인디언 전설 속 괴물이\n썼을 것으로 추정됩니다."},
	{L"전등 모자", L"어둠 속에서도 빛을 발하는\n실용적인 전등 모자입니다."},
	{L"명승호의 모자", L"명승호가 즐겨 쓰던\n모자입니다. 고풍스럽습니다."},
	{L"오페라의 유령", L"오페라 유령의 마법이\n담겨 있는 신비로운 모자입니다."},
	{L"콧수염 모자", L"귀여운 콧수염 장식이\n달려 있는 재미있는 모자입니다."},
	{L"전사의 모자", L"전사들이 전투 시\n착용했던 용맹한 모자입니다."},
	{L"애벌레의 모자", L"애벌레 모양의\n귀여운 모자입니다."},
	{L"선비의 모자", L"지혜로운 선비들이\n쓰던 전통적인 모자입니다."},
	{L"화환", L"화려한 꽃으로 장식된\n아름다운 화환 모자입니다."},
	{L"사과 모자", L"신선한 사과 모양의\n상큼한 모자입니다."},
	};

	vector<ItemInfo> m_vecItemInfo = {
	{L"반쪽 짜리 사과", L"한 입 베어 물린\n반쪽 사과입니다."},
	{L"사과", L"신선하고\n달콤한 사과입니다."},
	{L"먹음직스러운 사과", L"눈으로만 봐도\n침이 고이는 사과입니다."},
	{L"나뭇잎", L"싱그러운\n초록빛 나뭇잎입니다."},
	{L"풍성한 나뭇잎", L"무성하게 자란\n건강한 나뭇잎입니다."},
	{L"전설의 나뭇잎", L"오랜 전설에\n등장하는 신비한 나뭇잎입니다."},
	{L"잎파리", L"작고 귀여운\n잎파리 모양입니다."},
	{L"막대기", L"튼튼한 나무로\n만들어진 막대기입니다."},
	{L"사슴벌레", L"강력한 집게를\n가진 사슴벌레입니다."},
	{L"장수풍뎅이", L"날카로운 뿔을 가진\n위엄있는 장수풍뎅이입니다."},
	{L"꼬깔콘", L"재미있는 모양의\n바삭한 꼬깔콘입니다."},
	{L"전등모자", L"어두운 곳을 밝히는\n실용적인 전등모자입니다."},
	{L"오페라의 유령", L"오페라의 유령이\n착용했던 신비한 모자입니다."},
	{L"헤드폰", L"고음질 사운드를\n제공하는 헤드폰입니다."},
	{L"손오공", L"전설의 손오공이\n쓰던 모자입니다."}
	};

public:
	/* 원형객체를 생성한다. */
	static CInventory* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END