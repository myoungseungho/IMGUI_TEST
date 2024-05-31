#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"


#include "..\Public\Player.h"
#include "..\Public\Tree.h"
#include "Bush.h"
#include "GameInstance.h"
#include "Skill_Player.h"
#include "Skill_Monster.h"
#include "Monster.h"
#include "Push_Stone.h"
#include "Skill_Koofu_Bubble.h"
#include "Skill_Bug_Bullet.h"
#include "Skill_Koofu_Rolling.h"
#include <Mon_Bear_Cannon.h>
#include <RockBreakable.h>
#include <Monkey_Statue.h>
#include <Block.h>
#include "Npc.h"
#include "TravelNpc.h"
#include "UI_Npc_Talk.h"
#include "UI_Npc_Question_Effect.h"
#include <Laser.h>
#include <Un_Laser.h>
#include <Door.h>
#include "End_Orb.h"
#include "Level_Loading.h"
#include <Small_Orb.h>
#include <Un_Small_Orb.h>
#include <Effect_Player.h>

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CGameObject{ Prototype }
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

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	LEVELID level = (LEVELID)m_pGameInstance->GetLoadingLevelIndex();

	m_ePlayerDir = DIR_DOWN;
	m_bAttack = false;

	switch (level)
	{
	case Client::LEVEL_TACHO:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(19.649f, 0.5f, 30.250f));
		m_ePlayerCurState = STATE_IDLE;
		break;
	case Client::LEVEL_JUNGLE:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(36.736f, 5.5f, 16.412f));
		m_ePlayerCurState = STATE_BALLON_DOWN;
		break;
	case Client::LEVEL_SNOW:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(54.f, 5.5f, 30.f));
		m_ePlayerCurState = STATE_BALLON_DOWN;
		m_bHaveSkill = true;
		break;
	case Client::LEVEL_KOOFU:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(49.5f, 5.5f, 17.f));
		m_ePlayerCurState = STATE_BALLON_DOWN;
		m_bHaveSkill = true;
		break;
	case Client::LEVEL_BUG:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(34.5f, 5.5f, 21.f));
		m_ePlayerCurState = STATE_BALLON_DOWN;
		m_bHaveSkill = true;
		break;
	case Client::LEVEL_EDIT:
		break;
	default:
		break;
	}

	m_forScaled = m_pTransformCom->Get_Scaled();

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
}

void CPlayer::Update(_float fTimeDelta)
{
	Key_Input(fTimeDelta);

	if (m_ePlayerCurState == STATE_ATTACK)
	{
		For_Attack_State(fTimeDelta);
	}
	else
		m_pTransformCom->Set_Scaled(m_forScaled);

	if (m_ePlayerCurState == STATE_HIT)
	{
		For_Damage_State(fTimeDelta);
	}
	if (m_ePlayerCurState == STATE_GET)
	{
		For_Get_State(fTimeDelta);
	}
	if (m_ePlayerCurState == STATE_SKILL && m_bHaveSkill)
	{
		if (m_pCal_Timercom->Time_Limit(fTimeDelta, 0.5f)) // E 키를 누른 시간 (1초마다)
		{
			m_iCurrentSkillCount += 1;
			Player_Skill();

			if (m_iCurrentSkillCount > 3)
			{
				m_ePlayerCurState = STATE_IDLE;
				m_iCurrentSkillCount = 0;
			}
		}
	}
	if (m_ePlayerCurState == STATE_BALLON_UP)
	{
		m_pTransformCom->Set_Scaled(_float3(3.f, 3.f, 1.f));

		if (!m_bIsMovingUp)
		{
			// 초기 설정
			m_fElapsedTime = 0.0f;
			m_fDuration = 5.0f; // 예를 들어 2초 동안 이동
			m_fInitialY = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
			m_fTargetY = m_fInitialY + 10.0f; // y값을 5만큼 증가
			m_bIsMovingUp = true;
		}

		// 시간 경과
		m_fElapsedTime += fTimeDelta;
		float t = m_fElapsedTime / m_fDuration;
		if (t >= 1.0f)
		{
			t = 1.0f;
			m_bIsMovingUp = false;
			m_bIsMovingComplete = true;
			m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f));
			return;
		}

		// 위치 보간
		_float3 position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		position.y = Lerp(m_fInitialY, m_fTargetY, t);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &position);
	}
	else if (m_ePlayerCurState == STATE_BALLON_DOWN)
	{
		m_pTransformCom->Set_Scaled(_float3(3.f, 3.f, 1.f));

		if (!m_bIsMovingDown)
		{
			// 초기 설정
			m_fElapsedTime = 0.0f;
			m_fDuration = 3.0f; // 예를 들어 2초 동안 이동
			m_fInitialY = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
			m_fTargetY = m_fInitialY - 4.5f; // y값을 5만큼 감소
			m_bIsMovingDown = true;
		}

		// 시간 경과
		m_fElapsedTime += fTimeDelta;
		float t = m_fElapsedTime / m_fDuration;
		if (t >= 1.0f)
		{
			t = 1.0f;
			_float3 position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			position.y -= 0.5f;
			m_bIsMovingDown = false;
			m_ePlayerCurState = STATE_IDLE; // 상태 변경
			m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &position);
			return;
		}

		// 위치 보간
		_float3 position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		position.y = Lerp(m_fInitialY, m_fTargetY, t);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &position);
	}
	else
		m_iCurrentSkillCount = 0;
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CPlayer::Render(_float fTimeDelta)
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	Player_AnimState(fTimeDelta);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (m_ePlayerCurState == STATE_BALLON_UP && m_bIsMovingComplete)
		m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pGraphic_Device, (LEVELID)(m_pGameInstance->GetCurrentLevelIndex() + 1)));

	// 시간 경과
	return S_OK;
}

void CPlayer::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CBush*>(otherObject))
		return;

	if (dynamic_cast<CUn_Laser*>(otherObject))
		return;

	if (dynamic_cast<CLaser*>(otherObject))
		return;

	if (dynamic_cast<CSkill_Monster*>(otherObject) && m_ePlayerCurState != STATE_ATTACK)
	{
		if (m_bCanDamaged && m_bForTestDamaged != false)
		{
			m_ePlayerCurState = STATE_HIT;
			Player_Damaged();


			//스킬몬스터랑 충돌하면, 플레이어가 밀려남.
			_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			CSkill_Monster* pMonSkill = dynamic_cast<CSkill_Monster*>(otherObject);
			CTransform* pMonSkillTransform = dynamic_cast<CTransform*>(pMonSkill->Get_Component(TEXT("Com_Transform")));

			_float3 vDir = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pMonSkillTransform->Get_State(CTransform::STATE_POSITION);
			vDir.y = 0.0f;

			vPosition += *D3DXVec3Normalize(&vDir, &vDir) * 0.5f;


			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPosition);

			m_bCanMoveRight = false;
			m_bCanMoveLeft = false;
			m_bCanMoveForward = false;
			m_bCanMoveBackward = false;
			m_bCanDamaged = false;

			return;
		}
	}

	if (dynamic_cast<CMonster*>(otherObject) && m_ePlayerCurState != STATE_ATTACK)
	{
		if (m_bCanDamaged && m_bForTestDamaged != false)
		{
			m_ePlayerCurState = STATE_HIT;
			Player_Damaged();

			_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			CMonster* pMonster = dynamic_cast<CMonster*>(otherObject);
			CTransform* pMonTransform = dynamic_cast<CTransform*>(pMonster->Get_Component(TEXT("Com_Transform")));

			_float3 vDir = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pMonTransform->Get_State(CTransform::STATE_POSITION);
			vDir.y = 0.0f;

			vPosition += *D3DXVec3Normalize(&vDir, &vDir) * 0.5f;


			m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPosition);

			m_bCanMoveRight = false;
			m_bCanMoveLeft = false;
			m_bCanMoveForward = false;
			m_bCanMoveBackward = false;
			m_bCanDamaged = false;

			return;
		}
	}

	if (dynamic_cast<CPush_Stone*>(otherObject))
	{
		m_bPush = true;
		return;
	}

	if (dynamic_cast<CBlock*>(otherObject))
	{
		if (m_ePlayerCurState == STATE_WALK)
		{
			CBlock* pBlock = dynamic_cast<CBlock*>(otherObject);

			if (pBlock->m_eAnimState != 3)
			{
				// Transform 컴포넌트를 가져옴
				CComponent* other_component = otherObject->Get_Component(TEXT("Com_Transform"));
				CTransform* other_transform = static_cast<CTransform*>(other_component);

				// 플레이어와 다른 객체의 위치를 가져옴
				_float3 playerPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				_float3 otherPosition = other_transform->Get_State(CTransform::STATE_POSITION);

				if (m_bMoveRight && playerPosition.x < otherPosition.x) {
					m_bCanMoveRight = false;
				}
				if (m_bMoveLeft && playerPosition.x > otherPosition.x) {
					m_bCanMoveLeft = false;
				}
				if (m_bMoveUp && playerPosition.z < otherPosition.z) {
					m_bCanMoveForward = false;
				}
				if (m_bMoveDown && playerPosition.z > otherPosition.z) {
					m_bCanMoveBackward = false;
				}
			}
		}

		return;
	}

	if (dynamic_cast<CDoor*>(otherObject))
	{
		if (m_ePlayerCurState == STATE_WALK)
		{
			if (CEnd_Orb::m_eClearState != 1)
			{
				// Transform 컴포넌트를 가져옴
				CComponent* other_component = otherObject->Get_Component(TEXT("Com_Transform"));
				CTransform* other_transform = static_cast<CTransform*>(other_component);

				// 플레이어와 다른 객체의 위치를 가져옴
				_float3 playerPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				_float3 otherPosition = other_transform->Get_State(CTransform::STATE_POSITION);

				if (m_bMoveRight && playerPosition.x < otherPosition.x) {
					m_bCanMoveRight = false;
				}
				if (m_bMoveLeft && playerPosition.x > otherPosition.x) {
					m_bCanMoveLeft = false;
				}
				if (m_bMoveUp && playerPosition.z < otherPosition.z) {
					m_bCanMoveForward = false;
				}
				if (m_bMoveDown && playerPosition.z > otherPosition.z) {
					m_bCanMoveBackward = false;
				}
			}
		}

		return;
	}

	CNpc* npc = dynamic_cast<CNpc*>(otherObject);
	CTravelNpc* travelnpc = dynamic_cast<CTravelNpc*>(otherObject);

	if (npc != nullptr)
		Interaction_NPC(npc);

	if (travelnpc != nullptr)
		Interaction_NPC(travelnpc);


	// Transform 컴포넌트를 가져옴
	CComponent* other_component = otherObject->Get_Component(TEXT("Com_Transform"));
	CTransform* other_transform = static_cast<CTransform*>(other_component);

	// 플레이어와 다른 객체의 위치를 가져옴
	_float3 playerPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 otherPosition = other_transform->Get_State(CTransform::STATE_POSITION);

	// 충돌 방향 감지 및 이동 제한
	if (m_bMoveRight && playerPosition.x < otherPosition.x) {
		m_bCanMoveRight = false;
	}
	if (m_bMoveLeft && playerPosition.x > otherPosition.x) {
		m_bCanMoveLeft = false;
	}
	if (m_bMoveUp && playerPosition.z < otherPosition.z) {
		m_bCanMoveForward = false;
	}
	if (m_bMoveDown && playerPosition.z > otherPosition.z) {
		m_bCanMoveBackward = false;
	}
}

void CPlayer::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CMon_Bear_Cannon*>(otherObject))
		return;

	if (dynamic_cast<CTree*>(otherObject))
	{
		CComponent* other_component = otherObject->Get_Component(TEXT("Com_Transform"));
		CTransform* other_transform = static_cast<CTransform*>(other_component);

		// 플레이어와 다른 객체의 위치를 가져옴
		_float3 playerPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 otherPosition = other_transform->Get_State(CTransform::STATE_POSITION);

		if (m_bMoveRight && playerPosition.x < otherPosition.x) {
			m_bCanMoveRight = false;
		}
		if (m_bMoveLeft && playerPosition.x > otherPosition.x) {
			m_bCanMoveLeft = false;
		}
		if (m_bMoveUp && playerPosition.z < otherPosition.z) {
			m_bCanMoveForward = false;
		}
		if (m_bMoveDown && playerPosition.z > otherPosition.z) {
			m_bCanMoveBackward = false;
		}
		return;
	}


	if (dynamic_cast<CMonster*>(otherObject))
	{
		CMonster* pDamagedObj = dynamic_cast<CMonster*>(otherObject);

		if (m_ePlayerCurState == STATE_ATTACK && m_bAttack)
		{
			pDamagedObj->Damaged();
			m_bAttack = false;
			return;
		}
	}

	if (dynamic_cast<CSkill_Bug_Bullet*>(otherObject))
	{
		if (m_ePlayerCurState == STATE_ATTACK && m_bAttack)
		{
			CSkill_Bug_Bullet* pPushObj = dynamic_cast<CSkill_Bug_Bullet*>(otherObject);
			CTransform* pPushObjTranform = dynamic_cast<CTransform*>(pPushObj->Get_Component(TEXT("Com_Transform")));
			pPushObjTranform->Set_Speed(-3.5f);
		}
		return;
	}

	if (dynamic_cast<CSkill_Koofu_Rolling*>(otherObject))
	{
		_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		CSkill_Koofu_Rolling* pMonSkill = dynamic_cast<CSkill_Koofu_Rolling*>(otherObject);
		CTransform* pMonSkillTransform = dynamic_cast<CTransform*>(pMonSkill->Get_Component(TEXT("Com_Transform")));

		m_pTransformCom->Set_Speed(10.f);
		m_pTransformCom->Go_Backward(fTimeDelta);

		return;
	}

	if (dynamic_cast<CPush_Stone*>(otherObject))
	{
		if (m_ePlayerCurState == STATE_PUSH)
		{
			CPush_Stone* pPushObj = dynamic_cast<CPush_Stone*>(otherObject);
			CTransform* pPushObjTranform = dynamic_cast<CTransform*>(pPushObj->Get_Component(TEXT("Com_Transform")));
			_float3 PushObjPos = pPushObjTranform->Get_State(CTransform::STATE_POSITION);

			if (((PushObjPos.z > m_pTransformCom->Get_State(CTransform::STATE_POSITION).z && m_ePlayerDir == DIR_UP ||
				PushObjPos.z <  m_pTransformCom->Get_State(CTransform::STATE_POSITION).z && m_ePlayerDir == DIR_DOWN ||
				PushObjPos.x > m_pTransformCom->Get_State(CTransform::STATE_POSITION).x && m_ePlayerDir == DIR_RIGHT ||
				PushObjPos.x < m_pTransformCom->Get_State(CTransform::STATE_POSITION).x && m_ePlayerDir == DIR_LEFT)))

			{
				pPushObj->Push_Move(fTimeDelta, m_ePlayerDir);
			}
			return;
		}
	}

	if (dynamic_cast<CRockBreakable*>(otherObject))
	{
		if (m_ePlayerCurState == STATE_WALK)
		{
			// Transform 컴포넌트를 가져옴
			CComponent* other_component = otherObject->Get_Component(TEXT("Com_Transform"));
			CTransform* other_transform = static_cast<CTransform*>(other_component);

			// 플레이어와 다른 객체의 위치를 가져옴
			_float3 playerPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 otherPosition = other_transform->Get_State(CTransform::STATE_POSITION);

			if (m_bMoveRight && playerPosition.x < otherPosition.x) {
				m_bCanMoveRight = false;
			}
			if (m_bMoveLeft && playerPosition.x > otherPosition.x) {
				m_bCanMoveLeft = false;
			}
			if (m_bMoveUp && playerPosition.z < otherPosition.z) {
				m_bCanMoveForward = false;
			}
			if (m_bMoveDown && playerPosition.z > otherPosition.z) {
				m_bCanMoveBackward = false;
			}
		}
	}

	if (dynamic_cast<CMonkey_Statue*>(otherObject))
	{
		if (m_ePlayerCurState == STATE_WALK)
		{
			// Transform 컴포넌트를 가져옴
			CComponent* other_component = otherObject->Get_Component(TEXT("Com_Transform"));
			CTransform* other_transform = static_cast<CTransform*>(other_component);

			// 플레이어와 다른 객체의 위치를 가져옴
			_float3 playerPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 otherPosition = other_transform->Get_State(CTransform::STATE_POSITION);

			if (m_bMoveRight && playerPosition.x < otherPosition.x) {
				m_bCanMoveRight = false;
			}
			if (m_bMoveLeft && playerPosition.x > otherPosition.x) {
				m_bCanMoveLeft = false;
			}
			if (m_bMoveUp && playerPosition.z < otherPosition.z) {
				m_bCanMoveForward = false;
			}
			if (m_bMoveDown && playerPosition.z > otherPosition.z) {
				m_bCanMoveBackward = false;
			}
		}
	}

	if (dynamic_cast<CBlock*>(otherObject))
	{
		if (m_ePlayerCurState == STATE_WALK)
		{
			CBlock* pBlock = dynamic_cast<CBlock*>(otherObject);

			if (pBlock->m_eAnimState != 3)
			{
				// Transform 컴포넌트를 가져옴
				CComponent* other_component = otherObject->Get_Component(TEXT("Com_Transform"));
				CTransform* other_transform = static_cast<CTransform*>(other_component);

				// 플레이어와 다른 객체의 위치를 가져옴
				_float3 playerPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				_float3 otherPosition = other_transform->Get_State(CTransform::STATE_POSITION);

				if (m_bMoveRight && playerPosition.x < otherPosition.x) {
					m_bCanMoveRight = false;
				}
				if (m_bMoveLeft && playerPosition.x > otherPosition.x) {
					m_bCanMoveLeft = false;
				}
				if (m_bMoveUp && playerPosition.z < otherPosition.z) {
					m_bCanMoveForward = false;
				}
				if (m_bMoveDown && playerPosition.z > otherPosition.z) {
					m_bCanMoveBackward = false;
				}
			}
		}
	}

	if (dynamic_cast<CDoor*>(otherObject))
	{
		if (m_ePlayerCurState == STATE_WALK)
		{
			if (CEnd_Orb::m_eClearState != 1)
			{
				// Transform 컴포넌트를 가져옴
				CComponent* other_component = otherObject->Get_Component(TEXT("Com_Transform"));
				CTransform* other_transform = static_cast<CTransform*>(other_component);

				// 플레이어와 다른 객체의 위치를 가져옴
				_float3 playerPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				_float3 otherPosition = other_transform->Get_State(CTransform::STATE_POSITION);

				if (m_bMoveRight && playerPosition.x < otherPosition.x) {
					m_bCanMoveRight = false;
				}
				if (m_bMoveLeft && playerPosition.x > otherPosition.x) {
					m_bCanMoveLeft = false;
				}
				if (m_bMoveUp && playerPosition.z < otherPosition.z) {
					m_bCanMoveForward = false;
				}
				if (m_bMoveDown && playerPosition.z > otherPosition.z) {
					m_bCanMoveBackward = false;
				}
			}
		}

		return;
	}
}

void CPlayer::OnCollisionExit(class CCollider* other)
{
	if (other->m_Died)
		return;

	// 충돌 해제 시 해당 방향 이동 가능으로 설정
	CGameObject* otherObject = other->m_MineGameObject;

	if (otherObject->m_Died)
	{
		m_bCanMoveRight = true;
		m_bCanMoveLeft = true;
		m_bCanMoveForward = true;
		m_bCanMoveBackward = true;
		return;
	}

	if (dynamic_cast<CSkill_Monster*>(otherObject))
	{
		m_bCanMoveRight = true;
		m_bCanMoveLeft = true;
		m_bCanMoveForward = true;
		m_bCanMoveBackward = true;
		return;
	}

	if (dynamic_cast<CPush_Stone*>(otherObject))
	{
		m_bPush = false;
		m_ePlayerCurState = STATE_IDLE;
	}

	//NPC랑 충돌벗어나면 해제
	CNpc* npc = dynamic_cast<CNpc*>(otherObject);
	if (npc != nullptr)
	{
		Set_Npc_Talk(false);
		m_pCurrentCollisionOk_Npc = nullptr;
		m_bIsInteractionIng = false;
	}

	CTravelNpc* travelNpc = dynamic_cast<CTravelNpc*>(otherObject);
	// Transform 컴포넌트를 가져옴
	if (travelNpc != nullptr)
	{
		Set_Npc_Talk(false);
		m_pCurrentCollisionOk_Npc = nullptr;
		m_bIsInteractionIng = false;
	}

	CComponent* other_component = otherObject->Get_Component(TEXT("Com_Transform"));
	CTransform* other_transform = static_cast<CTransform*>(other_component);


	// 플레이어와 다른 객체의 위치를 가져옴
	_float3 playerPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 otherPosition = other_transform->Get_State(CTransform::STATE_POSITION);

	if (playerPosition.x < otherPosition.x) {
		m_bCanMoveRight = true;
	}
	if (playerPosition.x > otherPosition.x) {
		m_bCanMoveLeft = true;
	}
	if (playerPosition.z < otherPosition.z) {
		m_bCanMoveForward = true;
	}
	if (playerPosition.z > otherPosition.z) {
		m_bCanMoveBackward = true;
	}
}

void CPlayer::Player_Damaged()
{
	if (m_bCanDamaged && m_bForTestDamaged != false)
	{
		--m_iPlayerHp;
	}

}

void CPlayer::Interaction_NPC(CGameObject* _npc)
{
	m_pCurrentCollisionOk_Npc = _npc;
}

void CPlayer::Set_Npc_Talk(_bool _isOn)
{
	_uint level = m_pGameInstance->GetCurrentLevelIndex();
	CGameObject* gameObjectTalk = m_pGameInstance->Get_GameObject(level, TEXT("Layer_UI_Npc_Talk"));
	CUI_Npc_Talk* npcTalk = static_cast<CUI_Npc_Talk*>(gameObjectTalk);

	//충돌 On
	if (_isOn)
	{
		CNpc* npc = dynamic_cast<CNpc*>(m_pCurrentCollisionOk_Npc);
		CTravelNpc* travel = dynamic_cast<CTravelNpc*>(m_pCurrentCollisionOk_Npc);

		if (npc)
		{
			vector<pair<wstring, wstring>> messages = {
			{TEXT("최진영"), TEXT("아............\n어렵네요.....")},
			{TEXT("최진영"), TEXT("잘 모르겠어요....... 아.... 어렵네요...")},
			{TEXT("최진영"), TEXT("네엡")} };
			npcTalk->SetNpcTalkMessages(messages);
		}
		else if (travel)
		{
			vector<pair<wstring, wstring>> messages = {
		{TEXT("변수기"), TEXT("저 잠시 담타좀 하고 오겠슴다\n (연기를 내뿜으며) 후우...")},
		{TEXT("변수기"), TEXT("내가 만든 퀴즈 맵 가볼래?")} };
			npcTalk->SetNpcTalkMessages(messages);
		}


		npcTalk->SetIsNpcTalkOn(_isOn);
		//// 이펙트 수정
		CGameObject* gameObjectEffect = m_pGameInstance->Get_GameObject(level, TEXT("Layer_Npc_Question"));
		static_cast<CUI_Npc_Question_Effect*>(gameObjectEffect)->SetIsOn(_isOn);

	}
	//충돌 Off인데
	else if (m_bIsInteractionIng && !_isOn)
	{
		if (npcTalk->m_bIsNpcTalkOn == true)
			return;

		CTravelNpc* travel = dynamic_cast<CTravelNpc*>(m_pCurrentCollisionOk_Npc);
		//// 이펙트 수정
		CGameObject* gameObjectEffect = m_pGameInstance->Get_GameObject(level, TEXT("Layer_Npc_Question"));
		static_cast<CUI_Npc_Question_Effect*>(gameObjectEffect)->SetIsOn(_isOn);

		if (travel)
		{
			m_ePlayerCurState = STATE_BALLON_UP;
		}
	}
}

void CPlayer::MoveUp(float fDuration, float fDistance)
{
}

void CPlayer::MoveDown(float fDuration)
{
}

_float CPlayer::Lerp(float start, float end, float t)
{
	return start + t * (end - start);
}

HRESULT CPlayer::Ready_Components()
{
	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Amin */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		TEXT("Com_Anim"), reinterpret_cast<CComponent**>(&m_pAnimCom))))
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


	TransformDesc.fSpeedPerSec = 3.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(35.0f, 0.5f, 15.f));

	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = (m_pTransformCom->Get_Scaled().x);
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 0.5f;
	ColliderDesc.MineGameObject = this;

	//콜라이더 사본을 만들때 Cube 정보 추가해줘야 함.
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//콜라이더오브젝트 추가
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_PLAYER, this);

	return S_OK;
}

HRESULT CPlayer::Ready_Animation()
{
	// Idle
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_Up"), TEXT("Player_Idle_Up"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_Right"), TEXT("Player_Idle_Right"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_Down"), TEXT("Player_Idle_Down"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_Left"), TEXT("Player_Idle_Left"));

	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_LeftUp"), TEXT("Player_Idle_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_RightUp"), TEXT("Player_Idle_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_RightDown"), TEXT("Player_Idle_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_LeftDown"), TEXT("Player_Idle_LeftDown"));


	// Walk
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_Up"), TEXT("Player_Walk_Up"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_Right"), TEXT("Player_Walk_Right"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_Down"), TEXT("Player_Walk_Down"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_Left"), TEXT("Player_Walk_Left"));

	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_LeftUp"), TEXT("Player_Walk_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_RightUp"), TEXT("Player_Walk_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_RightDown"), TEXT("Player_Walk_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_LeftDown"), TEXT("Player_Walk_LeftDown"));

	// Attack
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_Up"), TEXT("Player_Attack_Up"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_Right"), TEXT("Player_Attack_Right"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_Down"), TEXT("Player_Attack_Down"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_Left"), TEXT("Player_Attack_Left"));

	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_LeftUp"), TEXT("Player_Attack_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_RightUp"), TEXT("Player_Attack_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_RightDown"), TEXT("Player_Attack_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_LeftDown"), TEXT("Player_Attack_LeftDown"));

	// Skill
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_Up"), TEXT("Player_Skill_Up"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_Right"), TEXT("Player_Skill_Right"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_Down"), TEXT("Player_Skill_Down"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_Left"), TEXT("Player_Skill_Left"));

	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_LeftUp"), TEXT("Player_Skill_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_RightUp"), TEXT("Player_Skill_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_RightDown"), TEXT("Player_Skill_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_LeftDown"), TEXT("Player_Skill_LeftDown"));

	//Hit
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_Up"), TEXT("Player_Hit_Up"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_Right"), TEXT("Player_Hit_Right"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_Down"), TEXT("Player_Hit_Down"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_Left"), TEXT("Player_Hit_Left"));

	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_LeftUp"), TEXT("Player_Hit_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_RightUp"), TEXT("Player_Hit_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_RightDown"), TEXT("Player_Hit_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_LeftDown"), TEXT("Player_Hit_LeftDown"));

	// Push
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Push_Up"), TEXT("Player_Push_Up"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Push_Right"), TEXT("Player_Push_Right"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Push_Down"), TEXT("Player_Push_Down"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Push_Left"), TEXT("Player_Push_Left"));

	// Ballon
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Ballon_Up"), TEXT("Player_Ballon_Up"));
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Ballon_Down"), TEXT("Player_Ballon_Down"));

	// Get Item
	m_pAnimCom->Add_Animator(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Get_Item"), TEXT("Player_Get_Item"));
	return S_OK;
}

HRESULT CPlayer::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CPlayer::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CPlayer::Key_Input(_float fTimeDelta)
{
	m_ePlayerPreState = m_ePlayerCurState;

	m_bMoveRight = m_pKeyCom->Key_Pressing(VK_RIGHT);
	m_bMoveLeft = m_pKeyCom->Key_Pressing(VK_LEFT);
	m_bMoveUp = m_pKeyCom->Key_Pressing(VK_UP);
	m_bMoveDown = m_pKeyCom->Key_Pressing(VK_DOWN);

	if (m_bCanDamaged && !m_bAttack && m_ePlayerCurState != STATE_BALLON_UP && m_ePlayerCurState != STATE_BALLON_DOWN)
	{
		if (m_pKeyCom->Key_Pressing('E'))
		{
			if (m_bHaveSkill)
				m_ePlayerCurState = STATE_SKILL;

			if (m_pKeyCom->Key_Pressing(VK_UP))
			{
				if (m_pKeyCom->Key_Pressing(VK_LEFT))
				{
					m_ePlayerDir = DIR_LEFTUP;
					_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

					_float3 vDir = -vLook + vRight;
					D3DXVec3Normalize(&vDir, &vDir);

					m_SkillDir = -vDir;
				}

				else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
				{
					m_ePlayerDir = DIR_RIGHTUP;
					_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

					_float3 vDir = -vLook - vRight;
					D3DXVec3Normalize(&vDir, &vDir);

					m_SkillDir = -vDir;
				}

				else
				{
					m_ePlayerDir = (DIR_UP);
					_float3		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

					D3DXVec3Normalize(&vLook, &vLook);

					m_SkillDir = vLook;
				}
			}

			else if (m_pKeyCom->Key_Pressing(VK_DOWN))
			{

				if (m_pKeyCom->Key_Pressing(VK_LEFT))
				{
					m_ePlayerDir = (DIR_LEFTDOWN);
					_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

					_float3 vDir = -vLook - vRight;
					D3DXVec3Normalize(&vDir, &vDir);

					m_SkillDir = vDir;
				}


				else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
				{
					m_ePlayerDir = (DIR_RIGHTDOWN);
					_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

					_float3 vDir = -vLook + vRight;
					D3DXVec3Normalize(&vDir, &vDir);

					m_SkillDir = vDir;
				}


				else
				{
					m_ePlayerDir = (DIR_DOWN);
					_float3		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

					m_SkillDir = *D3DXVec3Normalize(&vLook, &vLook);

					m_SkillDir = -vLook;

				}


			}

			else if (m_pKeyCom->Key_Pressing(VK_LEFT))
			{
				m_ePlayerDir = (DIR_LEFT);
				_float3		vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

				m_SkillDir = *D3DXVec3Normalize(&vRight, &vRight);

				m_SkillDir = -vRight;
			}


			else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
			{
				m_ePlayerDir = (DIR_RIGHT);
				_float3		vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

				m_SkillDir = *D3DXVec3Normalize(&vRight, &vRight);

				m_SkillDir = vRight;
			}
		}

		else if (m_bMoveUp)
		{
			m_ePlayerCurState = STATE_WALK;

			if (m_bPush)
			{
				m_ePlayerCurState = STATE_PUSH;
				m_pTransformCom->Set_Speed(1.f);
			}


			if (m_bMoveLeft) {
				Set_Direction(DIR_LEFTUP);
				if (m_bCanMoveForward && m_bCanMoveLeft)
					m_pTransformCom->Go_Straight_Left(fTimeDelta);
			}
			else if (m_bMoveRight) {
				Set_Direction(DIR_RIGHTUP);
				if (m_bCanMoveForward && m_bCanMoveRight)
					m_pTransformCom->Go_Straight_Right(fTimeDelta);
			}
			else {
				Set_Direction(DIR_UP);
				if (m_bCanMoveForward)
					m_pTransformCom->Go_Straight(fTimeDelta);
			}
		}

		else if (m_bMoveDown)
		{
			m_ePlayerCurState = STATE_WALK;
			if (m_bPush)
			{
				m_ePlayerCurState = STATE_PUSH;
				m_pTransformCom->Set_Speed(1.f);
			}

			if (m_bMoveLeft) {
				Set_Direction(DIR_LEFTDOWN);
				if (m_bCanMoveBackward && m_bCanMoveLeft)
					m_pTransformCom->Go_Backward_Left(fTimeDelta);
			}
			else if (m_bMoveRight) {
				Set_Direction(DIR_RIGHTDOWN);
				if (m_bCanMoveBackward && m_bCanMoveRight)
					m_pTransformCom->Go_Backward_Right(fTimeDelta);
			}
			else {
				Set_Direction(DIR_DOWN);
				if (m_bCanMoveBackward)
					m_pTransformCom->Go_Backward(fTimeDelta);
			}
		}

		else if (m_bMoveLeft)
		{
			Set_Direction(DIR_LEFT);
			m_ePlayerCurState = STATE_WALK;

			if (m_bPush)
			{
				m_ePlayerCurState = STATE_PUSH;
				m_pTransformCom->Set_Speed(1.f);
			}

			if (m_bCanMoveLeft)
				m_pTransformCom->Go_Left(fTimeDelta);
		}

		else if (m_bMoveRight)
		{
			Set_Direction(DIR_RIGHT);
			m_ePlayerCurState = STATE_WALK;

			if (m_bPush)
			{
				m_ePlayerCurState = STATE_PUSH;
				m_pTransformCom->Set_Speed(1.f);
			}

			if (m_bCanMoveRight)
				m_pTransformCom->Go_Right(fTimeDelta);
		}
		else if (m_ePlayerCurState != STATE_ATTACK && m_ePlayerCurState != STATE_PUSH &&
			m_ePlayerCurState != STATE_HIT && m_ePlayerCurState != STATE_GET)
		{
			m_ePlayerCurState = STATE_IDLE;
		}

		if (m_pKeyCom->Key_Pressing(VK_SHIFT))
			m_pTransformCom->Set_Speed(5.f);
		else
			m_pTransformCom->Set_Speed(3.f);

		if (m_pKeyCom->Key_Down('A') && m_ePlayerCurState!= STATE_HIT)
		{
			m_bAttack = true;
			m_ePlayerCurState = (STATE_ATTACK);
			Player_Attack(fTimeDelta);
		}

		if (m_pKeyCom->Key_Down(VK_RETURN))
		{
			if (m_bForTestDamaged)
				m_bForTestDamaged = false;
			else
				m_bForTestDamaged = true;
		}
	}

	if (m_pCurrentCollisionOk_Npc != nullptr)
	{
		if (m_pKeyCom->Key_Down(VK_SPACE))
		{
			Set_Npc_Talk(true);
			m_bIsInteractionIng = true;
		}
	}

	return S_OK;
}


void CPlayer::Player_Attack(_float fTimeDelta)
{
	if (m_ePlayerCurState == STATE_ATTACK)
	{
		CEffect_Player::EFFECT_PLAYER_DESC EFFECTPLAYERDESC{};

		EFFECTPLAYERDESC.pTargetTransform = m_pTransformCom;
		EFFECTPLAYERDESC.pTargetDirection = m_ePlayerDir;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Player"), TEXT("Layer_Effect_Player"), &EFFECTPLAYERDESC);


		_float3		curScaled;

		curScaled.x = m_forScaled.x + 1.5f;
		curScaled.y = m_forScaled.y + 1.5f;
		curScaled.z = m_forScaled.z + 1.5f;

		m_pTransformCom->Set_Scaled(curScaled);
	}	
}

HRESULT CPlayer::Player_Skill()
{
	if (m_iCurrentSkillCount > 3)
		return S_OK;

	CSkill_Player::SKILL_PLAYER_DESC SkillPlayerDesc = {};

	LEVELID currentLevel = (LEVELID)m_pGameInstance->GetCurrentLevelIndex();
	SkillPlayerDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(currentLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));
	SkillPlayerDesc.m_iCurrentSkillCount = m_iCurrentSkillCount;
	SkillPlayerDesc.m_SkillDir = m_SkillDir;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Skill_Player"), TEXT("Layer_Player_Skill"), &SkillPlayerDesc)))
		return E_FAIL;

	return S_OK;
}


void CPlayer::BillBoarding()
{
	_float4x4		ViewMatrix{};

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (_float3*)&ViewMatrix.m[0][0]);
	// m_pTransformCom->Set_State(CTransform::STATE_UP, (_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, (_float3*)&ViewMatrix.m[2][0]);
}

void CPlayer::Player_AnimState(_float _fTimeDelta)
{
	switch (m_ePlayerCurState)
	{
	case STATE_IDLE:
		switch (m_ePlayerDir)
		{
		case DIR_UP:
			m_pAnimCom->Play_Animator(TEXT("Player_Idle_Up"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_RIGHT:
			m_pAnimCom->Play_Animator(TEXT("Player_Idle_Right"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_DOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Idle_Down"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_LEFT:
			m_pAnimCom->Play_Animator(TEXT("Player_Idle_Left"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_LEFTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Idle_LeftUp"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_RIGHTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Idle_RightUp"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_RIGHTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Idle_RightDown"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_LEFTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Idle_LeftDown"), 1.0f, _fTimeDelta, true);
			break;
		}
		break;
	case STATE_WALK:
		switch (m_ePlayerDir)
		{
		case DIR_UP:
			m_pAnimCom->Play_Animator(TEXT("Player_Walk_Up"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_RIGHT:
			m_pAnimCom->Play_Animator(TEXT("Player_Walk_Right"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_DOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Walk_Down"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_LEFT:
			m_pAnimCom->Play_Animator(TEXT("Player_Walk_Left"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_LEFTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Walk_LeftUp"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_RIGHTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Walk_RightUp"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_RIGHTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Walk_RightDown"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_LEFTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Walk_LeftDown"), 1.0f, _fTimeDelta, true);
			break;
		}
		break;
	case STATE_ATTACK:
		switch (m_ePlayerDir)
		{
		case DIR_UP:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_Up"), 0.5f, _fTimeDelta, false);
			break;
		case DIR_RIGHT:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_Right"), 0.5f, _fTimeDelta, false);
			break;
		case DIR_DOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_Down"), 0.5f, _fTimeDelta, false);
			break;
		case DIR_LEFT:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_Left"), 0.5f, _fTimeDelta, false);
			break;
		case DIR_LEFTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_LeftUp"), 0.5f, _fTimeDelta, false);
			break;
		case DIR_RIGHTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_RightUp"), 0.5f, _fTimeDelta, false);
			break;
		case DIR_RIGHTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_RightDown"), 0.5f, _fTimeDelta, false);
			break;
		case DIR_LEFTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_LeftDown"), 0.5f, _fTimeDelta, false);
			break;
		}
		break;
	case STATE_SKILL:
		switch (m_ePlayerDir)
		{
		case DIR_UP:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_Up"), 0.7f, _fTimeDelta, false);
			break;
		case DIR_RIGHT:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_Right"), 0.7f, _fTimeDelta, false);
			break;
		case DIR_DOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_Down"), 0.7f, _fTimeDelta, false);
			break;
		case DIR_LEFT:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_Left"), 0.7f, _fTimeDelta, false);
			break;
		case DIR_LEFTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_LeftUp"), 0.7f, _fTimeDelta, false);
			break;
		case DIR_RIGHTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_RightUp"), 0.7f, _fTimeDelta, false);
			break;
		case DIR_RIGHTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_RightDown"), 0.7f, _fTimeDelta, false);
			break;
		case DIR_LEFTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_LeftDown"), 0.7f, _fTimeDelta, false);
			break;
		}
		break;
	case STATE_PUSH:
		switch (m_ePlayerDir)
		{
		case DIR_UP:
			m_pAnimCom->Play_Animator(TEXT("Player_Push_Up"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_RIGHT:
			m_pAnimCom->Play_Animator(TEXT("Player_Push_Right"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_DOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Push_Down"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_LEFT:
			m_pAnimCom->Play_Animator(TEXT("Player_Push_Left"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_LEFTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Push_Up"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_RIGHTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Push_Up"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_RIGHTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Push_Down"), 1.0f, _fTimeDelta, true);
			break;
		case DIR_LEFTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Push_Down"), 1.0f, _fTimeDelta, true);
			break;
		}
		break;
	case STATE_HIT:
		switch (m_ePlayerDir)
		{
		case DIR_UP:
			m_pAnimCom->Play_Animator(TEXT("Player_Hit_Up"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_RIGHT:
			m_pAnimCom->Play_Animator(TEXT("Player_Hit_Right"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_DOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Hit_Down"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_LEFT:
			m_pAnimCom->Play_Animator(TEXT("Player_Hit_Left"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_LEFTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Hit_LeftUp"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_RIGHTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Hit_RightUp"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_RIGHTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Hit_LeftDown"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_LEFTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Hit_RightDown"), 1.0f, _fTimeDelta, false);
			break;
		}
		break;
	case STATE_BALLON_UP:
		m_pAnimCom->Play_Animator(TEXT("Player_Ballon_Up"), 2.0f, _fTimeDelta, false);
		break;
	case STATE_BALLON_DOWN:
		m_pAnimCom->Play_Animator(TEXT("Player_Ballon_Down"), 3.0f, _fTimeDelta, false);
		break;
	case STATE_GET:
		m_pAnimCom->Play_Animator(TEXT("Player_Get_Item"), 1.0f, _fTimeDelta, false);
		break;
	}
}

void CPlayer::For_Attack_State(_float fTimeDelta)
{
	if (m_ePlayerCurState == STATE_ATTACK)
	{
		m_fAttackTime += fTimeDelta;

		if (m_fAttackTime >= 0.5f)
		{
			m_pTransformCom->Set_Scaled(m_forScaled);
			m_ePlayerCurState = STATE_IDLE;
			m_fAttackTime = 0.0f;
			m_bAttack = false;


			LEVELID level = (LEVELID)m_pGameInstance->GetCurrentLevelIndex();

			if (level == LEVEL_JUNGLE)
			{
				CSmall_Orb* pSmallOrb = dynamic_cast<CSmall_Orb*>(m_pGameInstance->Get_GameObject(LEVEL_JUNGLE, TEXT("Layer_Small_Orb")));
				pSmallOrb->Change_bIsChangeOnceTrue();

				for (int i = 0; i < 6; i++)
				{
					CUn_Small_Orb* pUnSmallOrb = dynamic_cast<CUn_Small_Orb*>(m_pGameInstance->Get_GameObject(LEVEL_JUNGLE, TEXT("Layer_Un_Small_Orb"), i));
					pUnSmallOrb->Change_bIsChangeOnceTrue();
				}

				for (int i = 0; i < 5; i++)
				{
					CMonkey_Statue* pMonkey = dynamic_cast<CMonkey_Statue*>(m_pGameInstance->Get_GameObject(LEVEL_JUNGLE, TEXT("Layer_MonkeyStatue"), i));
					pMonkey->Change_bIsChangeOnceTrue();
				}
			}

		}

	}
	else
	{
		m_pTransformCom->Set_Scaled(m_forScaled);
	}

}

void CPlayer::For_Get_State(_float fTimeDelta)
{
	if (m_ePlayerCurState == STATE_GET)
	{
		m_fGetTime += fTimeDelta;

		if (m_fGetTime >= 2.f)
		{
			m_ePlayerCurState = STATE_IDLE;
			m_fGetTime = 0.0f;
			m_bAttack = false;
		}
	}
}

void CPlayer::For_Damage_State(_float fTimeDelta)
{
	m_fDamageTime += fTimeDelta;

	if (m_fDamageTime >= 1.5f)
	{
		m_ePlayerCurState = STATE_IDLE;
		m_fDamageTime = 0.0f;
		m_bCanDamaged = true;
		m_bCanMoveRight = true;
		m_bCanMoveLeft = true;
		m_bCanMoveForward = true;
		m_bCanMoveBackward = true;

	}
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
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
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pCal_Timercom);
	Safe_Release(m_pKeyCom);
	Safe_Release(m_pAnimCom);
	Safe_Release(m_pColliderCom);
	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
