#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CMon_Pocket final : public CGameObject
{	
public:
	typedef struct
	{
		CTransform* pTargetTransform = { nullptr };
	}MONSTER_DESC;
private:
	CMon_Pocket(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CMon_Pocket(const CMon_Pocket& Prototype); /* 사본생성 시 */
	virtual ~CMon_Pocket() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:	
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };

private:
	CTransform*			m_pTargetTransform = { nullptr };


private:
	HRESULT Ready_Components();



public:
	/* 원형객체를 생성한다. */
	static CMon_Pocket* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END