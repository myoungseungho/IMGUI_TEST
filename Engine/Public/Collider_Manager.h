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
	HRESULT Check_Collison(_float);
	bool IsColliding(const CCollider* a, const CCollider* b);

private:
	list<class CCollider*>			m_Colliders[CG_END];
	map<std::pair<CCollider*, CCollider*>, bool> m_CollisionHistory;
public:
	static CCollider_Manager* Create();
	virtual void Free() override;
};

END