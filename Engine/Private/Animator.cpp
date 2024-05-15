#include "Animator.h"

#include"Texture.h"
#include"GameInstance.h"

CAnimator::CAnimator(LPDIRECT3DDEVICE9 pGrpahic_Device)
	:CComponent { pGrpahic_Device }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

CAnimator::CAnimator(const CAnimator& pPrototype)
	:CComponent{ pPrototype }
	, m_pGameInstance{ pPrototype.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CAnimator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimator::Initialize(void* pArg)
{
	return S_OK;
}



HRESULT CAnimator::Add_Animator(_uint iLevel, const wstring& strLayerTag, const wstring& strComponentTag, const wstring& strAnimTag)
{ 
	CTexture* pTexture = dynamic_cast<CTexture*>(m_pGameInstance->Get_Component(iLevel, strLayerTag , strComponentTag));
	
	if (pTexture == nullptr)
		return E_FAIL;

	m_pTexture.emplace(strAnimTag, pTexture);

	return S_OK;
}

CAnimator* CAnimator::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAnimator* pInstance = new CAnimator(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Animator"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CAnimator::Clone(void* pArg)
{
	CAnimator* pInstance = new CAnimator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Animator"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimator::Free()
{
	__super::Free();
}
