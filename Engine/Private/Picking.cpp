#include "..\Public\Picking.h"

CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CPicking::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	return S_OK;
}

void CPicking::Update()
{
	D3DVIEWPORT9		ViewportDesc{};
	m_pGraphic_Device->GetViewport(&ViewportDesc);	

	POINT		ptMouse{};

	/* Viewport���� ���콺 ��ǥ�� ���Ѵ�. */
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	_float3		vMousePos;
	/* ���������̽� ���� ���콺 ��ǥ�� ������. */
	/* x : 0 -> -1, winsizex -> 1 */
	vMousePos.x = ptMouse.x / (ViewportDesc.Width * 0.5f) - 1.f;

	/* y : 0 -> 1, winsizey -> -1 */
	vMousePos.y = ptMouse.y / -(ViewportDesc.Height * 0.5f) + 1.f;

	/* viewspace.z == near -> 0.0f */
	/* near����� ���콺�� Ŭ�������Ƿ� ���������̽� ���� z�� 0�̴�. */
	vMousePos.z = 0.f;

	_float4x4		ViewMatrixInv, ProjMatrixInv;	

	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrixInv);
	D3DXMatrixInverse(&ProjMatrixInv, nullptr, &ProjMatrixInv);

	/* �佺���̽� ���� RayDir, RayPos�� ������. */
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &ProjMatrixInv);

	m_vRayPos = _float3(0.f, 0.f, 0.f);
	m_vRayDir = vMousePos - m_vRayPos;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrixInv);
	D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrixInv);

	/* ���彺���̽� ���� RayDir, RayPos�� ������. */
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &ViewMatrixInv);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &ViewMatrixInv);
}

void CPicking::Transform_ToLocalSpace(const _float4x4 * pWorldMatrix)
{
	_float4x4		WorldMatrixInv{};
	D3DXMatrixInverse(&WorldMatrixInv, nullptr, pWorldMatrix);

	D3DXVec3TransformNormal(&m_vRayDir_Local, &m_vRayDir, &WorldMatrixInv);
	D3DXVec3Normalize(&m_vRayDir_Local, &m_vRayDir_Local);
	D3DXVec3TransformCoord(&m_vRayPos_Local, &m_vRayPos, &WorldMatrixInv);
}

//129 x 129���� ����� ���ؽ��� �簢���� �ϳ��� �������� �����ϴ� 3���� ����
// �ͷ����� ������� ����ķ� ���� �ͷ����� ������ ���ý����̽� ������ ��ġ�� RayPos�� RayDir�� Tri �Լ� �Ű������� �ִ´�.
_bool CPicking::Picked_InLocalSpace(const _float3 * pPointA, const _float3 * pPointB, const _float3 * pPointC, _float3 * pPickPos)
{
	_float			fU, fV, fDist;
	_bool			isPicked = { false };

	//�־��� �ﰢ��(pPointA, pPointB, pPointC)�� ���� �����̽������� ����(m_vRayPos_Local, m_vRayDir_Local)�� �����ϴ��� �˻��մϴ�.
	//�����ϸ� isPicked�� true�� �ǰ�, �������� ������ false�� �˴ϴ�.
	//fDist�� ������ ���������κ��� ���� ���������� �Ÿ��Դϴ�.
	isPicked = D3DXIntersectTri(pPointA, pPointB, pPointC, &m_vRayPos_Local, &m_vRayDir_Local, &fU, &fV, &fDist);
	if (true == isPicked)
	{
		//�ͷ����� ���ý����̽� �󿡼� ���� Pos
		*pPickPos = m_vRayPos_Local + *D3DXVec3Normalize(&m_vRayDir_Local, &m_vRayDir_Local) * fDist;
	}

	return isPicked;
}
_bool CPicking::Picked_InWorldSpace(const _float3 * pPointA, const _float3 * pPointB, const _float3 * pPointC, _float3 * pPickPos)
{
	_float			fU, fV, fDist;

	_bool			isPicked = { false };

	isPicked = D3DXIntersectTri(pPointA, pPointB, pPointC, &m_vRayPos, &m_vRayDir, &fU, &fV, &fDist);

	if (true == isPicked)
	{
		*pPickPos = m_vRayPos + *D3DXVec3Normalize(&m_vRayDir, &m_vRayDir) * fDist;
	}

	return isPicked;
}

CPicking * CPicking::Create(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd)
{
	CPicking*		pInstance = new CPicking(pGraphic_Device);

	if (FAILED(pInstance->Initialize(hWnd)))
	{
		MSG_BOX(TEXT("Failed to Created : CPicking"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPicking::Free()
{
	Safe_Release(m_pGraphic_Device);

	__super::Free();
}
