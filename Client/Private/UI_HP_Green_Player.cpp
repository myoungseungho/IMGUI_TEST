#include "stdafx.h"
#include "..\Public\UI_HP_Green_Player.h"
#include "GameInstance.h"

CUI_HP_Green_Player::CUI_HP_Green_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_HP_Green_Player::CUI_HP_Green_Player(const CUI_HP_Green_Player& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_HP_Green_Player::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CUI_HP_Green_Player::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 220.f;
	m_fSizeY = 35.f;
	m_fX = -450.f;
	m_fY = 305.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));


	m_fAlpha = 255.f;

	LEVELID currentLevel = (LEVELID)m_pGameInstance->GetLoadingLevelIndex();
	CGameObject* gameobject = m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_Player"));
	m_pPlayerCopy = static_cast<CPlayer*>(gameobject);
	Safe_AddRef(m_pPlayerCopy);

	m_iPlayerMaxHp = m_pPlayerCopy->Get_Player_MaxHp();
	
	return S_OK;
}

void CUI_HP_Green_Player::Priority_Update(_float fTimeDelta)
{
}

void CUI_HP_Green_Player::Update(_float fTimeDelta)
{
	m_iPlayerCurrentHp = m_pPlayerCopy->Get_Player_Hp();
}

void CUI_HP_Green_Player::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	// ü�� ���� ���
	_float healthRatio = static_cast<_float>(m_iPlayerCurrentHp) / static_cast<_float>(m_iPlayerMaxHp);

	// ü�¹��� �ʱ� X ������ ����
	_float scaleX = 220.f;

	// ���� �������� �����ͼ� X �������� ü�� ������ ���� ����
	m_fSizeX = scaleX * healthRatio; // ü�� ������ ���� X ������ ����

	// ü�¹��� ���� ���� ������Ű�� ���� ��ġ ����
	// �ʱ� ü�¹��� ���� �� ��ġ�� �������� ���ο� ��ġ ���
	_float leftFixedX = m_fX - scaleX * 0.5f;
	_float adjustedX = leftFixedX + m_fSizeX * 0.5f;

	// ���ο� ��ġ�� ũ�� ����
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(adjustedX, m_fY, 0.f));

	// ���� ��ü �߰�
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_HP_Green_Player::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	/* �簢������ �ø��� ���� �ױ��ĸ� �̸� ��ġ�� ���ε��Ѵ�.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

HRESULT CUI_HP_Green_Player::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_HP_Green"),
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

CUI_HP_Green_Player* CUI_HP_Green_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_HP_Green_Player* pInstance = new CUI_HP_Green_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_HP_Green_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_HP_Green_Player::Clone(void* pArg)
{
	CUI_HP_Green_Player* pInstance = new CUI_HP_Green_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_HP_Green_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_HP_Green_Player::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pPlayerCopy);

	__super::Free();
}
