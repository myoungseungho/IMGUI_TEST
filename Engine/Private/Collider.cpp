#include "..\Public\Collider.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CCollider::CCollider(const CCollider& Prototype)
	: CComponent{ Prototype }
{
}

HRESULT CCollider::Initialize_Prototype()
{
	return S_OK;
}



HRESULT CCollider::Initialize(void* pArg)
{
	COLLIDER_DESC* pDesc = static_cast<COLLIDER_DESC*>(pArg);

	m_Center = pDesc->center;
	m_Width = pDesc->width;
	m_Height = pDesc->height;
	m_Depth = pDesc->depth;

	if (pDesc->MineGameObject != nullptr)
	{
		m_MineGameObject = pDesc->MineGameObject;
		Safe_AddRef(m_MineGameObject);
	}

	return S_OK;
}

void CCollider::OnCollisionEnter(CCollider* other)
{

}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider* pInstance = new CCollider(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Release(m_MineGameObject);
}
