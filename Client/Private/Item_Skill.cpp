#include "stdafx.h"
#include "..\Public\Item_Skill.h"

#include "GameInstance.h"

CItem_Skill::CItem_Skill(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CItem_Skill::CItem_Skill(const CItem_Skill& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CItem_Skill::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CItem_Skill::Initialize(void* pArg)
{
	ITEM_SKILL_DESC* pDesc = static_cast<ITEM_SKILL_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x, vTargetPos.y + 2.f, vTargetPos.z - 0.01f));

	return S_OK;
}

void CItem_Skill::Priority_Update(_float fTimeDelta)
{
}

void CItem_Skill::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_pTimerCom->Time_Limit(fTimeDelta, 2.f))
	{
		m_Died = true;
	}
}

void CItem_Skill::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CItem_Skill::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

HRESULT CItem_Skill::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Item_Skill"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

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

	/* For.Com_Calc_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Calc_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
		return E_FAIL;

	return S_OK;
}

CItem_Skill* CItem_Skill::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_Skill* pInstance = new CItem_Skill(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_Skill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CItem_Skill::Clone(void* pArg)
{
	CItem_Skill* pInstance = new CItem_Skill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CItem_Skill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Skill::Free()
{
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}

