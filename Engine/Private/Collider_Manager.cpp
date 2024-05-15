#include "..\Public\Collider_Manager.h"

#include "GameObject.h"
#include "Component.h"
CCollider_Manager::CCollider_Manager()
{
}

void CCollider_Manager::Update(_float fTimeDelta)
{
	// ���� ��� �ݶ��̴��� ������Ʈ
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

	//�纻 ������Ʈ
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
	//currentCollisions: ���� �����ӿ����� �浹 ���¸� �����ϴ� ���Դϴ�.
	// m_CollisionHistory�� ���� ������ �Ǿ� �ֽ��ϴ�.

	map<pair<CCollider*, CCollider*>, bool> currentCollisions;
	//����ȭ ���� ��
	//1. ���� ���� (����Ʈ��, ��Ʈ��)
	//2. �̺�Ʈ ��� �浹 �˻� (��ü �����϶��� �浹 �˻�?)
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

						//ù ��° ����: m_CollisionHistory.find(key) == m_CollisionHistory.end()
						//���� �����ӿ��� �� ���� �������� �ʾ����� OnCollisionEnter �̺�Ʈ�� ȣ���մϴ�.
						
						//�� ��° ���� : !m_CollisionHistory[key]
						//���� �����ӿ��� �� ���� ���������� �浹���� �ʾ����� OnCollisionEnter �̺�Ʈ�� ȣ���մϴ�.
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

	//���� �����ӿ����� �浹������
	// ���� �����ӿ����� �浹���� �ʴ� ��� OnCollisionExit �̺�Ʈ�� ȣ���մϴ�.
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
	// AABB �浹 �˻�
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
