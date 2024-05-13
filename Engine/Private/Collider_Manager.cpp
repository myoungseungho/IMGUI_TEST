#include "..\Public\Collider_Manager.h"

#include "GameObject.h"
#include "Component.h"
#include "Collider.h"
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

	m_ColliderObjects[eColliderGroup].emplace_back(pColliderObject);

	Safe_AddRef(pColliderObject);

	return S_OK;
}

HRESULT CCollider_Manager::Check_Collison(_float fTimeDelta)
{
	//����ȭ ���� ��
	//1. ���� ���� (����Ʈ��, ��Ʈ��)
	//2. �̺�Ʈ ��� �浹 �˻� (��ü �����϶��� �浹 �˻�?)
	for (size_t i = 0; i < CG_END; i++)
	{
		for (size_t j = i + 1; j < CG_END; j++)
		{
			for (auto& objA : m_ColliderObjects[i])
			{
				CComponent* objA_Com = objA->Get_Component(TEXT("Com_Collider"));
				if (objA_Com == nullptr)
					continue;

				for (auto& objB : m_ColliderObjects[j])
				{
					CComponent* objB_Com = objB->Get_Component(TEXT("Com_Collider"));
					if (objB_Com == nullptr)
						continue;

					CCollider* objA_Collider_Com = static_cast<CCollider*>(objA_Collider_Com);
					CCollider* objB_Collider_Com = static_cast<CCollider*>(objA_Collider_Com);
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
		for (auto& pRenderObject : m_ColliderObjects[i])
			Safe_Release(pRenderObject);
		m_ColliderObjects[i].clear();
	}
}
