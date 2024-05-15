#include "Animator.h"

CAnimator::CAnimator(LPDIRECT3DDEVICE9 pGrpahic_Device)
	:CComponent { pGrpahic_Device }
{
}

CAnimator::CAnimator(const CAnimator& pPrototype)
	:CComponent{ pPrototype }
{
}

HRESULT CAnimator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimator::Initialize(void* pArg)
{
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
