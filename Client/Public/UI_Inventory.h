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

BEGIN(Client)

class CUI_Inventory final : public CGameObject
{
private:
	CUI_Inventory(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CUI_Inventory(const CUI_Inventory& Prototype); /* 사본생성 시 */
	virtual ~CUI_Inventory() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

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
	_bool m_bIsOn = { false };
	_float4x4			m_ViewMatrix = {};
	_float4x4			m_ProjMatrix = {};

private:
	vector<CUIObject*> m_vecUIObject;

public:
	/* 원형객체를 생성한다. */
	static CUI_Inventory* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END