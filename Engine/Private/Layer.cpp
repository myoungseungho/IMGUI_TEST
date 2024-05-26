#include "..\Public\Layer.h"

#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent* CLayer::Get_Component(const _wstring& strComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	if (iter == m_GameObjects.end())
		return nullptr;

	return (*iter)->Get_Component(strComponentTag);
}

CGameObject* CLayer::Get_GameObject(_uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	if (iter == m_GameObjects.end())
		return nullptr;

	return (*iter);
}

CGameObject* CLayer::Get_GameObjectByIndex(_uint iIndex)
{
	// 리스트를 순회하며 m_index가 iIndex와 같은 객체를 찾음
	for (auto& pGameObject : m_GameObjects)
	{
		if (pGameObject->m_iIndex == iIndex)
			return pGameObject; // 일치하는 객체를 찾으면 반환
	}

	return nullptr; // 일치하는 객체가 없으면 nullptr 반환
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.emplace_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Add_List(list<CGameObject*>* pList)
{
	if (pList == nullptr)
		return E_FAIL;

	//얕은복사
	*pList = m_GameObjects;

	for (auto& iter : *pList)
	{
		Safe_AddRef(iter);
	}

	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); )
	{
		if ((*it)->m_Died)
		{
			it = m_GameObjects.erase(it); // 현재 요소를 제거하고 반복기 업데이트
			continue; // 다음 반복으로 이동
		}
		else
		{
			(*it)->Priority_Update(fTimeDelta);
			++it; // 다음 요소로 이동
		}
	}
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); )
	{
		if ((*it)->m_Died)
		{
			it = m_GameObjects.erase(it); // 현재 요소를 제거하고 반복기 업데이트
			continue; // 다음 반복으로 이동
		}
		else
		{
			(*it)->Update(fTimeDelta);
			++it; // 다음 요소로 이동
		}
	}
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); )
	{
		if ((*it)->m_Died)
		{
			it = m_GameObjects.erase(it); // 현재 요소를 제거하고 반복기 업데이트
			continue; // 다음 반복으로 이동
		}
		else
		{
			(*it)->Late_Update(fTimeDelta);
			++it; // 다음 요소로 이동
		}
	}
}

CLayer* CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();

}

