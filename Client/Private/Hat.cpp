#include "stdafx.h"
#include "..\Public\Hat.h"

#include "GameInstance.h"
#include <Player.h>

CHat::CHat(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CHat::CHat(const CHat& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CHat::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CHat::Initialize(void* pArg)
{
	HAT_DESC* pDesc = static_cast<HAT_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	m_pTagetDirection = pDesc->pTargetDirection;

	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x, vTargetPos.y + 0.5f, vTargetPos.z - 0.06f));
	m_pTransformCom->Set_Scaled(_float3(0.8f, 0.5f, 1.f));


	return S_OK;
}

void CHat::Priority_Update(_float fTimeDelta)
{
}

void CHat::Update(_float fTimeDelta)
{
	_uint level = m_pGameInstance->GetCurrentLevelIndex();

	if (level != 1)
	{
		CPlayer* pCopyPlayer = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(level, TEXT("Layer_Player")));

		_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);

		m_pTagetDirection = pCopyPlayer->Get_Player_Direction();

		if (pCopyPlayer->Get_Player_CurState() == pCopyPlayer->STATE_IDLE)
		{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x, vTargetPos.y + 0.5f, vTargetPos.z - 0.06f));
		}

		else if (pCopyPlayer->Get_Player_CurState() == pCopyPlayer->STATE_WALK)
		{
			if (m_pTagetDirection == pCopyPlayer->DIR_DOWN)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x, vTargetPos.y + 0.5f, vTargetPos.z - 0.06f));
			}
			else if (m_pTagetDirection == pCopyPlayer->DIR_LEFT)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x - 0.15f, vTargetPos.y + 0.6f, vTargetPos.z - 0.06f));
			}
			else if (m_pTagetDirection == pCopyPlayer->DIR_RIGHT)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x + 0.15f, vTargetPos.y + 0.6f, vTargetPos.z - 0.06f));
			}
			else if (m_pTagetDirection == pCopyPlayer->DIR_UP)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x, vTargetPos.y + 0.5f, vTargetPos.z - 0.06f));
			}
			else if (m_pTagetDirection == pCopyPlayer->DIR_LEFTUP)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x - 0.1f, vTargetPos.y + 0.5f, vTargetPos.z - 0.06f));
			}
			else if (m_pTagetDirection == pCopyPlayer->DIR_RIGHTUP)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x + 0.1f, vTargetPos.y + 0.5f, vTargetPos.z - 0.06f));
			}
			else if (m_pTagetDirection == pCopyPlayer->DIR_LEFTDOWN)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x - 0.1f, vTargetPos.y + 0.5f, vTargetPos.z - 0.06f));
			}
			else if (m_pTagetDirection == pCopyPlayer->DIR_RIGHTDOWN)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(vTargetPos.x + 0.1f, vTargetPos.y + 0.5f, vTargetPos.z - 0.06f));
			}
		}

	}
	__super::Update(fTimeDelta);

}

void CHat::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CHat::Render(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	Begin_RenderState();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	End_RenderState();

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CHat::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CHat::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CHat::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hat_Towel"),
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

CHat* CHat::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHat* pInstance = new CHat(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CHat"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHat::Clone(void* pArg)
{
	CHat* pInstance = new CHat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CHat"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHat::Free()
{
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}

