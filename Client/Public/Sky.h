#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CSky final : public CGameObject
{	

private:
	CSky(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CSky(const CSky& Prototype); /* 사본생성 시 */
	virtual ~CSky() = default;

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
	CVIBuffer_Cube*		m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	/* 원형객체를 생성한다. */
	static CSky* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END