#pragma once

#include "Base.h"

/* 1. 화면에 그려야할 객체들을 그리는 순서대로 보관한다. */
/* 2. 보관하고 있는 객체들의 렌더함수를 호출한다.(렌더콜) */

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RG_PRIORITY, RG_NONBLEND, RG_BLEND, RG_UI, RG_END };
private:
	CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Draw();

private:
	LPDIRECT3DDEVICE9					m_pGraphic_Device = { nullptr };
	list<class CGameObject*>			m_RenderObjects[RG_END];

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END