#include "stdafx.h"

#include "Skill_Arror.h"

CSkill_Arror::CSkill_Arror(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CSkill_Monster{ pGraphic_Device }
{
}

CSkill_Arror::CSkill_Arror(const CSkill_Arror& Prototype)
	:CSkill_Monster{ Prototype }
{
}

HRESULT CSkill_Arror::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Arror::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SKILL_MONSTER__DESC* pDesc = static_cast<SKILL_MONSTER__DESC*>(pArg);
	m_pTargetTransform = pDesc->pTargetTransform;

	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_pTargetTransform->Get_State(CTransform::STATE_POSITION));


	return S_OK;
}

void CSkill_Arror::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Arror::Update(_float fTimeDelta)
{
	_float3 vPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vDirPos = {};
	_float fDirAngle = m_pTargetTransform->Dir_Degree();

		if (fDirAngle >= 337.5f || fDirAngle <= 22.5f)
		{
			m_iDirTexture = 0;
			vDirPos.x = vPos.x;
			vDirPos.y = vPos.y;
			vDirPos.z = vPos.z +0.5f;

		}
		else if (fDirAngle <= 67.5f)
		{
			m_iDirTexture = 1;

			vDirPos.x = vPos.x + 0.5f;
			vDirPos.y = vPos.y;
			vDirPos.z = vPos.z + 0.5f;
		}
		else if (fDirAngle <= 112.5f)
		{
			m_iDirTexture = 6;

			vDirPos.x = vPos.x + 0.5f;
			vDirPos.y = vPos.y;
			vDirPos.z = vPos.z;
		}
		else if (fDirAngle <= 157.5f)
		{
			m_iDirTexture = 7;

			vPos.x = vPos.x + 0.5f; 
			vPos.z = vPos.z - 0.5f;
		}
		else if (fDirAngle <= 202.5f)
		{
			m_iDirTexture = 4;

			vPos.z = vPos.z - 0.5f;
		}
		else if (fDirAngle <= 247.5f)
		{
			m_iDirTexture = 5;

			vPos.x = vPos.x - 0.5f;
			vPos.z = vPos.z - 0.5f;
		}
		else if (fDirAngle <= 292.5f)
		{
			m_iDirTexture = 2;

			vPos.x = vPos.x - 0.5f;
		}
		else if (fDirAngle < 337.5f)
		{
			m_iDirTexture = 3;

			vPos.x = vPos.x - 0.5f;
			vPos.z = vPos.z + 0.5f;
		}
		else
		{
			MSG_BOX(TEXT("Dir_Error"));
		}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vDirPos);
}

void CSkill_Arror::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this); 
}

HRESULT CSkill_Arror::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iDirTexture)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Arror::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Arror"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(0.5f, 0.5f, 1.f));


	return S_OK;
}

HRESULT CSkill_Arror::Ready_Animation()
{
	return S_OK;
}

HRESULT CSkill_Arror::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSkill_Arror::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CSkill_Arror::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
}

void CSkill_Arror::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CSkill_Arror::OnCollisionExit(CCollider* other)
{
}

void CSkill_Arror::Destroy(_float fTimeDelta)
{
}

CSkill_Arror* CSkill_Arror::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkill_Arror* pInstance = new CSkill_Arror(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Skill_Arror"));
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject* CSkill_Arror::Clone(void* pArg)
{
	CSkill_Arror* pInstance = new CSkill_Arror(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : Skill_Arror"));
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CSkill_Arror::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
