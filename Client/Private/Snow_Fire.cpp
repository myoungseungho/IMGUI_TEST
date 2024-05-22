#include "stdafx.h"
#include "..\Public\Snow_Fire.h"

#include "GameInstance.h"

CSnow_Fire::CSnow_Fire(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CSnow_Fire::CSnow_Fire(const CSnow_Fire& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CSnow_Fire::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CSnow_Fire::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (m_bIsPasingObject)
	{
		FILEDATA* fileData = static_cast<FILEDATA*>(pArg);
		m_pTransformCom->Set_Scaled(_float3(fileData->scale.x, fileData->scale.y, fileData->scale.z));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(fileData->position.x, fileData->position.y, fileData->position.z));
	}

	return S_OK;
}

void CSnow_Fire::Priority_Update(_float fTimeDelta)
{
}

void CSnow_Fire::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CSnow_Fire::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSnow_Fire::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	AnimState(fTimeDelta);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}


HRESULT CSnow_Fire::Ready_Components()
{
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSnow_Fire::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Snow_Fire_Idle"), TEXT("AnimTexture_Snow_Fire_Idle"));

	return S_OK;
}

void CSnow_Fire::AnimState(_float _fTimeDelta)
{
	switch (m_eAnimState)
	{
	case ANIMATION_STATE::ANIM_IDLE:
		m_pAnimCom->Play_Animator(TEXT("AnimTexture_Snow_Fire_Idle"), 1.f, _fTimeDelta, true);
		break;
	}
}

CSnow_Fire* CSnow_Fire::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSnow_Fire* pInstance = new CSnow_Fire(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSnow_Fire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CSnow_Fire::Clone(void* pArg)
{
	CSnow_Fire* pInstance = new CSnow_Fire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CSnow_Fire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSnow_Fire::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pAnimCom);
}
