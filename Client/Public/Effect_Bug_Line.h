#pragma once

#include "GameInstance.h"
#include "Effect_Monster.h"

BEGIN(Client)

class CEffect_Bug_Line :public CEffect_Monster
{
public:
	typedef struct :public EFFECT_MONSTER__DESC
	{
		CTransform* pPlayerTransform = { nullptr };

	}EFFECT_BUG_LINE_DESC;

private:
	CEffect_Bug_Line(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEffect_Bug_Line(const CEffect_Bug_Line& Prototype);
	virtual ~CEffect_Bug_Line() = default;



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
	CTransform* m_pPlayerTransform = { nullptr };

	_float m_fLength = { 5.f };
	_float3 m_vMoveDir = {};

public:
	static CEffect_Bug_Line* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
