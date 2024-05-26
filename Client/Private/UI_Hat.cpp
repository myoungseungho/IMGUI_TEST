#include "stdafx.h"
#include "..\Public\UI_Hat.h"

#include "GameInstance.h"

CUI_Hat::CUI_Hat(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Hat::CUI_Hat(const CUI_Hat& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Hat::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUI_Hat::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	UIDATA* uiData = reinterpret_cast<UIDATA*>(pArg);

	m_fSizeX = uiData->scale.x;
	m_fSizeY = uiData->scale.y;
	m_fX = uiData->position.x;
	m_fY = uiData->position.y;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));

	m_fAlpha = uiData->alpha;
	m_iIndex = uiData->index;

	return S_OK;
}

void CUI_Hat::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn이 false이면 업데이트를 수행하지 않음
}

void CUI_Hat::Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn이 false이면 업데이트를 수행하지 않음

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
	/*if (GetAsyncKeyState(VK_UP) & 0x8000) {
		m_fAlpha += 1.f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		m_fAlpha -= 1.f;
	}*/
}

void CUI_Hat::Late_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn이 false이면 업데이트를 수행하지 않음

	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Hat::Render(_float fTimeDelta)
{
	if (!m_bIsOn) return S_OK; // m_bIsOn이 false이면 업데이트를 수행하지 않음

	__super::Begin_RenderState();

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(m_iIndex)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

HRESULT CUI_Hat::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_Hat"),
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

	return S_OK;
}

CUI_Hat* CUI_Hat::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Hat* pInstance = new CUI_Hat(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Hat"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Hat::Clone(void* pArg)
{
	CUI_Hat* pInstance = new CUI_Hat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Hat"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Hat::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
