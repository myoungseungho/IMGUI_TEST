#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)

class CBoss_Koofu final :public CMonster
{
public:
	typedef struct :public CMonster::MONSTER_DESC
	{
		
	}BOSS_KOOFU_DESC;

private:
	CBoss_Koofu(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBoss_Koofu(const CBoss_Koofu& Prototype);
	virtual ~CBoss_Koofu() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void MonState(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components();
	HRESULT  KeyInput(_float fTimeDelta);
	void ScaleUp(_float fTimeDelta);
	
private:
	bool	m_isScale = { false };
	_float fScaleTimer = { 0.f };

public:
	static CBoss_Koofu* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

