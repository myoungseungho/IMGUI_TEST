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

class CRotation_Orb final : public CEnviormentObject
{	
public:
	typedef struct
	{
		CTransform* pTargetTransform = { nullptr };
	}ROTATION_ORB_DESC;

private:
	CRotation_Orb(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CRotation_Orb(const CRotation_Orb& Prototype); /* �纻���� �� */
	virtual ~CRotation_Orb() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

public:


private:	
	CTexture*				m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CCalc_Timer*			m_pTimerCom = { nullptr };
	CCollider*				m_pColliderCom = { nullptr };

	CTransform* m_pTargetTransform = { nullptr };
private:
	HRESULT Ready_Components();


private:

public:
	/* ������ü�� �����Ѵ�. */
	static CRotation_Orb* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END