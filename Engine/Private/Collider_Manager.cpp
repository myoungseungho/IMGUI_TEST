#include "..\Public\Collider_Manager.h"

#include "GameObject.h"
#include "Component.h"
CCollider_Manager::CCollider_Manager()
{
}

void CCollider_Manager::Update(_float fTimeDelta)
{
	Check_Collison(fTimeDelta);
}

HRESULT CCollider_Manager::Add_ColliderObject(COLLIDERGROUP eColliderGroup, CGameObject* pColliderObject)
{
	if (pColliderObject == nullptr || eColliderGroup >= CG_END)
		return E_FAIL;

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

				}
			}
		}
	}
	return S_OK;
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
