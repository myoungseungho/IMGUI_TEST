#pragma once

#include "GameInstance.h"
#include "Effect_Monster.h"

BEGIN(Client)

class CEffect_Koofu_Smoke :public CEffect_Monster
{
public:
	typedef struct
	{
		CTransform* pTargetTransform = { nullptr };
		_uint		iSmokeNum = { 0 };
	}EFFECT_SMOKE_DESC;

private:
	CEffect_Koofu_Smoke(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEffect_Koofu_Smoke(const CEffect_Koofu_Smoke& Prototype);
	virtual ~CEffect_Koofu_Smoke() = default;


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

private:
	_uint m_iSmokeNum = { 0 };
	D3DMATERIAL9		MaterialDesc{};
	D3DLIGHT9			LightDesc{};

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0), 2.0f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(D3DCOLOR_XRGB(255, 0, 0), D3DCOLOR_XRGB(255, 0, 0), D3DCOLOR_XRGB(255, 0, 0), D3DCOLOR_XRGB(0, 0, 0), 2.0f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(D3DCOLOR_XRGB(0, 255, 0), D3DCOLOR_XRGB(0, 255, 0), D3DCOLOR_XRGB(0, 255, 0), D3DCOLOR_XRGB(0, 0, 0), 2.0f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(D3DCOLOR_XRGB(0, 0, 255), D3DCOLOR_XRGB(0, 0, 255), D3DCOLOR_XRGB(0, 0, 255), D3DCOLOR_XRGB(0, 0, 0), 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(D3DCOLOR_XRGB(255, 255, 0), D3DCOLOR_XRGB(255, 255, 0), D3DCOLOR_XRGB(255, 255, 0), D3DCOLOR_XRGB(0, 0, 0), 2.0f);

public:
	static CEffect_Koofu_Smoke* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

