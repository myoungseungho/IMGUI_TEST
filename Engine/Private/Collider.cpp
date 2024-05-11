#include "Collider.h"
#include "VIBuffer_Cube.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer{ pGraphic_Device }
{
}

CCollider::CCollider(const CCollider& Prototype)
	:CVIBuffer{ Prototype }
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

HRESULT CCollider::Render(CVIBuffer_Cube** ppCube)
{
	Begin_Render();


	if (FAILED((*ppCube)->Render()))
		return E_FAIL;
	  
		
	End_Render();

	return S_OK;
}

void CCollider::Begin_Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CCollider::End_Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
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
