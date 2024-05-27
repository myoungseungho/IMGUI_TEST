#include "stdafx.h"
#include "..\Public\UI_Npc_Talk_BackGround.h"

#include "GameInstance.h"

CUI_Npc_Talk_BackGround::CUI_Npc_Talk_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Npc_Talk_BackGround::CUI_Npc_Talk_BackGround(const CUI_Npc_Talk_BackGround& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Npc_Talk_BackGround::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CUI_Npc_Talk_BackGround::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 340.f;
	m_fSizeY = 265.f;
	m_fX = 450.f;
	m_fY = -140.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));


	m_fAlpha = 66.f;
	return S_OK;
}

void CUI_Npc_Talk_BackGround::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
}

void CUI_Npc_Talk_BackGround::Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
	
}

void CUI_Npc_Talk_BackGround::Late_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn�� false�̸� ������Ʈ�� �������� ����
	__super::Late_Update(fTimeDelta);

	
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Npc_Talk_BackGround::Render(_float fTimeDelta)
{
	if (!m_bIsOn) return S_OK; // m_bIsOn�� false�̸� �������� �������� ����
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

HRESULT CUI_Npc_Talk_BackGround::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Test_UITextBox_TextBox"),
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

CUI_Npc_Talk_BackGround* CUI_Npc_Talk_BackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Npc_Talk_BackGround* pInstance = new CUI_Npc_Talk_BackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Npc_Talk_BackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Npc_Talk_BackGround::Clone(void* pArg)
{
	CUI_Npc_Talk_BackGround* pInstance = new CUI_Npc_Talk_BackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Npc_Talk_BackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Npc_Talk_BackGround::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
