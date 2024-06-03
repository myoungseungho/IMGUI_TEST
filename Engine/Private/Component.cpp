#include "..\Public\Component.h"
#include "GameInstance.h"

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device{ pGraphic_Device }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
	, m_isCloned{ false }
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameInstance);
}

CComponent::CComponent(const CComponent& Prototype)
	: m_pGraphic_Device{ Prototype.m_pGraphic_Device }
	, m_pGameInstance{ Prototype.m_pGameInstance }
	, m_isCloned{ true }
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameInstance);
}
HRESULT CComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CComponent::Initialize(void * pArg)
{
	return S_OK;
}


void CComponent::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);

	__super::Free();
}
