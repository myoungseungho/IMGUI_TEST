#include "../Public/Collision.h"

CCollision::CCollision(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent { pGraphic_Device }
{
}

CCollision::CCollision(const CCollision& Prototype)
	: CComponent{Prototype}
{
}

HRESULT CCollision::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollision::Initialize(void* pArg)
{
	return S_OK;
}


CCollision* CCollision::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollision* pInstance = new CCollision(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Collision_Component"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollision::Clone(void* pArg)
{
	CCollision* pInstance = new CCollision(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCollision_Component "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollision::Free()
{
	__super::Free();
}
