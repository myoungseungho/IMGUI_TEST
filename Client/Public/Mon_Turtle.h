#pragma once

#include "Monster.h"

BEGIN(Client)

class ENGINE_DLL CMon_Turtle  final:public CMonster
{
private:
	CMon_Turtle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMon_Turtle(const CMon_Turtle& Prototype);
	virtual ~CMon_Turtle() = default;

	virtual HRESULT Initialize_Prototype()					override;
	virtual HRESULT Initialize(void* pArg)					override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components();

public:
	/* ������ü�� �����Ѵ�. */
	static CMon_Turtle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

