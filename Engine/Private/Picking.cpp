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

	/* Viewport상의 마우스 좌표를 구한다. */
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	_float3		vMousePos;
	/* 투영스페이스 상의 마우스 좌표를 구하자. */
	/* x : 0 -> -1, winsizex -> 1 */
	vMousePos.x = ptMouse.x / (ViewportDesc.Width * 0.5f) - 1.f;

	/* y : 0 -> 1, winsizey -> -1 */
	vMousePos.y = ptMouse.y / -(ViewportDesc.Height * 0.5f) + 1.f;

	/* viewspace.z == near -> 0.0f */
	/* near평면을 마우스가 클릭했으므로 투영스페이스 상의 z는 0이다. */
	vMousePos.z = 0.f;

	_float4x4		ViewMatrixInv, ProjMatrixInv;	

	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrixInv);
	D3DXMatrixInverse(&ProjMatrixInv, nullptr, &ProjMatrixInv);

	/* 뷰스페이스 상의 RayDir, RayPos를 구하자. */
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &ProjMatrixInv);

	m_vRayPos = _float3(0.f, 0.f, 0.f);
	m_vRayDir = vMousePos - m_vRayPos;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrixInv);
	D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrixInv);

	/* 월드스페이스 상의 RayDir, RayPos를 구하자. */
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

//129 x 129개가 만드는 버텍스의 사각형중 하나의 폴리곤을 구성하는 3개의 점과
// 터레인의 월드행렬 역행렬로 인해 터레인의 정점의 로컬스페이스 공간에 위치한 RayPos와 RayDir를 Tri 함수 매개변수에 넣는다.
_bool CPicking::Picked_InLocalSpace(const _float3 * pPointA, const _float3 * pPointB, const _float3 * pPointC, _float3 * pPickPos)
{
	_float			fU, fV, fDist;
	_bool			isPicked = { false };

	//주어진 삼각형(pPointA, pPointB, pPointC)과 로컬 스페이스에서의 레이(m_vRayPos_Local, m_vRayDir_Local)가 교차하는지 검사합니다.
	//교차하면 isPicked가 true가 되고, 교차하지 않으면 false가 됩니다.
	//fDist는 레이의 시작점으로부터 교차 지점까지의 거리입니다.
	isPicked = D3DXIntersectTri(pPointA, pPointB, pPointC, &m_vRayPos_Local, &m_vRayDir_Local, &fU, &fV, &fDist);
	if (true == isPicked)
	{
		//터레인의 로컬스페이스 상에서 찍힌 Pos
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
