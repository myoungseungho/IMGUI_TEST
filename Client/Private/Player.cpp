#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"


#include "..\Public\Player.h"
#include "..\Public\Tree.h"

#include "GameInstance.h"
#include "Skill_Player.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject{ pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CLandObject{ Prototype }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	forScaled = m_pTransformCom->Get_Scaled();

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
}

void CPlayer::Update(_float fTimeDelta)
{
	SetUp_OnTerrain(m_pTransformCom, 0.5f);

	while (m_PlayerCurState == STATE_ATTACK)
	{
		fTimeAcc += fTimeDelta;

		if (fTimeAcc >= 100000.f)
		{
			m_PlayerCurState = STATE_IDLE;
			m_pTransformCom->Set_Scaled(forScaled);

			fTimeAcc = 0.f;
			break;
		}
	}

	if (m_PlayerCurState == STATE_SKILL)
	{
		fTimeAcc += fTimeDelta;

		if (fTimeAcc >= 1.0f) // E 키를 누른 시간 (1초마다)
		{
			Create_Skill();
			fTimeAcc = 0.0f;

			if (m_iCurrentSkillCount >= 3)
			{
				m_PlayerCurState = STATE_IDLE;
				m_iCurrentSkillCount = 0;
			}
		}
	}

	Key_Input(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CPlayer::Render(_float fTimeDelta)
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CPlayer::OnCollisionEnter(CCollider* other)
{
	CGameObject* otherObject = other->m_MineGameObject;
	if (dynamic_cast<CTree*>(otherObject)) {
		int a = 3;
	}
}

void CPlayer::OnCollisionStay(CCollider* other)
{
}

void CPlayer::OnCollisionExit(CCollider* other)
{

}

HRESULT CPlayer::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	/* For.Com_KeyState */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Key"),
		TEXT("Com_KeyState"), reinterpret_cast<CComponent**>(&m_pKeyCom))))
		return E_FAIL;

	/* For.Com_Calc_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Calc_Timer"), reinterpret_cast<CComponent**>(&m_pCal_Timercom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};


	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(0.0f, 2.f, 0.f));

	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 1.f;
	ColliderDesc.MineGameObject = this;

	//콜라이더 사본을 만들때 Cube 정보 추가해줘야 함.
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//콜라이더오브젝트 추가
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_PLAYER, this);

	return S_OK;
}

HRESULT CPlayer::Key_Input(_float fTimeDelta)
{
	if (m_pKeyCom->Key_Pressing(VK_SHIFT))
		m_pTransformCom->Set_Speed(10.f);
	else
		m_pTransformCom->Set_Speed(5.f);
	
	
	if (m_pKeyCom->Key_Down('A'))
	{
		m_PlayerCurState = (STATE_ATTACK);
		Player_Attack(fTimeDelta);	
	}

	if (m_pKeyCom->Key_Pressing('E'))
	{
		m_PlayerCurState = STATE_SKILL;

		if (m_pKeyCom->Key_Pressing(VK_UP))
		{
			if (m_pKeyCom->Key_Pressing(VK_LEFT))
			{
				m_PlayerDir = DIR_LEFTUP;
			}

			else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
			{
				m_PlayerDir = DIR_RIGHTUP;
			}

			else
			{
				m_PlayerDir = (DIR_UP);
		    }
		}

		if (m_pKeyCom->Key_Pressing(VK_DOWN))
		{

			if (m_pKeyCom->Key_Pressing(VK_LEFT))
			{
				m_PlayerDir = (DIR_LEFTDOWN);
			}


			else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
			{
				m_PlayerDir = (DIR_RIGHTDOWN);
			}


			else
			{
				m_PlayerDir = (DIR_DOWN);
			}

		}

		if (m_pKeyCom->Key_Pressing(VK_LEFT))
		{
			m_PlayerDir = (DIR_LEFT);
		}


		if (m_pKeyCom->Key_Pressing(VK_RIGHT))
		{
			m_PlayerDir = (DIR_RIGHT);
		}
	}

	else if (m_pKeyCom->Key_Pressing(VK_UP))
	{
		m_PlayerCurState = STATE_WALK;

		if (m_pKeyCom->Key_Pressing(VK_LEFT))
		{
			m_PlayerDir = DIR_LEFTUP;
			m_pTransformCom->Go_Straight_Left(fTimeDelta);
		}


		else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
		{
			m_PlayerDir = DIR_RIGHTUP;
			m_pTransformCom->Go_Straight_Right(fTimeDelta);
		}


		else
		{
			m_PlayerDir = (DIR_UP);
			m_pTransformCom->Go_Straight(fTimeDelta);
		}
	}

	else if (m_pKeyCom->Key_Pressing(VK_DOWN))
	{
		m_PlayerCurState = STATE_WALK;

		if (m_pKeyCom->Key_Pressing(VK_LEFT))
		{
			m_PlayerDir = (DIR_LEFTDOWN);
			m_pTransformCom->Go_Backward_Left(fTimeDelta);
		}


		else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
		{
			m_PlayerDir = (DIR_RIGHTDOWN);
			m_pTransformCom->Go_Backward_Right(fTimeDelta);
		}


		else
		{
			m_PlayerDir = (DIR_DOWN);
			m_pTransformCom->Go_Backward(fTimeDelta);
		}

	}

	else if (m_pKeyCom->Key_Pressing(VK_LEFT))
	{
		m_PlayerCurState = STATE_WALK;
		m_PlayerDir = (DIR_LEFT);
		m_pTransformCom->Go_Left(fTimeDelta);
	}


	else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
	{
		m_PlayerCurState = STATE_WALK;
		m_PlayerDir = (DIR_RIGHT);
		m_pTransformCom->Go_Right(fTimeDelta);
	}


	m_PlayerPreState = m_PlayerCurState;

	return S_OK;
}

	
void CPlayer::Player_Attack(_float fTimeDelta)
{
	_float3		curScaled;

	// 평타
	// 플레이어 트랜스폼 가지고 와서 바라보는 방향으로 일정 거리 이내에 오브젝트가 있다면 삭제?
	// 시간 값 받아서 일정 시간 지나면 상태 기본으로 변경


	curScaled.x = forScaled.x + 10.f;
	curScaled.y = forScaled.y + 10.f;
	curScaled.z = forScaled.z + 10.f;

	m_pTransformCom->Set_Scaled(curScaled);

}

HRESULT CPlayer::Create_Skill()
{
	if (m_iCurrentSkillCount >= 3)
		return S_OK;

	CSkill_Player::SKILL_PLAYER_DESC SkillPlayerDesc = {};

	SkillPlayerDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));

	if(FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Player"), TEXT("Layer_Player_Skill"), &SkillPlayerDesc)));
		return E_FAIL;

	++m_iCurrentSkillCount;

	return S_OK;
}


CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

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
	Safe_Release(m_pTextureCom);

	Safe_Release(m_pCal_Timercom);

	Safe_Release(m_pColliderCom);

	Safe_Release(m_pKeyCom);
}
