#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"


#include "..\Public\Player.h"

#include "GameInstance.h"
#include <Skill_Bug_Bullet.h>

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer & Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	// m_pTransformCom->Set_Scaled(_float3(0.5f, 0.5f, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(0.0f, 3.f, 0.f));


	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CPlayer::Update(_float fTimeDelta)
{
	Key_Input(fTimeDelta);

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	int a = 10;
}

HRESULT CPlayer::Render()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferRectCom->Render()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCubeCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}
HRESULT CPlayer::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferRectCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer_Cube"), reinterpret_cast<CComponent**>(&m_pVIBufferCubeCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};


	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CPlayer::Key_Input(_float fTimeDelta)
{
	if (m_pKeyCom->Key_Pressing(VK_UP))
	{
		if (m_pKeyCom->Key_Pressing(VK_LEFT))
			m_pTransformCom->Go_Straight_Left(fTimeDelta);

		else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
			m_pTransformCom->Go_Straight_Right(fTimeDelta);

		else
			m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (m_pKeyCom->Key_Pressing(VK_DOWN))
	{
		if (m_pKeyCom->Key_Pressing(VK_LEFT))
			m_pTransformCom->Go_Backward_Left(fTimeDelta);

		else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
			m_pTransformCom->Go_Backward_Right(fTimeDelta);

		else
			m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (m_pKeyCom->Key_Pressing(VK_LEFT))
		m_pTransformCom->Go_Left(fTimeDelta);

	if (m_pKeyCom->Key_Pressing(VK_RIGHT))
		m_pTransformCom->Go_Right(fTimeDelta);

	if (m_pKeyCom->Key_Pressing(VK_SHIFT))
		m_pTransformCom->Set_Speed(10.f);
	else
		m_pTransformCom->Set_Speed(5.f);

	return S_OK;
}


CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*		pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);

	Safe_Release(m_pVIBufferRectCom);

	Safe_Release(m_pVIBufferCubeCom);

	Safe_Release(m_pTextureCom);

	Safe_Release(m_pKeyCom);
}
