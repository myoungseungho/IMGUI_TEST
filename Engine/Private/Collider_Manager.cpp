#include "..\Public\Collider_Manager.h"

#include "GameObject.h"
#include "Component.h"
CCollider_Manager::CCollider_Manager()
{
}

void CCollider_Manager::Update(_float fTimeDelta)
{
	// 먼저 모든 콜라이더를 업데이트
	for (size_t i = 0; i < CG_END; i++) {
		for (auto& collider : m_Colliders[i])
			collider->Update(fTimeDelta);
	}

	Check_Collison(fTimeDelta);
}

HRESULT CCollider_Manager::Render()
{
	for (size_t i = 0; i < CG_END; i++)
	{
		for (auto& collider : m_Colliders[i])
			collider->Render();
	}

	return S_OK;
}

HRESULT CCollider_Manager::Add_ColliderObject(COLLIDERGROUP eColliderGroup, CGameObject* pColliderObject)
{
	if (pColliderObject == nullptr || eColliderGroup >= CG_END)
		return E_FAIL;

	//사본 컴포넌트
	CComponent* com = pColliderObject->Get_Component(TEXT("Com_Collider"));

	if (com == nullptr)
		return E_FAIL;

	Safe_AddRef(com);

	CCollider* collider = static_cast<CCollider*>(com);

	m_Colliders[eColliderGroup].emplace_back(collider);

	return S_OK;
}

HRESULT CCollider_Manager::Check_Collison(_float fTimeDelta)
{
	//currentCollisions: 현재 프레임에서의 충돌 상태를 저장하는 맵입니다.
	// m_CollisionHistory와 같은 구조로 되어 있습니다.

	map<pair<CCollider*, CCollider*>, bool> currentCollisions;
	//최적화 개선 법
	//1. 공간 분할 (쿼드트리, 옥트리)
	//2. 이벤트 기반 충돌 검사 (객체 움직일때만 충돌 검사?)
	for (size_t i = 0; i < CG_END; i++)
	{
		for (size_t j = i + 1; j < CG_END; j++)
		{
			for (auto& colliderA : m_Colliders[i])
			{
				for (auto& colliderB : m_Colliders[j])
				{
					if (IsColliding(colliderA, colliderB)) {

						auto key = make_pair(colliderA, colliderB);
						currentCollisions[key] = true;

						//첫 번째 조건: m_CollisionHistory.find(key) == m_CollisionHistory.end()
						//이전 프레임에서 이 쌍이 존재하지 않았으면 OnCollisionEnter 이벤트를 호출합니다.
						
						//두 번째 조건 : !m_CollisionHistory[key]
						//이전 프레임에서 이 쌍이 존재했지만 충돌하지 않았으면 OnCollisionEnter 이벤트를 호출합니다.
						if (m_CollisionHistory.find(key) == m_CollisionHistory.end() || !m_CollisionHistory[key]) {
							colliderA->OnCollisionEnter(colliderB);
							colliderB->OnCollisionEnter(colliderA);
						}
						else {
							colliderA->OnCollisionStay(colliderB);
							colliderB->OnCollisionStay(colliderA);
						}
					}
				}
			}
		}
	}

	//이전 프레임에서는 충돌했지만
	// 현재 프레임에서는 충돌하지 않는 경우 OnCollisionExit 이벤트를 호출합니다.
	for (auto& pair : m_CollisionHistory) {
		if (currentCollisions.find(pair.first) == currentCollisions.end() || !currentCollisions[pair.first]) {
			pair.first.first->OnCollisionExit(pair.first.second);
			pair.first.second->OnCollisionExit(pair.first.first);
		}
	}

	// Update the collision history for the next frame
	m_CollisionHistory = currentCollisions;
	return S_OK;
}

bool CCollider_Manager::IsColliding(const CCollider* a, const CCollider* b)
{
	// AABB 충돌 검사
	return (a->m_Center.x - a->m_Width / 2 < b->m_Center.x + b->m_Width / 2 &&
		a->m_Center.x + a->m_Width / 2 > b->m_Center.x - b->m_Width / 2 &&
		a->m_Center.y - a->m_Height / 2 < b->m_Center.y + b->m_Height / 2 &&
		a->m_Center.y + a->m_Height / 2 > b->m_Center.y - b->m_Height / 2 &&
		a->m_Center.z - a->m_Depth / 2 < b->m_Center.z + b->m_Depth / 2 &&
		a->m_Center.z + a->m_Depth / 2 > b->m_Center.z - b->m_Depth / 2);
}


CCollider_Manager* CCollider_Manager::Create()
{
	return new CCollider_Manager();
}

void CCollider_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < CG_END; i++)
	{
		for (auto& pCollider : m_Colliders[i])
			Safe_Release(pCollider);
		m_Colliders[i].clear();
	}
}
