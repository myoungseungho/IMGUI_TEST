#include "..\Public\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CTransform::CTransform(const CTransform & Prototype)
	: CComponent{ Prototype }
	, m_WorldMatrix{ Prototype.m_WorldMatrix }
{
	
}

void CTransform::Set_Scaled(const _float3 & vScaled)
{
	_float3		vRight = Get_State(STATE_RIGHT);
	Set_State(STATE_RIGHT, &(*D3DXVec3Normalize(&vRight, &vRight) * vScaled.x));

	_float3		vUp = Get_State(STATE_UP);
	Set_State(STATE_UP, &(*D3DXVec3Normalize(&vUp, &vUp) * vScaled.y));

	_float3		vLook = Get_State(STATE_LOOK);
	Set_State(STATE_LOOK, &(*D3DXVec3Normalize(&vLook, &vLook) * vScaled.z));
}

HRESULT CTransform::Initialize_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{	

	TRANSFORM_DESC*		pDesc = static_cast<TRANSFORM_DESC*>(pArg);

	m_fSpeedPerSec = pDesc->fSpeedPerSec;
	m_fRotationPerSec = pDesc->fRotationPerSec;

	return S_OK;
}

HRESULT CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;	
}

HRESULT CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Straight_Left(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	_float3 vLook = Get_State(STATE_LOOK);
	_float3 vRight = Get_State(STATE_RIGHT);

	_float3 vDir = vLook - vRight;
	D3DXVec3Normalize(&vDir, &vDir);

	vPosition += vDir * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Straight_Right(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	_float3 vLook = Get_State(STATE_LOOK);
	_float3 vRight = Get_State(STATE_RIGHT);

	_float3 vDir = vLook + vRight;
	D3DXVec3Normalize(&vDir, &vDir);

	vPosition += vDir * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Backward_Left(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	_float3 vLook = Get_State(STATE_LOOK);
	_float3 vRight = Get_State(STATE_RIGHT);

	_float3 vDir = -vLook - vRight;
	D3DXVec3Normalize(&vDir, &vDir);

	vPosition += vDir * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Backward_Right(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	_float3 vLook = Get_State(STATE_LOOK);
	_float3 vRight = Get_State(STATE_RIGHT);

	_float3 vDir = -vLook + vRight;
	D3DXVec3Normalize(&vDir, &vDir);

	vPosition += vDir * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Up(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	_float3 vDirection = _float3(0.f, 1.f, 0.f);

	vPosition += vDirection * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Down(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	_float3 vDirection = _float3(0.f, -1.f, 0.f);

	vPosition += vDirection * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;
}

HRESULT CTransform::Go_VectorDown(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vUp = Get_State(STATE_UP);

	vPosition -= *D3DXVec3Normalize(&vUp, &vUp) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;
}

HRESULT CTransform::Go_VectorUp(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vUp = Get_State(STATE_UP);

	vPosition += *D3DXVec3Normalize(&vUp, &vUp) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;
}

HRESULT CTransform::Gravity(_float fWeight, _float fLandPosY, _float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	_float3 vDirection = _float3(0.f, -1.f, 0.f);

	m_fGravity_Weight += fWeight;

	vPosition += vDirection * m_fGravity_Weight * fTimeDelta;

	if (vPosition.y <= fLandPosY)
	{
		vPosition.y = fLandPosY;
		m_fGravity_Weight = { 0.f };
	}

	Set_State(STATE_POSITION, &vPosition);

	return S_OK;
}

void CTransform::Turn(const _float3 & vAxis, _float fTimeDelta)
{
	_float3		vRight = Get_State(STATE_RIGHT);
	_float3		vUp = Get_State(STATE_UP);
	_float3		vLook = Get_State(STATE_LOOK);

	_float4x4	RotationMatrix{};
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_fRotationPerSec * fTimeDelta);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, &vRight);
	Set_State(STATE_UP, &vUp);
	Set_State(STATE_LOOK, &vLook);
}

void CTransform::Radian_Turn(const _float3& vAxis, _float fRadian)
{
	_float3		vRight = Get_State(STATE_RIGHT);
	_float3		vUp = Get_State(STATE_UP);
	_float3		vLook = Get_State(STATE_LOOK);

	_float4x4	RotationMatrix{};
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, &vRight);
	Set_State(STATE_UP, &vUp);
	Set_State(STATE_LOOK, &vLook);
}

void CTransform::Init_Rotation(const _float3& vAxis, _float fRadian)
{
	_float3		vScaled = Get_Scaled();

	_float3		vRight = _float3(1.f, 0.f, 0.f) * vScaled.x;
	_float3		vUp = _float3(0.f, 1.f, 0.f) * vScaled.y;
	_float3		vLook = _float3(0.f, 0.f, 1.f) * vScaled.z;

	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, &vRight);
	Set_State(STATE_UP, &vUp);
	Set_State(STATE_LOOK, &vLook);
}

void CTransform::Rotation(const _float3& vAxis, _float fRadian)
{
	_float3 vScaled = Get_Scaled();

	_float3 vRight = Get_State(STATE_RIGHT);
	_float3 vUp = Get_State(STATE_UP);
	_float3 vLook = Get_State(STATE_LOOK);

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	_float4x4 RotationMatrix{};
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	vRight *= vScaled.x;
	vUp *= vScaled.y;
	vLook *= vScaled.z;

	Set_State(STATE_RIGHT, &vRight);
	Set_State(STATE_UP, &vUp);
	Set_State(STATE_LOOK, &vLook);
}

void CTransform::LookAt(const _float3 & vTargetPos)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = vTargetPos - vPosition;

	_float3		vRight;
	D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);

	_float3		vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	_float3		vScaled = Get_Scaled();

	Set_State(STATE_RIGHT, &(*D3DXVec3Normalize(&vRight, &vRight) * vScaled.x));
	Set_State(STATE_UP, &(*D3DXVec3Normalize(&vUp, &vUp) * vScaled.y));
	Set_State(STATE_LOOK, &(*D3DXVec3Normalize(&vLook, &vLook) * vScaled.z));
}

void CTransform::LookAt_ForLandObject(const _float3 & vTargetPos)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vLook = vTargetPos - vPosition;

	_float3		vRight;
	D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);

	D3DXVec3Cross(&vLook, &vRight, &_float3(0.f, 1.f, 0.f));
		
	_float3		vScaled = Get_Scaled();

	Set_State(STATE_RIGHT, &(*D3DXVec3Normalize(&vRight, &vRight) * vScaled.x));	
	Set_State(STATE_LOOK, &(*D3DXVec3Normalize(&vLook, &vLook) * vScaled.z));
}

void CTransform::Chase(const _float3 & vTargetPos, _float fTimeDelta, _float fMinDistance)
{
	_float3			vPosition = Get_State(STATE_POSITION);

	_float3			vMoveDir = vTargetPos - vPosition;

	_float			fDistance = D3DXVec3Length(&vMoveDir);

	if (fDistance > fMinDistance)
	{
		vPosition += *D3DXVec3Normalize(&vMoveDir, &vMoveDir) * m_fSpeedPerSec * fTimeDelta;

		Set_State(STATE_POSITION, &vPosition);
	}	
}

void CTransform::Away(const _float3& vTargetPos, _float fTimeDelta, _float fMinDistance)
{
	_float3			vPosition = Get_State(STATE_POSITION);

	_float3			vMoveDir = vPosition - vTargetPos ;

	_float			fDistance = D3DXVec3Length(&vMoveDir);

	if (fDistance < fMinDistance)
	{
		vPosition += *D3DXVec3Normalize(&vMoveDir, &vMoveDir) * m_fSpeedPerSec * fTimeDelta;

		Set_State(STATE_POSITION, &vPosition);
	}
}

_float CTransform::Dir_Degree()
{
	_float3		vLook = Get_State(STATE_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);

	_float3		vAxis(0.f, 0.f, 1.f);

	_float fAngle = D3DXVec3Dot(&vLook, &vAxis);

	fAngle = acos(fAngle);

	if (vLook.x < 0.f)
		fAngle = D3DX_PI * 2 - fAngle;

	fAngle = D3DXToDegree(fAngle);

	return fAngle;
}

_float CTransform::Target_Dir_Degree(const _float3& vTargetPos)
{
	_float3	vPosition = Get_State(STATE_POSITION);
	_float3 vDir = vTargetPos - vPosition  ;

	D3DXVec3Normalize(&vDir, &vDir);

	_float3		vAxis(0.f, 0.f, 1.f);

	_float fAngle = D3DXVec3Dot(&vDir, &vAxis);

	fAngle = acos(fAngle);

	if (vDir.x < 0.f)
		fAngle = D3DX_PI * 2 - fAngle;

	fAngle = D3DXToDegree(fAngle);

	return fAngle;
}

HRESULT CTransform::Bind_WorldMatrix()
{
	return m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);	
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*		pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CTransform::Clone(void * pArg)
{
	CTransform*		pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
