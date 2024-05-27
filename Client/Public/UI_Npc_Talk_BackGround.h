#pragma once

#include "Client_Defines.h"
#include "UIObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Npc_Talk_BackGround final : public CUIObject
{	
private:
	struct ItemInfo {
		wstring name;
		wstring talk;
	};
private:
	CUI_Npc_Talk_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CUI_Npc_Talk_BackGround(const CUI_Npc_Talk_BackGround& Prototype); /* 사본생성 시 */
	virtual ~CUI_Npc_Talk_BackGround() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:	
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	LPD3DXFONT			m_pName_Font = { nullptr };
	LPD3DXFONT			m_pTalk_Font = { nullptr };

private:
	HRESULT Ready_Components();
	void Font_Initialize();
private:
	wstring m_WstringName = { TEXT("이름 테스트") };
	wstring m_WstringTalk = { TEXT("토크 테스트") };
	_float m_TextPosX = 50.0f; // 텍스트 x 좌표
	_float m_TextPosY = 50.0f; // 텍스트 y 좌표
public:
	/* 원형객체를 생성한다. */
	static CUI_Npc_Talk_BackGround* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END