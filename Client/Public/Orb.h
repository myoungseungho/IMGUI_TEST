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

class COrb : public CEnviormentObject
{	
private:
	enum ORB_EFFECT
	{
		EFFECT_ON,
		EFFECT_OFF,
		EFFECT_END
	};

protected:
	COrb(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	COrb(const COrb& Prototype); /* 사본생성 시 */
	virtual ~COrb() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:


private:	
	CTexture*				m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CCollider*				m_pColliderCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END