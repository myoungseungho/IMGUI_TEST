#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
private:
	CPicking(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd);
	void Update();
	void Transform_ToLocalSpace(const _float4x4* pWorldMatrix);
	_bool Picked_InLocalSpace(const _float3 * pPointA, const _float3 * pPointB, const _float3 * pPointC, _float3* pPickPos);
	_bool Picked_InWorldSpace(const _float3 * pPointA, const _float3 * pPointB, const _float3 * pPointC, _float3* pPickPos);

private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	HWND						m_hWnd = {};
	_float3						m_vRayDir = {};
	_float3						m_vRayPos = {};

	_float3						m_vRayDir_Local = {};
	_float3						m_vRayPos_Local = {};

public:
	static CPicking* Create(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd);
	virtual void Free() override;
};

END