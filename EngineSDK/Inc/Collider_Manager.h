#pragma once

#include "Base.h"
#include "Collider.h"

/* 1. 화면에 그려야할 객체들을 그리는 순서대로 보관한다. */
/* 2. 보관하고 있는 객체들의 렌더함수를 호출한다.(렌더콜) */

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
	//m_CollisionHistory: 이전 프레임에서의 충돌 상태를 저장하는 맵입니다.
	// 키는 두 콜라이더의 포인터를 담은 쌍(pair)이고, 값은 이 쌍이 충돌 중인지 여부를 나타내는 부울 값입니다.
	map<pair<CCollider*, CCollider*>, bool> m_CollisionHistory;

	bool m_IsOn = { false };
	_float m_CollisionCheckTimer = 0.0f;
	const _float m_CollisionCheckInterval = 1.f; // 0.5초마다 검사

public:
	static CCollider_Manager* Create();
	virtual void Free() override;
};

END