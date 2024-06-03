#pragma once

#include "Client_Defines.h"
#include "UIObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CKeyState;

END

BEGIN(Client)

class CUI_Npc_Talk final : public CUIObject
{	
private:
	struct ItemInfo {
		wstring name;
		wstring talk;
	};
private:
	CUI_Npc_Talk(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CUI_Npc_Talk(const CUI_Npc_Talk& Prototype); /* 사본생성 시 */
	virtual ~CUI_Npc_Talk() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	void SetIsNpcTalkOn(_bool _isOn);
	void SetNpcTalkMessages(const vector<pair<wstring, wstring>>& messages);
	void AdvanceTalkCycle();
private:	
	CTexture*			m_pTextureCom = { nullptr };
	CTexture*			m_pArrow_Down_TextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CKeyState* m_pKeyCom = { nullptr };
	LPD3DXFONT			m_pName_Font = { nullptr };
	LPD3DXFONT			m_pTalk_Font = { nullptr };

private:
	HRESULT Ready_Components();
	void Font_Initialize();
	
private:
	wstring m_WstringName = { TEXT("명승호 저녁 일기") };
	wstring m_WstringTalk = TEXT(
		"저는 오늘 짜장면을 먹었습니다.\n"
		"군만두도 먹었습니다..\n"
		"지금 아주 배가 부릅니다.\n"
		"음료수가 먹고싶네요"
	);

	vector<pair<wstring, wstring>> m_vecMessages;
	_uint m_CurrentMessageIndex = { 0 };
	wstring m_DisplayText;
	_float m_TextPosX = 50.0f; // 텍스트 x 좌표
	_float m_TextPosY = 50.0f; // 텍스트 y 좌표
	_float m_fTextUpdateTime = {0.f};
	_float m_fTextDisplayInterval= { 0.1f };
	size_t m_CurrentCharIndex = { 0 };

	//크기 애니메이션
	_float m_fCreateTime = 0.0f; // 객체가 생성된 시간
	_float m_fGrowthDuration = 0.5f; // n초간 커지는 시간
	_bool m_bGrowthComplete = false; // 성장 완료 여부
	_bool m_bIsShrinking = { false };
	
public:
	_bool m_bIsNpcTalkOn = { false };
private:
	_float m_fArrowOffsetY = 0.f; // 화살표의 Y 오프셋
	_float m_fArrowAnimDuration = 0.5f; // 애니메이션 시간
	_float m_fArrowAnimTime = 0.0f; // 애니메이션 경과 시간
	_float m_fArrowOffsetRange = 10.0f; // 화살표의 Y 등락폭
	_bool m_bArrowAnimUp = true; // 화살표가 올라가는 중인지 여부
public:
	/* 원형객체를 생성한다. */
	static CUI_Npc_Talk* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END