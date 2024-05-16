#pragma once

#include "Component.h"
#include "GameObject.h"
BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	typedef struct
	{
		_float3 center;  // �߽� ��ǥ
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
	void OnCollisionEnter(CCollider*);
	void OnCollisionStay(CCollider*);
	void OnCollisionExit(CCollider*);

public:
	CGameObject* m_MineGameObject = { nullptr };
	_float3 m_Center = {};  // �߽� ��ǥ
	_float m_Width = { 0.0f };  // ��
	_float m_Height = { 0.0f }; // ����
	_float m_Depth = { 0.0f };  // ����
	_float4x4				m_WorldMatrix;
private:
	bool m_IsCollied = { false };
public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END