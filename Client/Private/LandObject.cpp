#include "stdafx.h"
#include "..\Public\LandObject.h"

#include "GameInstance.h"

CLandObject::CLandObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CLandObject::CLandObject(const CLandObject & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CLandObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::Initialize(void * pArg)
{
	LANDOBJECT_DESC*		pDesc = static_cast<LANDOBJECT_DESC*>(pArg);

	m_pTerrainBuffer = pDesc->m_pTerrainBuffer;
	Safe_AddRef(m_pTerrainBuffer);

	m_pTerrainTransform = pDesc->m_pTerrainTransform;
	Safe_AddRef(m_pTerrainTransform);

	return S_OK;
}

void CLandObject::Priority_Update(_float fTimeDelta)
{
}

void CLandObject::Update(_float fTimeDelta)
{
}

void CLandObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CLandObject::Render()
{
	return S_OK;
}

void CLandObject::SetUp_OnTerrain(CTransform * pTargetTransform, _float fOffset)
{
	_float3		vWorldPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
	_float3		vLocalPos = *D3DXVec3TransformCoord(&vLocalPos, &vWorldPos, &m_pTerrainTransform->Get_WorldMatrixInverse());

	_float		fHeight = m_pTerrainBuffer->Compute_Height(vLocalPos);

	vLocalPos.y = fHeight + fOffset;

	D3DXVec3TransformCoord(&vWorldPos, &vLocalPos, &m_pTerrainTransform->Get_WorldMatrix());

	pTargetTransform->Set_State(CTransform::STATE_POSITION, &vWorldPos);


}

void CLandObject::Free()
{
	__super::Free();

	Safe_Release(m_pTerrainTransform);
	Safe_Release(m_pTerrainBuffer);
}
