#pragma once

#include "Client_Defines.h"
#include "UIObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_HP_Player final : public CUIObject
{	
private:
	CUI_HP_Player(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CUI_HP_Player(const CUI_HP_Player& Prototype); /* 사본생성 시 */
	virtual ~CUI_HP_Player() = default;

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
	LPD3DXFONT m_pFont = { nullptr };

	float m_TextPosX = 50.0f; // 텍스트 x 좌표
	float m_TextPosY = 50.0f; // 텍스트 y 좌표

private:
	HRESULT Ready_Components();

public:
	/* 원형객체를 생성한다. */
	static CUI_HP_Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END