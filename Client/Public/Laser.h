#pragma once

#include "Client_Defines.h"
#include "EnviormentObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollider;
END

BEGIN(Client)

class CLaser final : public CEnviormentObject
{	
public:
	typedef struct
	{
		CTransform* pTargetTransform = { nullptr };
	}LASER_DESC;
	

private:
	CLaser(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CLaser(const CLaser& Prototype); /* �纻���� �� */
	virtual ~CLaser() = default;

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
	CCollider* m_pColliderCom = { nullptr };

	CTransform* m_pTargetTransform = { nullptr };
private:
	HRESULT Ready_Components();

public:
	/* ������ü�� �����Ѵ�. */
	static CLaser* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END