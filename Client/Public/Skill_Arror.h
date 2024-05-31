#pragma once

#include "Skill_Monster.h"	

BEGIN(Engine)

class CSkill_Arror final:public CSkill_Monster
{
private:
	CSkill_Arror(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkill_Arror(const CSkill_Arror& Prototype);
	virtual  ~CSkill_Arror() = default;

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
public:
	virtual void OnCollisionEnter(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

private:
	void Destroy(_float fTimeDelta);

	_uint m_iDirTexture = { 0 };

public:
	static CSkill_Arror* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

