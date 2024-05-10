#include "Collider.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent{ pGraphic_Device }
{
}

CCollider::CCollider(const CCollider& Prototype)
	:CComponent{ Prototype }
{
}

HRESULT CCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	return S_OK;
}

void CCollider::Render()
{
}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider* pInstance = new CCollider(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype	()))
	{
		MSG_BOX(TEXT("Failed to Created : Collider "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Collider "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();
}
