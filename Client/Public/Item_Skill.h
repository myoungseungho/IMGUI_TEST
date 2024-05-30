#pragma once

#include "Client_Defines.h"
#include "EnviormentObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollider;
class CCalc_Timer;
END

BEGIN(Client)

class CItem_Skill final : public CEnviormentObject
{	
public:
typedef struct
	{
		CTransform* pTargetTransform = { nullptr };
	}ITEM_SKILL_DESC;

private:
	CItem_Skill(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CItem_Skill(const CItem_Skill& Prototype); /* �纻���� �� */
	virtual ~CItem_Skill() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:	
	CTexture*				m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CCollider*				m_pColliderCom = { nullptr };
	CCalc_Timer*			m_pTimerCom = { nullptr };

	CTransform* m_pTargetTransform = { nullptr };
private:
	HRESULT Ready_Components();
	
private:

	
public:
	/* ������ü�� �����Ѵ�. */
	static CItem_Skill* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END