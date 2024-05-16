#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CAnimator final :public CComponent
{
private:
	CAnimator(LPDIRECT3DDEVICE9 pGrpahic_Device);
	CAnimator(const CAnimator& pPrototype);
	virtual ~CAnimator() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Add_Animator(_uint iLevel, const wstring& strComponentTag, const wstring& strTextureTag);
	HRESULT Play_Animator(const wstring& strTextureTag, _float fFrame, _float fTimeDelta , _bool isLoop);
	void Move_Frame(_float fFrame, _uint iNumTextures, _float fTimeDelta, _bool isLoop);

private:
	class CTexture* Find_Texture(const wstring& strTextureTag);

private:
	map<const _wstring, class CTexture*> m_pTexture;
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	_uint m_iIndex = { 0 };
	_float m_fTimeAcc = { 0.f };
	
public:
	static CAnimator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END