#pragma once

#include "Client_Defines.h"
#include "UIObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_HP_Enemy final : public CUIObject
{	
private:
	CUI_HP_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CUI_HP_Enemy(const CUI_HP_Enemy& Prototype); /* 사본생성 시 */
	virtual ~CUI_HP_Enemy() = default;

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
private:
	class CMonster* m_pEnemyCopy = { nullptr };
	_uint m_iEnemyCurrentHp = { 0 };
	_uint m_iEnemyMaxHp = { 0 };

	float m_TextPosX = 50.0f; // 텍스트 x 좌표
	float m_TextPosY = 50.0f; // 텍스트 y 좌표

private:
	HRESULT Ready_Components();

public:
	/* 원형객체를 생성한다. */
	static CUI_HP_Enemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END