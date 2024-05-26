#pragma once

#include "Client_Defines.h"
#include "UIObject.h"


BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
class CAnimator;
END

BEGIN(Client)

class CUI_Loading_Orgu_Run final : public CUIObject
{
private:
	CUI_Loading_Orgu_Run(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CUI_Loading_Orgu_Run(const CUI_Loading_Orgu_Run& Prototype); /* 사본생성 시 */
	virtual ~CUI_Loading_Orgu_Run() = default;

	enum ANIMATION_STATE {
		ANIM_IDLE,
	};

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CAnimator* m_pAnimCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Ready_Animation();
	void AnimState(_float _fTimeDelta);

private:
	ANIMATION_STATE m_eAnimState = ANIM_IDLE;

public:
	/* 원형객체를 생성한다. */
	static CUI_Loading_Orgu_Run* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END