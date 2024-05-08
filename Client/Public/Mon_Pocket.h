#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)

class CMon_Pocket final : public CMonster
{	
public:
	typedef struct 
	{
		CTransform* pTargetTransform = { nullptr };
	}MONSTER_DESC;
private:
	CMon_Pocket(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CMon_Pocket(const CMon_Pocket& Prototype); /* �纻���� �� */
	virtual ~CMon_Pocket() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

private:
	CTransform* m_pTargetTransform = { nullptr };

public:
	/* ������ü�� �����Ѵ�. */
	static CMon_Pocket* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END