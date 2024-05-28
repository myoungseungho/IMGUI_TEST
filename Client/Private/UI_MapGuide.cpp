#include "stdafx.h"
#include "..\Public\UI_MapGuide.h"
#include <sstream>
#include "GameInstance.h"

CUI_MapGuide::CUI_MapGuide(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_MapGuide::CUI_MapGuide(const CUI_MapGuide& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_MapGuide::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

void CUI_MapGuide::Font_Initialize()
{
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	// ��Ʈ ���� - m_pName_Font
	tFontInfo.Height = 40;
	tFontInfo.Width = 30;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	wcscpy_s(tFontInfo.FaceName, LF_FACESIZE, TEXT("Cafe24 Ssurround air OTF Light"));

	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pName_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentPlayerMoney_Font Failed");
		return;
	}
}

HRESULT CUI_MapGuide::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 340.f;
	m_fSizeY = 120.f;
	m_fX = 0.f;
	m_fY = m_fStartY; // �ʱ� ��ġ�� ���� ��ġ�� ����
	m_fTextY = m_fTextStartY; // �ؽ�Ʈ �ʱ� ��ġ�� ����

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));


	Font_Initialize();

	m_fAlpha = 255.f;
	return S_OK;
}

void CUI_MapGuide::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsMapGuide) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
}

void CUI_MapGuide::Update(_float fTimeDelta)
{
	if (!m_bIsMapGuide) return; // ������Ʈ�� ��Ȱ��ȭ�� ��� ��ȯ

	m_fCreateTime += fTimeDelta;
	UpdatePosition(fTimeDelta);
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

	// Ű �Է� ó��
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_fTextY -= 10.f; // �������� �̵�
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_fTextY += 10.f; // �Ʒ������� �̵�
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_TextPosX -= 10.f; // �������� �̵�
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_TextPosX += 10.f; // ���������� �̵�
	}
}

void CUI_MapGuide::Late_Update(_float fTimeDelta)
{
	if (!m_bIsMapGuide)
		return;

	__super::Late_Update(fTimeDelta);

	/*m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(offsetX + m_fX, offsetY + m_fY, 0));
	m_pTransformCom->Set_Scaled(_float3(offsetXScale + m_fSizeX, offsetYScale + m_fSizeY, 0));*/

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_MapGuide::Render(_float fTimeDelta)
{
	if (!m_bIsMapGuide)
		return S_OK;

	__super::Begin_RenderState();

	wchar_t text[256];
	RECT rect;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	// �ؽ�Ʈ ������ - �̸�
	swprintf_s(text, L"%s", m_WstringName.c_str());
	SetRect(&rect, static_cast<int>(520.f), static_cast<int>(m_fTextY), 0, 0); // �ؽ�Ʈ�� ����� ��ġ ����
	m_pName_Font->DrawText(
		NULL,
		text,
		-1,
		&rect,
		DT_NOCLIP,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);

	__super::End_RenderState();

	return S_OK;
}



void CUI_MapGuide::SetIsNpcTalkOn(_bool _isOn)
{
	m_fCreateTime = 0.0f;

	if (_isOn)
	{
		m_bIsMapGuide = true;
		m_fY = m_fStartY; // �ִϸ��̼��� ���� �ʱ� ��ġ�� ����
		m_fTextY = m_fTextStartY; // �ؽ�Ʈ �ʱ� ��ġ�� ����
		m_fAnimTime = 0.0f; // �ִϸ��̼� �ð� �ʱ�ȭ
		m_bIsReturning = false; // ���ư��� ������ ���� �ʱ�ȭ
		m_fWaitTime = 0.0f; // ��� �ð� �ʱ�ȭ
	}
	else
	{
		m_bIsMapGuide = false;
	}
}

void CUI_MapGuide::UpdatePosition(_float fTimeDelta)
{
	if (m_fAnimTime < m_fAnimDuration)
	{
		m_fAnimTime += fTimeDelta;
		_float progress = m_fAnimTime / m_fAnimDuration;
		m_fY = m_fStartY + (m_fTargetY - m_fStartY) * progress;
		m_fTextY = m_fTextStartY + (m_fTextTargetY - m_fTextStartY) * progress;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));
	}
	else if (!m_bIsReturning)
	{
		m_fWaitTime += fTimeDelta;
		if (m_fWaitTime >= m_fWaitDuration)
		{
			m_bIsReturning = true;
			m_fAnimTime = 0.0f;
			m_fWaitTime = 0.0f;
			std::swap(m_fStartY, m_fTargetY); // ���� ��ġ�� Ÿ�� ��ġ�� �ٲ�
			std::swap(m_fTextStartY, m_fTextTargetY); // �ؽ�Ʈ ���� ��ġ�� Ÿ�� ��ġ�� �ٲ�
		}
	}
	else if (m_bIsReturning && m_fAnimTime < m_fAnimDuration)
	{
		m_fAnimTime += fTimeDelta;
		_float progress = m_fAnimTime / m_fAnimDuration;
		m_fY = m_fTargetY + (m_fStartY - m_fTargetY) * (1.0f - progress);
		m_fTextY = m_fTextTargetY + (m_fTextStartY - m_fTextTargetY) * (1.0f - progress);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));
	}
	else
	{
		m_bIsMapGuide = false; // �ִϸ��̼� ���� �� ��Ȱ��ȭ
	}
}

HRESULT CUI_MapGuide::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_BoxShadow"),
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

CUI_MapGuide* CUI_MapGuide::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_MapGuide* pInstance = new CUI_MapGuide(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_MapGuide"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MapGuide::Clone(void* pArg)
{
	CUI_MapGuide* pInstance = new CUI_MapGuide(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_MapGuide"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MapGuide::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pName_Font);
	__super::Free();
}
