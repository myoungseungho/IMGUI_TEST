#include "..\Public\Collider_Manager.h"

#include "GameObject.h"

CCollider_Manager::CCollider_Manager()
{
}

HRESULT CCollider_Manager::Add_ColliderObject(COLLIDERGROUP eColliderGroup, CGameObject* pColliderObject)
{
	if (pColliderObject == nullptr || eColliderGroup >= CG_END)
		return E_FAIL;

	m_ColliderObjects[eColliderGroup].emplace_back(pColliderObject);

	Safe_AddRef(pColliderObject);

	return S_OK;
}

HRESULT CCollider_Manager::Check_Collison()
{
	
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
		for (auto& pRenderObject : m_ColliderObjects[i])
			Safe_Release(pRenderObject);
		m_ColliderObjects[i].clear();
	}
}
