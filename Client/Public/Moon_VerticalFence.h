#pragma once

#include "Client_Defines.h"
#include "EnviormentObject.h"


BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
class CAnimator;
class CCollider;
END

BEGIN(Client)

class CMoon_VerticalFence final : public CEnviormentObject
{
private:
	CMoon_VerticalFence(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CMoon_VerticalFence(const CMoon_VerticalFence& Prototype); /* �纻���� �� */
	virtual ~CMoon_VerticalFence() = default;

private:
	enum ANIMATION_STATE {
		ANIM_IDLE,
		ANIM_DIE,
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
	CCollider* m_pColliderCom = { nullptr };
	CAnimator* m_pAnimCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Ready_Animation();
	void AnimState(_float _fTimeDelta);

private:
	ANIMATION_STATE m_eAnimState = ANIM_IDLE;

public:
	/* ������ü�� �����Ѵ�. */
	static CMoon_VerticalFence* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END