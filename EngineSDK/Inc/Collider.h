#pragma once

#include "Component.h"
#include "GameObject.h"
BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	typedef struct
	{
		_float3 center;  // Áß½É ÁÂÇ¥
		_float width, height, depth;
		CGameObject* MineGameObject;
	}COLLIDER_DESC;

private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
public:
	void Update(_float fTimeDelta);
	void Render();

public:
	void OnCollisionEnter(CCollider*, _float fTimeDelta);
	void OnCollisionStay(CCollider*, _float fTimeDelta);
	void OnCollisionExit(CCollider*);

public:
	CGameObject* m_MineGameObject = { nullptr };
	_float3 m_Center = {};  // Áß½É ÁÂÇ¥
	_float m_Width = { 0.0f };  // Æø
	_float m_Height = { 0.0f }; // ³ôÀÌ
	_float m_Depth = { 0.0f };  // ±íÀÌ
	_float4x4				m_WorldMatrix;
private:
	_bool m_bIsCollied = { false };
	_bool m_bEnabled = { false };
public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END