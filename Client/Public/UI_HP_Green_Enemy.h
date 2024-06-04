#pragma once

#include "Client_Defines.h"
#include "UIObject.h"
#include "Monster.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_HP_Green_Enemy final : public CUIObject
{	
private:
	CUI_HP_Green_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CUI_HP_Green_Enemy(const CUI_HP_Green_Enemy& Prototype); /* �纻���� �� */
	virtual ~CUI_HP_Green_Enemy() = default;

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
private:
	CMonster* m_pEnemyCopy = { nullptr };
	_uint    m_iEnemyCurrentHp = { 0 };
	_uint    m_iEnemyMaxHp = { 0 };
private:
	HRESULT Ready_Components();

public:
	/* ������ü�� �����Ѵ�. */
	static CUI_HP_Green_Enemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END