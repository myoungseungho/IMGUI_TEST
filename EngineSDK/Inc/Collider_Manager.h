#pragma once

#include "Base.h"
#include "Collider.h"

/* 1. ȭ�鿡 �׷����� ��ü���� �׸��� ������� �����Ѵ�. */
/* 2. �����ϰ� �ִ� ��ü���� �����Լ��� ȣ���Ѵ�.(������) */

BEGIN(Engine)

class CCollider_Manager final : public CBase
{
public:
	enum COLLIDERGROUP { CG_PLAYER, CG_MONSTER, CG_STATIC, CG_END };
private:
	CCollider_Manager();
	virtual ~CCollider_Manager() = default;

public:
	void Update(_float fTimeDelta);
	HRESULT Render();

public:
	HRESULT Add_ColliderObject(COLLIDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Check_Collision(_float);
	bool IsColliding(const CCollider* a, const CCollider* b);
	HRESULT Show_Collider(bool _isOn)
	{
		m_IsOn = _isOn;
		return S_OK;
	};

private:
	list<class CCollider*>			m_Colliders[CG_END];
	//m_CollisionHistory: ���� �����ӿ����� �浹 ���¸� �����ϴ� ���Դϴ�.
	// Ű�� �� �ݶ��̴��� �����͸� ���� ��(pair)�̰�, ���� �� ���� �浹 ������ ���θ� ��Ÿ���� �ο� ���Դϴ�.
	map<pair<CCollider*, CCollider*>, bool> m_CollisionHistory;

	bool m_IsOn = { false };
	_float m_CollisionCheckTimer = 0.0f;
	const _float m_CollisionCheckInterval = 1.f; // 0.5�ʸ��� �˻�

public:
	static CCollider_Manager* Create();
	virtual void Free() override;
};

END