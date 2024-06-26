#include "..\Public\GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device{ pGraphic_Device }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject& Prototype)
	: m_pGraphic_Device{ Prototype.m_pGraphic_Device }
	, m_pGameInstance{ Prototype.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return S_OK;

	FILEDATA* fileData = static_cast<FILEDATA*>(pArg);
	m_bIsPasingObject = fileData->isParsing;

	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{
}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render(_float DeltaTime)
{
	return S_OK;
}

void CGameObject::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
}

void CGameObject::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CGameObject::OnCollisionExit(CCollider* other)
{
}

void CGameObject::Delete_Object()
{
	CGameObject* pThis = this;

	Safe_Release(pThis);
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	/* 이미 strComponentTag키를 가진 컴포넌트가 있었다. */
	if (nullptr != Get_Component(strComponentTag))
		return E_FAIL;

	/* strComponentTag키를 가진 컴포넌트가 없었다.*/
	CComponent* pComponent = m_pGameInstance->Clone_Component(iPrototypeLevelIndex, strPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);

	__super::Free();
}
