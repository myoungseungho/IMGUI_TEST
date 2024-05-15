#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CAnimator final:public CComponent
{
private:
	CAnimator(LPDIRECT3DDEVICE9 pGrpahic_Device);
	CAnimator(const CAnimator& pPrototype);
	virtual ~CAnimator() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

	virtual HRESULT Add_Animator(_uint iLevel,const wstring& strLayerTag, const wstring& strComponentTag);

private:
	map<const _wstring, class CTexture*> m_pTexture;

	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CAnimator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END