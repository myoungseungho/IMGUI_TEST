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

class CUI_MapGuide final : public CUIObject
{
private:
	struct ItemInfo {
		wstring name;
	};

private:
	CUI_MapGuide(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CUI_MapGuide(const CUI_MapGuide& Prototype); /* 사본생성 시 */
	virtual ~CUI_MapGuide() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	LPD3DXFONT			m_pName_Font = { nullptr };

private:
	HRESULT Ready_Components();
	void Font_Initialize();

public:
	void SetIsNpcTalkOn(_bool _isOn);
	void UpdatePosition(_float fTimeDelta);
	void SetMapGuideText(const wstring& text)
	{
		m_WstringName = text;
	}
private:
	wstring m_WstringName = { TEXT("명승호 저녁 일기") };

	_float m_TextPosX = 50.0f; // 텍스트 x 좌표
	_float m_fTextY = { 0.f }; // 텍스트 y 좌표
	_float m_fTextTargetY = { 34.8f };
	_float m_fTextStartY = { -10.f };
	_float m_fCreateTime = { 0.f };
	_float m_fTargetY = 305.f;
	_float m_fStartY = 350.f;
	_float m_fAnimDuration = 2.0f; // 애니메이션 지속 시간
	_float m_fAnimTime = 0.0f; // 애니메이션 경과 시간
	_float m_fWaitTime = 0.0f; // 대기 시간
	_float m_fWaitDuration = 1.0f; // 대기 지속 시간 (예: 2초)
private:
	_bool m_bIsMapGuide = { true };
	_bool m_bIsReturning = { false };
public:
	/* 원형객체를 생성한다. */
	static CUI_MapGuide* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END