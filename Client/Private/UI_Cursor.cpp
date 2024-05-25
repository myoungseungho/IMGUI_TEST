#include "stdafx.h"
#include "..\Public\UI_Cursor.h"

#include "GameInstance.h"

CUI_Cursor::CUI_Cursor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Cursor::CUI_Cursor(const CUI_Cursor& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Cursor::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CUI_Cursor::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 160.f;
	m_fSizeY = 165.f;
	m_fX = -430.f;
	m_fY = 85.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));

	m_fAlpha = 255.f;

	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 1.f;
	ColliderDesc.MineGameObject = this;

	//콜라이더 사본을 만들때 Cube 정보 추가해줘야 함.
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//콜라이더오브젝트 추가
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_STATIC, this);

	return S_OK;
}

void CUI_Cursor::Priority_Update(_float fTimeDelta)
{
}

void CUI_Cursor::Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn이 false이면 업데이트를 수행하지 않음
	
}

void CUI_Cursor::Late_Update(_float fTimeDelta)
{
	if (!m_bIsOn) return; // m_bIsOn이 false이면 업데이트를 수행하지 않음
	
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Cursor::Render(_float fTimeDelta)
{
	if (!m_bIsOn) return S_OK; // m_bIsOn이 false이면 렌더링을 수행하지 않음

	__super::Begin_RenderState();

	AnimState(fTimeDelta);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

void CUI_Cursor::AnimState(_float _fTimeDelta)
{
	m_pAnimCom->Play_Animator(TEXT("AnimTexture_Cursor_Idle"), 0.5f, _fTimeDelta, true);
}


void CUI_Cursor::OnCollisionEnter(CCollider* other)
{
	m_eAnimState = ANIMATION_STATE::ANIM_UNBLOCK;
}

void CUI_Cursor::OnCollisionStay(CCollider* other, _float fTimeDelta)
{

}

void CUI_Cursor::OnCollisionExit(CCollider* other)
{
	m_eAnimState = ANIMATION_STATE::ANIM_BLOCK;
}

HRESULT CUI_Cursor::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Sprite_UI_Cursor"),
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

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Cursor::Ready_Animation()
{
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Sprite_UI_Cursor"), TEXT("AnimTexture_Cursor_Idle"));

	return S_OK;
}


CUI_Cursor* CUI_Cursor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Cursor* pInstance = new CUI_Cursor(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Cursor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Cursor::Clone(void* pArg)
{
	CUI_Cursor* pInstance = new CUI_Cursor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Cursor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Cursor::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pAnimCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
