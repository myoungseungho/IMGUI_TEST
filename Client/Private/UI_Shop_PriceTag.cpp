#include "stdafx.h"
#include "..\Public\UI_Shop_PriceTag.h"

#include "GameInstance.h"

CUI_Shop_PriceTag::CUI_Shop_PriceTag(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Shop_PriceTag::CUI_Shop_PriceTag(const CUI_Shop_PriceTag& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Shop_PriceTag::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUI_Shop_PriceTag::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	PRICEUIDATA* uiData = reinterpret_cast<PRICEUIDATA*>(pArg);

	m_fSizeX = uiData->scale.x;
	m_fSizeY = uiData->scale.y;
	m_fX = uiData->position.x;
	m_fY = uiData->position.y;
	m_fAlpha = uiData->alpha;
	m_iIndex = uiData->index;
	m_iPrice = uiData->price;
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));

	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	// 폰트 설정 - CurrentPlayerMoney
	tFontInfo.Height = 18;
	tFontInfo.Width = 13;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	wcscpy_s(tFontInfo.FaceName, LF_FACESIZE, TEXT("Cafe24 Ssurround air OTF Light"));

	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pPirce_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentPlayerMoney_Font Failed");
		return E_FAIL;
	}
	return S_OK;
}

void CUI_Shop_PriceTag::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn이 false이면 업데이트를 수행하지 않음
}

void CUI_Shop_PriceTag::Update(_float fTimeDelta)
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

	//if (GetAsyncKeyState(VK_UP) & 0x8000) {
	//	m_TextPosY -= 1.f;
	//}
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
	//	m_TextPosY += 1.f;
	//}
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
	//	m_TextPosX -= 1.f;
	//}
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
	//	m_TextPosX += 1.f;
	//}
}

void CUI_Shop_PriceTag::Late_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn이 false이면 업데이트를 수행하지 않음
	__super::Late_Update(fTimeDelta);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(offsetX + m_fX, offsetY + m_fY, 0));
	m_pTransformCom->Set_Scaled(_float3(offsetXScale + m_fSizeX, offsetYScale + m_fSizeY, 0));

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Shop_PriceTag::Render(_float fTimeDelta)
{
	if (!m_bIsOn) return S_OK; // m_bIsOn이 false이면 렌더링을 수행하지 않음
	__super::Begin_RenderState();

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	// 텍스트 형식화 및 렌더링
	wchar_t text[256];
	RECT rect;

	// 폰트 좌표계 보정
	int winCenterX = g_iWinSizeX / 2;
	int winCenterY = g_iWinSizeY / 2;
	// m_iPrice를 사용하여 텍스트 내용을 설정
	swprintf_s(text, L"%d", m_iPrice);  // m_iPrice는 멤버 변수
	SetRect(&rect, static_cast<int>(winCenterX + m_fX) + m_TextPosX, static_cast<int>(winCenterY - m_fY) + m_TextPosY, 0, 0); // 텍스트를 출력할 위치 변경
	m_pPirce_Font->DrawText(
		NULL,
		text,
		-1,
		&rect,
		DT_NOCLIP,
		D3DCOLOR_ARGB(255, 0, 0, 0)
	);

	__super::End_RenderState();

	return S_OK;
}

HRESULT CUI_Shop_PriceTag::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Shop_PriceTag"),
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

CUI_Shop_PriceTag* CUI_Shop_PriceTag::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Shop_PriceTag* pInstance = new CUI_Shop_PriceTag(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Shop_PriceTag"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Shop_PriceTag::Clone(void* pArg)
{
	CUI_Shop_PriceTag* pInstance = new CUI_Shop_PriceTag(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Shop_PriceTag"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Shop_PriceTag::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pPirce_Font);
	__super::Free();
}
