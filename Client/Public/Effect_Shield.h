#pragma once

#include "GameInstance.h"
#include "Effect_Monster.h"

BEGIN(Client)

class CEffect_Shield :public CEffect_Monster
{
private:
	CEffect_Shield(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEffect_Shield(const CEffect_Shield& Prototype);
	virtual ~CEffect_Shield() = default;


public:
	virtual HRESULT Initialize_Prototype()					override;
	virtual HRESULT Initialize(void* pArg)					override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;


private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_Animation();

	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

private:
	void Destroy(_float fTimeDelta);


public:
	static CEffect_Shield* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

