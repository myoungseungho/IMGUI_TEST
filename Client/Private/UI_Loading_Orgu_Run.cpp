#include "stdafx.h"
#include "..\Public\UI_Loading_Orgu_Run.h"

#include "GameInstance.h"

CUI_Loading_Orgu_Run::CUI_Loading_Orgu_Run(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Loading_Orgu_Run::CUI_Loading_Orgu_Run(const CUI_Loading_Orgu_Run& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Loading_Orgu_Run::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUI_Loading_Orgu_Run::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 160.f;
	m_fSizeY = 165.f;
	m_fX = 0.f;
	m_fY = 0.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));

	m_fAlpha = 255.f;

	return S_OK;
}

void CUI_Loading_Orgu_Run::Priority_Update(_float fTimeDelta)
{
}

void CUI_Loading_Orgu_Run::Update(_float fTimeDelta)
{
	//if (GetAsyncKeyState('F') & 0x8000) {
	//	offsetX -= 5.f;
	//}
	//if (GetAsyncKeyState('H') & 0x8000) {
	//	offsetX += 5.f;
	//}
	//if (GetAsyncKeyState('T') & 0x8000) {
	//	offsetY += 5.f;
	//}
	//if (GetAsyncKeyState('G') & 0x8000) {
	//	offsetY -= 5.f;
	//}
	//if (GetAsyncKeyState('R') & 0x8000) {
	//	offsetZ -= 0.01f;
	//}
	//if (GetAsyncKeyState('Y') & 0x8000) {
	//	offsetZ += 0.01f;
	//}
	//if (GetAsyncKeyState('J') & 0x8000) {
	//	offsetXScale -= 5.f;
	//}
	//if (GetAsyncKeyState('K') & 0x8000) {
	//	offsetXScale += 5.f;
	//}
	//if (GetAsyncKeyState('N') & 0x8000) {
	//	offsetYScale -= 5.f;
	//}
	//if (GetAsyncKeyState('M') & 0x8000) {
	//	offsetYScale += 5.f;
	//}
	//if (GetAsyncKeyState(VK_UP) & 0x8000) {
	//	m_fAlpha += 1.f;
	//}
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
	//	m_fAlpha -= 1.f;
	//}
}

void CUI_Loading_Orgu_Run::Late_Update(_float fTimeDelta)
{
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX + offsetXScale, m_fSizeY + offsetYScale, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX + offsetX, m_fY + offsetY, 0.f));
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Loading_Orgu_Run::Render(_float fTimeDelta)
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

void CUI_Loading_Orgu_Run::AnimState(_float _fTimeDelta)
{
	m_pAnimCom->Play_Animator(TEXT("AnimTexture_LoadingOgu_Idle"), 0.5f, _fTimeDelta, true);
}


HRESULT CUI_Loading_Orgu_Run::Ready_Components()
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

HRESULT CUI_Loading_Orgu_Run::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Sprite_Loading_Ogu"), TEXT("AnimTexture_LoadingOgu_Idle"));

	return S_OK;
}


CUI_Loading_Orgu_Run* CUI_Loading_Orgu_Run::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Loading_Orgu_Run* pInstance = new CUI_Loading_Orgu_Run(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Loading_Orgu_Run"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Loading_Orgu_Run::Clone(void* pArg)
{
	CUI_Loading_Orgu_Run* pInstance = new CUI_Loading_Orgu_Run(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Loading_Orgu_Run"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading_Orgu_Run::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pAnimCom);

	__super::Free();
}
