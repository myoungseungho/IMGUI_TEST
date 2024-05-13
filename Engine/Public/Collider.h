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

private:
	_float3 m_Center = {};  // �߽� ��ǥ
	_float m_Width = { 0.0f };  // ��
	_float m_Height = { 0.0f }; // ����
	_float m_Depth = { 0.0f };  // ����
	CGameObject* m_MineGameObject = { nullptr };

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END