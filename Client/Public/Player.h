#pragma once

#include "Client_Defines.h"
#include "LandObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CComponent;
class CVIBuffer_Rect;
class CCollider;
END

BEGIN(Client)

class CPlayer final : public CLandObject
{	
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CPlayer(const CPlayer& Prototype); /* �纻���� �� */
	virtual ~CPlayer() = default;

//public:
//	virtual ObjectType GetType() const override {
//		return ObjectType::Player;
//	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other);
	virtual void OnCollisionExit(class CCollider* other);
private:	
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };


private:
	HRESULT Ready_Components();

public:
	/* ������ü�� �����Ѵ�. */
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END