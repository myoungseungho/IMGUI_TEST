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

_bool CLayer::Delete_GameObject()
{
	for (auto& iter = m_GameObjects.begin() ; iter != m_GameObjects.end() ; ++iter)
	{
		_bool isDeath  = (*iter)->Get_Death();

		return true;

	}

	return false;
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

	//¾èÀºº¹»ç
	*pList = m_GameObjects;
	
	for (auto& iter : *pList)
	{
		iter->AddRef();
	}

	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Priority_Update(fTimeDelta);
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject = m_GameObjects.begin() ; pGameObject!= m_GameObjects.end();  )
	{
		if ((*pGameObject) == nullptr)
			return;

		(*pGameObject)->Update(fTimeDelta);
		_bool isDeath = (*pGameObject)->Get_Death();

		if (isDeath)
		{
			Safe_Release(*pGameObject);
			pGameObject = m_GameObjects.erase(pGameObject);                                
		}
		else
			pGameObject++;
	}
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Late_Update(fTimeDelta);
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

