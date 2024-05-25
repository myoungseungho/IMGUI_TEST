#include "stdafx.h"
#include "..\Public\UI_HP_Player.h"
#include "Player.h"
#include "GameInstance.h"

CUI_HP_Player::CUI_HP_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_HP_Player::CUI_HP_Player(const CUI_HP_Player& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_HP_Player::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CUI_HP_Player::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 235.f;
	m_fSizeY = 50.f;
	m_fX = -450.f;
	m_fY = 305.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));

	m_fAlpha = 255.f;

	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 25;
	tFontInfo.Width = 15;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;

	wcscpy_s(tFontInfo.FaceName, LF_FACESIZE, /*TEXT("ī��24 �����")*/TEXT("Cafe24 Ssurround air OTF Light"));

	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pFont)))
	{
		MSG_BOX(L"CreateFontIndirect Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CUI_HP_Player::Priority_Update(_float fTimeDelta)
{
}

void CUI_HP_Player::Update(_float fTimeDelta)
{
	
	//// Ű �Է� ó��
	//if (GetAsyncKeyState(VK_UP) & 0x8000)
	//{
	//	m_TextPosY -= 1.0f; // �������� �̵�
	//}
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//{
	//	m_TextPosY += 1.0f; // �Ʒ������� �̵�
	//}
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//{
	//	m_TextPosX -= 1.0f; // �������� �̵�
	//}
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//{
	//	m_TextPosX += 1.0f; // ���������� �̵�
	//}
}

void CUI_HP_Player::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);


	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_HP_Player::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	/* �簢������ �ø��� ���� �ױ��ĸ� �̸� ��ġ�� ���ε��Ѵ�.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	CGameObject* gameobject = m_pGameInstance->Get_GameObject(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Player"));
	CPlayer* player = static_cast<CPlayer*>(gameobject);
	_uint currentHp = player->Get_Player_Hp();
	_uint maxHp = player->Get_Player_MaxHp();

	// �ؽ�Ʈ ����ȭ
	wchar_t text[256];
	swprintf_s(text, L"%d / %d", currentHp, maxHp);

	// �ؽ�Ʈ ������
	RECT rect;
	SetRect(&rect, static_cast<int>(155.f), static_cast<int>(43.f), 0, 0); // �ؽ�Ʈ�� ����� ��ġ
	m_pFont->DrawText(
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

HRESULT CUI_HP_Player::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_HP_Frame"),
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

CUI_HP_Player* CUI_HP_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_HP_Player* pInstance = new CUI_HP_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_HP_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_HP_Player::Clone(void* pArg)
{
	CUI_HP_Player* pInstance = new CUI_HP_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_HP_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_HP_Player::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pFont);

	__super::Free();
}
