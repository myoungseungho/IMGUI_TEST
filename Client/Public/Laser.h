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
	CLaser(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CLaser(const CLaser& Prototype); /* 사본생성 시 */
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
	/* 원형객체를 생성한다. */
	static CLaser* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END