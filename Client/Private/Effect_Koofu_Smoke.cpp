#include "stdafx.h"

#include "Effect_Koofu_Smoke.h"

CEffect_Koofu_Smoke::CEffect_Koofu_Smoke(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect_Monster{ pGraphic_Device }
{
}

CEffect_Koofu_Smoke::CEffect_Koofu_Smoke(const CEffect_Koofu_Smoke& Prototype)
	:CEffect_Monster{ Prototype }
{
}

HRESULT CEffect_Koofu_Smoke::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Koofu_Smoke::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	EFFECT_SMOKE_DESC* pDesc = static_cast<EFFECT_SMOKE_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	m_iSmokeNum = pDesc->iSmokeNum;

	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	MaterialDesc = RED_MTRL;

	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
	LightDesc.Diffuse = D3DXCOLOR(1.0f, 1.f, 1.f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(1.0f, 1.f, 1.f, 1.f);
	LightDesc.Direction = _float3(1.f, -1.f, 1.f);

	_float3 vPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	vPos.z = m_pTargetTransform->Get_State(CTransform::STATE_POSITION).z - 1.f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);

	_float m_fAngleRange = 360.f;
	_float fStartRange = m_fAngleRange * 0.5f;
	_float fAngle_Per_Piece = m_fAngleRange / 10.f;

	m_pTransformCom->Rotation(_float3(0.f, 0.f, 1.f), (m_pTargetTransform->Dir_Degree() - fStartRange + fAngle_Per_Piece * m_iSmokeNum) * D3DX_PI / 180.f);

	return S_OK;
}

void CEffect_Koofu_Smoke::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Koofu_Smoke::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_VectorUp(fTimeDelta);
}

void CEffect_Koofu_Smoke::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
	Destroy(fTimeDelta);
}

HRESULT CEffect_Koofu_Smoke::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Koofu_Smoke::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Texture_Smoke"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL; 

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 2.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(2.5f, 3.f, 1.f));


	return S_OK;
}

HRESULT CEffect_Koofu_Smoke::Ready_Animation()
{
	return S_OK;
}

HRESULT CEffect_Koofu_Smoke::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 컬링 모드 설정
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 텍스처 페이저 설정
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(static_cast<DWORD>(150.f), 255, 255, 255));
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pGraphic_Device->SetMaterial(&MaterialDesc);

	if (FAILED(m_pGraphic_Device->SetLight(0, &LightDesc)))
		return E_FAIL;

	m_pGraphic_Device->LightEnable(0, TRUE);

	return S_OK;
}

HRESULT CEffect_Koofu_Smoke::End_RenderState()
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

void CEffect_Koofu_Smoke::Destroy(_float fTimeDelta)
{
	CEffect_Koofu_Smoke* pThis = this;

	if (m_pTimerCom->Time_Limit(fTimeDelta, 1.5f))
		Safe_Release(pThis);

}

D3DMATERIAL9 CEffect_Koofu_Smoke::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

CEffect_Koofu_Smoke* CEffect_Koofu_Smoke::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Koofu_Smoke* pInstance = new CEffect_Koofu_Smoke(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Effect_Koofu_Smoke"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

//쌤코드는 GameObject/ 내가 판단하기로는 BlendObject 일단 보류
CGameObject* CEffect_Koofu_Smoke::Clone(void* pArg)
{
	CEffect_Koofu_Smoke* pInstance = new CEffect_Koofu_Smoke(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Effect_Koofu_Smoke"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Koofu_Smoke::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);

	__super::Free();
}
