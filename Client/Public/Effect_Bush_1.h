#pragma once

#include "Client_Defines.h"
#include "Effect.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollider;
class CCalc_Timer;
class CAnimator;
END

BEGIN(Client)

class CEffect_Bush_1 final : public CEffect
{	
public:
typedef struct
	{
		CTransform* pTargetTransform = { nullptr };
	}EFFECT_BUSH_DESC;

private:
	CEffect_Bush_1(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CEffect_Bush_1(const CEffect_Bush_1& Prototype); /* �纻���� �� */
	virtual ~CEffect_Bush_1() = default;

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
	CAnimator*				 m_pAnimCom = { nullptr };


private:
	HRESULT		Ready_Components();

private:

	
private:
	CTransform* m_pTargetTransform = { nullptr };

public:
	/* ������ü�� �����Ѵ�. */
	static CEffect_Bush_1* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END