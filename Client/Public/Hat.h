#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollider;
class CCalc_Timer;
class CAnimator;
END

BEGIN(Client)

class CHat final : public CGameObject
{	
public:
typedef struct
	{
		CTransform* pTargetTransform = { nullptr };
		_uint				pTargetDirection = { 0 };
	}HAT_DESC;

private:
	CHat(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CHat(const CHat& Prototype); /* �纻���� �� */
	virtual ~CHat() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

protected:
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

private:	
	CTexture*				m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };



private:
	HRESULT		Ready_Components();
	
private:
	CTransform* m_pTargetTransform = { nullptr };
	_uint				m_pTagetDirection = { 0 };

public:
	/* ������ü�� �����Ѵ�. */
	static CHat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END