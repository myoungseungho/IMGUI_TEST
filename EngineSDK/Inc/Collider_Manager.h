#pragma once

#include "Base.h"

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
	HRESULT Add_ColliderObject(COLLIDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Check_Collison();

private:
	list<class CGameObject*>			m_ColliderObjects[CG_END];

public:
	static CCollider_Manager* Create();
	virtual void Free() override;
};

END