#include "stdafx.h"
#include "..\Public\Monkey_Statue.h"

#include "GameInstance.h"
#include <Player.h>
#include "Block.h"

_uint CMonkey_Statue::m_eMonkeyState = CMonkey_Statue::STATE_UP;

CMonkey_Statue::CMonkey_Statue(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CMonkey_Statue::CMonkey_Statue(const CMonkey_Statue& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CMonkey_Statue::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CMonkey_Statue::Initialize(void* pArg)
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

	CMonkey_Statue::m_eMonkeyState = STATE_UP;
	PrePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	return S_OK;
}

void CMonkey_Statue::Priority_Update(_float fTimeDelta)
{
}

void CMonkey_Statue::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	//if (m_pTimerCom->Time_Limit(fTimeDelta, 0.01f))
	//{
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(PrePos.x, PrePos.y, PrePos.z));
	//}
}

void CMonkey_Statue::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CMonkey_Statue::Render(_float fTimeDelta)
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

void CMonkey_Statue::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CPlayer*>(otherObject))
	{
		CPlayer* pCopyPlayer = dynamic_cast<CPlayer*>(otherObject);

		if (pCopyPlayer->Get_Player_State() == 2)
			Change_State(fTimeDelta);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(PrePos.x + 0.1f, PrePos.y, PrePos.z));
	}
}

void CMonkey_Statue::OnCollisionStay(CCollider* other, _float fTimeDelta)
{

}

void CMonkey_Statue::OnCollisionExit(class CCollider* other)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(PrePos.x, PrePos.y, PrePos.z));
}

HRESULT CMonkey_Statue::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Sprite_MonkeyStatue_Trigger"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Timer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
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

void CMonkey_Statue::Change_State(_float fTimeDelta)
{
	if (CMonkey_Statue::m_eMonkeyState == STATE_UP)
		CMonkey_Statue::m_eMonkeyState = STATE_DOWN;
	else if (CMonkey_Statue::m_eMonkeyState == STATE_DOWN)
		CMonkey_Statue::m_eMonkeyState = STATE_UP;
}

CMonkey_Statue* CMonkey_Statue::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonkey_Statue* pInstance = new CMonkey_Statue(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMonkey_Statue"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CMonkey_Statue::Clone(void* pArg)
{
	CMonkey_Statue* pInstance = new CMonkey_Statue(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMonkey_Statue"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonkey_Statue::Free()
{
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
