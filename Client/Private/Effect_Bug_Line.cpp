#include "stdafx.h"

#include "Effect_Bug_Line.h"

CEffect_Bug_Line::CEffect_Bug_Line(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect_Monster{ pGraphic_Device }
{
}

CEffect_Bug_Line::CEffect_Bug_Line(const CEffect_Bug_Line& Prototype)
	:CEffect_Monster{ Prototype }
{
}

HRESULT CEffect_Bug_Line::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Bug_Line::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	EFFECT_BUG_LINE_DESC* pDesc = static_cast<EFFECT_BUG_LINE_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	m_pPlayerTransform = pDesc->pPlayerTransform;

	Safe_AddRef(m_pTargetTransform);
	Safe_AddRef(m_pPlayerTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_pTargetTransform->Get_State(CTransform::STATE_POSITION));

	m_pTransformCom->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
	_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	m_pTransformCom->Radian_Turn(vRight, 270.f * D3DX_PI / 180.f);


	return S_OK;
}

void CEffect_Bug_Line::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Bug_Line::Update(_float fTimeDelta)
{
	if (m_pTimerCom->Time_Update(fTimeDelta, 3.f))
	{
		m_fLength += 10.f;
		m_pTransformCom->Set_Scaled(_float3(5.f, m_fLength, 1.f));
	}
	else
		Destroy(fTimeDelta);


}

void CEffect_Bug_Line::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CEffect_Bug_Line::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	if (FAILED(m_pAnimCom->Play_Animator(TEXT("EFFECT_BUG_LINE"), 0.5f, fTimeDelta, true)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Bug_Line::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 0.0f;
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

HRESULT CEffect_Bug_Line::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Effect_MothFlyLine"), TEXT("EFFECT_BUG_LINE"));
	return S_OK;
}

HRESULT CEffect_Bug_Line::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 컬링 모드 설정
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 텍스처 페이저 설정
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(static_cast<DWORD>(200.f), 255, 255, 255));
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return S_OK;
}

HRESULT CEffect_Bug_Line::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	// 알파 블렌딩 비활성화
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// 원래의 컬링 모드로 복원
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 알파 블렌딩 비활성화 및 텍스처 스테이지 상태 복원
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	return S_OK;
}

void CEffect_Bug_Line::Destroy(_float fTimeDelta)
{
	CEffect_Bug_Line* pThis = this;

	Safe_Release(pThis);
}

CEffect_Bug_Line* CEffect_Bug_Line::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Bug_Line* pInstance = new CEffect_Bug_Line(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Effect_Bug_Line"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Bug_Line::Clone(void* pArg)
{
	CEffect_Bug_Line* pInstance = new CEffect_Bug_Line(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Effect_Bug_Line"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Bug_Line::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pPlayerTransform);
	Safe_Release(m_pAnimCom);

	__super::Free();
}
