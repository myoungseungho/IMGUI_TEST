#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)

class CBoss_Bug final:public CMonster
{
private:
	CBoss_Bug(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBoss_Bug(const CBoss_Bug& Prototype);
	virtual ~CBoss_Bug() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components();

public:
	static CBoss_Bug* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

