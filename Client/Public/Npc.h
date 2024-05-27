#pragma once

#include "Client_Defines.h"
#include "LandObject.h"



BEGIN(Engine)
class CTransform;
class CComponent;
class CVIBuffer_Rect;
class CCollider;
class CAnimator;
END

BEGIN(Client)

class CNpc final : public CGameObject
{
private:
	enum ANIMATION_STATE {
		ANIM_IDLE,
		ANIM_REACTION
	};
private:
	CNpc(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CNpc(const CNpc& Prototype); /* �纻���� �� */
	virtual ~CNpc() = default;

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


private:
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CAnimator* m_pAnimCom = { nullptr };

private:
	ANIMATION_STATE m_eAnimState = ANIM_IDLE;
private:
	HRESULT Ready_Components();
	HRESULT Ready_Animation();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

private:
	void				 AnimState(_float _fTimeDelta);

public:

	/* ������ü�� �����Ѵ�. */
	static CNpc* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END