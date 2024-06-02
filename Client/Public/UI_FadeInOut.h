#pragma once

#include "Client_Defines.h"
#include "UIObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_FadeInOut final : public CUIObject
{	
private:
	CUI_FadeInOut(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CUI_FadeInOut(const CUI_FadeInOut& Prototype); /* 사본생성 시 */
	virtual ~CUI_FadeInOut() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	HRESULT Ready_Components();

private:	
	CTexture*			m_pTextureCom = { nullptr };
	CTexture*			m_pEffectTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };

private:
	_bool m_bIsFading = false;
	_bool m_bIncreasingAlpha = false;
	_float m_fStartAlpha = 0.f;
	_float m_fEndAlpha = 255.f;
	_float m_fFadeDuration = 1.f;
	_float m_fFadeElapsedTime = 0.f;
	_float m_fDefaultAlpha = 125.f;  // Default alpha value

public:
	void StartFading(_float fDuration, _float fStartAlpha, _float fEndAlpha);

public:
	/* 원형객체를 생성한다. */
	static CUI_FadeInOut* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END