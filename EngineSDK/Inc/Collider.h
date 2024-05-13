#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	typedef struct
	{
		_float3 center;  // Áß½É ÁÂÇ¥
		_float width, height, depth;
	}CUBEDESC;

private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

private:
	_float3 center = {};  // Áß½É ÁÂÇ¥
	_float width = 0.0f;  // Æø
	_float height = 0.0f; // ³ôÀÌ
	_float depth = 0.0f;  // ±íÀÌ

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END