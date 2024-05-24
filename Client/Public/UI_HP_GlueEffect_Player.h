#pragma once

#include "Client_Defines.h"
#include "UIObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_HP_GlueEffect_Player final : public CUIObject
{	
private:
	CUI_HP_GlueEffect_Player(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CUI_HP_GlueEffect_Player(const CUI_HP_GlueEffect_Player& Prototype); /* 사본생성 시 */
	virtual ~CUI_HP_GlueEffect_Player() = default;

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
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };

private:
	float m_fAlpha = { 0.f };               // Current alpha value
	bool m_bIncreasingAlpha = { true };     // Is alpha value increasing
	float m_fElapsedTime = { 0.f };       // Elapsed time for alpha animation
	float m_fAlphaAnimationDuration = { 0.5f }; // Duration for alpha increase/decrease
	float m_fMinAlpha = { 0.f };           // Minimum alpha value
	float m_fMaxAlpha = { 50.f };         // Maximum alpha value

public:
	/* 원형객체를 생성한다. */
	static CUI_HP_GlueEffect_Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END