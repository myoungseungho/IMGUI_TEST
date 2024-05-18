#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)

class CMon_Turtle  final:public CMonster
{
public:
	typedef struct :public CMonster::MONSTER_DESC
	{
		wstring ColorTexTag = {};
		CTransform* pTargetTransform = {};
		
	}MON_TURTLE_DESC;

private:
	CMon_Turtle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMon_Turtle(const CMon_Turtle& Prototype);
	virtual ~CMon_Turtle() = default;

	virtual HRESULT Initialize_Prototype()					override;
	virtual HRESULT Initialize(void* pArg)					override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;


private:
	void Mon_State(_float fTimeDelta);
	void Idle_Update(_float fTimeDelta);
	void Move_Update(_float fTimeDelta);

private:
	void Distory(_float fTimeDelta);

private:
	wstring m_ColorTexTag = {};
	CTransform* m_pTargetTransform = { nullptr };

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Animation();

	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static CMon_Turtle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

