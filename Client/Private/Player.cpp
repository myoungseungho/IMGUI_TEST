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
	if (GetKeyState(VK_UP) & 0x8000)
	{
		if (GetKeyState(VK_LEFT) & 0x8000)
			m_pTransformCom->Go_Straight_Left(fTimeDelta);

		else if (GetKeyState(VK_RIGHT) & 0x8000)
			m_pTransformCom->Go_Straight_Right(fTimeDelta);

		else
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		if (GetKeyState(VK_LEFT) & 0x8000)
			m_pTransformCom->Go_Backward_Left(fTimeDelta);

		else if (GetKeyState(VK_RIGHT) & 0x8000)
			m_pTransformCom->Go_Backward_Right(fTimeDelta);

		else
			m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
		m_pTransformCom->Go_Left(fTimeDelta);

	if (GetKeyState(VK_RIGHT) & 0x8000)
		m_pTransformCom->Go_Right(fTimeDelta);

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	int a = 10;
}

HRESULT CPlayer::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

 	if (FAILED(m_pColliderCom->Render(m_pVIBufferCube_Com)))
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
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_Cube"), reinterpret_cast<CComponent**>(&m_pVIBufferCube_Com))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom))))
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

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pVIBufferCube_Com);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
}
