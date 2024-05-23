#include "stdafx.h"
#include "..\Public\TachoGround_Tile.h"

#include "GameInstance.h"

CTachoGround_Tile::CTachoGround_Tile(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CTachoGround_Tile::CTachoGround_Tile(const CTachoGround_Tile& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CTachoGround_Tile::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CTachoGround_Tile::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (m_bIsPasingObject)
	{
		FILEDATA* fileData = static_cast<FILEDATA*>(pArg);
		m_pTransformCom->Set_Scaled(_float3(fileData->scale.x, fileData->scale.y, fileData->scale.z));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(fileData->position.x, fileData->position.y, fileData->position.z));
	}
	else
	{
		CTachoGround_Tile::TILEDESC* tileDesc = static_cast<CTachoGround_Tile::TILEDESC*>(pArg);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &tileDesc->startPosition);
	}

	m_pTransformCom->Rotation(_float3(1, 0, 0), 3.14f / 2.f);

	return S_OK;
}

void CTachoGround_Tile::Priority_Update(_float fTimeDelta)
{
}

void CTachoGround_Tile::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}

void CTachoGround_Tile::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CTachoGround_Tile::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

HRESULT CTachoGround_Tile::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_TachoGround_Tile"),
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

CTachoGround_Tile* CTachoGround_Tile::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTachoGround_Tile* pInstance = new CTachoGround_Tile(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : TachoGround_Tile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CTachoGround_Tile::Clone(void* pArg)
{
	CTachoGround_Tile* pInstance = new CTachoGround_Tile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : TachoGround_Tile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTachoGround_Tile::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}
