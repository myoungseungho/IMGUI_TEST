#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	struct Cube
	{
		_float3 center = {};  // ¡ﬂΩ… ¡¬«•
		_float width = 0.0f;  // ∆¯
		_float height = 0.0f; // ≥Ù¿Ã
		_float depth = 0.0f;  // ±Ì¿Ã
	};

private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

private:
	Cube cube;

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END