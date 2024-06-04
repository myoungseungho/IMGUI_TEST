#pragma once

#include "Client_Defines.h"
#include "BlendObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Client)

class CUIObject abstract : public CBlendObject
{
protected:
	CUIObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUIObject(const CUIObject& rhs);
	virtual ~CUIObject() = default;

protected:
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

protected:
	void BillBoarding();
	void Set_OrthoMatrix(); // ���� ���� ���� �Լ� �߰�
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

protected:
	_float4x4 m_ViewMatrix = {};
	_float4x4 m_ProjMatrix = {}; // Projection ��� ��� ���� �߰�
public:
	_float m_fSizeX = 0.f;
	_float m_fSizeY = 0.f;
protected:
	_float offsetX = { 0.f };
	_float offsetY = { 0.f };
	_float offsetZ = { 0.f };
	_float offsetXScale = { 0.f };
	_float offsetYScale = { 0.f };

public:
	_float m_fAlpha = { 190.f };               // Current alpha value
	_float m_fX = 0.f;
	_float m_fY = 0.f;

public:
	_bool m_bIsOn = { false };
	_uint m_iItemIndex = { 0 };
};

END