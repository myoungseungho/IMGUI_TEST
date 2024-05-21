#include "..\Public\Collider.h"
#include "Transform.h"
CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CCollider::CCollider(const CCollider& Prototype)
	: CComponent{ Prototype }
	, m_WorldMatrix{ Prototype.m_WorldMatrix }

{
}

HRESULT CCollider::Initialize_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

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
	}

	CComponent* componet = m_MineGameObject->Get_Component(TEXT("Com_Transform"));
	CTransform* transform = static_cast<CTransform*>(componet);

	m_WorldMatrix = transform->Get_WorldMatrix();
	m_Width /= transform->Get_Scaled().x;
	m_Height /= transform->Get_Scaled().y;
	m_Depth /= transform->Get_Scaled().z;

	return S_OK;
}


void CCollider::Render()
{
	if (!m_pGraphic_Device) return;

	// 현재 FILL 모드 상태 저장
	DWORD originalFillMode;
	m_pGraphic_Device->GetRenderState(D3DRS_FILLMODE, &originalFillMode);

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);

	// 로컬 스페이스에서 콜라이더의 8개 정점을 계산
	_float3 localVertices[8];
	float halfWidth = (m_Width / 2.0f);
	float halfHeight = m_Height / 2.0f;
	float halfDepth = m_Depth / 2.0f;

	localVertices[0] = _float3(-halfWidth, -halfHeight, -halfDepth);
	localVertices[1] = _float3(halfWidth, -halfHeight, -halfDepth);
	localVertices[2] = _float3(halfWidth, halfHeight, -halfDepth);
	localVertices[3] = _float3(-halfWidth, halfHeight, -halfDepth);
	localVertices[4] = _float3(-halfWidth, -halfHeight, halfDepth);
	localVertices[5] = _float3(halfWidth, -halfHeight, halfDepth);
	localVertices[6] = _float3(halfWidth, halfHeight, halfDepth);
	localVertices[7] = _float3(-halfWidth, halfHeight, halfDepth);

	// 인덱스 배열
	short indices[] = {
		0, 1, 1, 2, 2, 3, 3, 0, // 아래 면
		4, 5, 5, 6, 6, 7, 7, 4, // 위 면
		0, 4, 1, 5, 2, 6, 3, 7  // 수직 선
	};

	// 정점 버퍼 설정
	m_pGraphic_Device->SetFVF(D3DFVF_XYZ);

	// 와이어프레임 모드로 설정
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// 정점과 인덱스를 사용하여 육면체 그리기
	m_pGraphic_Device->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 8, 12, indices, D3DFMT_INDEX16, localVertices, sizeof(_float3));

	// 렌더링 상태를 원래대로 복원
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, originalFillMode);
}



void CCollider::Update(_float fTimeDelta)
{
	CComponent* componet = m_MineGameObject->Get_Component(TEXT("Com_Transform"));
	CTransform* transform = static_cast<CTransform*>(componet);

	m_Center.x = transform->Get_State(CTransform::STATE_POSITION).x;
	m_Center.y = transform->Get_State(CTransform::STATE_POSITION).y;
	m_Center.z = transform->Get_State(CTransform::STATE_POSITION).z;

	m_WorldMatrix = transform->Get_WorldMatrix();

	// 플레이어의 월드 행렬을 설정
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
}

void CCollider::OnCollisionEnter(CCollider* other)
{
	if (!m_bIsCollied)
		m_bIsCollied != m_bIsCollied;

	m_MineGameObject->OnCollisionEnter(other);
}


void CCollider::OnCollisionStay(CCollider* other)
{
	m_MineGameObject->OnCollisionStay(other);
}

void CCollider::OnCollisionExit(CCollider* other)
{
	if (!m_bIsCollied)
		m_bIsCollied != m_bIsCollied;

	if (other->m_Died || m_Died)
		return;

	m_MineGameObject->OnCollisionExit(other);
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
}
