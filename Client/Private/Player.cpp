#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"


#include "..\Public\Player.h"
#include "..\Public\Tree.h"
#include "Bush.h"
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

	m_forScaled = m_pTransformCom->Get_Scaled();

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
}

void CPlayer::Update(_float fTimeDelta)
{
	//SetUp_OnTerrain(m_pTransformCom, 3.f);

	Key_Input(fTimeDelta);

	if (m_PlayerCurState == STATE_ATTACK)
	{
		if (m_pCal_Timercom->Time_Limit(fTimeDelta, 1.f))
		{
			m_PlayerCurState = STATE_IDLE;
			m_pTransformCom->Set_Scaled(m_forScaled);
		}
	}

	if (m_PlayerCurState == STATE_SKILL)
	{
		if (m_pCal_Timercom->Time_Limit(fTimeDelta, 1.f)) // E 키를 누른 시간 (1초마다)
		{
			m_iCurrentSkillCount += 1;
			Create_Skill();

			if (m_iCurrentSkillCount > 3)
			{
				m_PlayerCurState = STATE_IDLE;
				m_iCurrentSkillCount = 0;
			}
		}
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

	if (dynamic_cast<CBush*>(otherObject))
		return;

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

void CPlayer::OnCollisionStay(CCollider* other)
{
	int a = 10;
}

void CPlayer::OnCollisionExit(CCollider* other)
{
	// 충돌 해제 시 해당 방향 이동 가능으로 설정
	CGameObject* otherObject = other->m_MineGameObject;

	if (otherObject->m_Died)
		return;

	// Transform 컴포넌트를 가져옴
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
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(35.f, 0.3f, 31.f));

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

HRESULT CPlayer::Key_Input(_float fTimeDelta) {
	m_bMoveRight = m_pKeyCom->Key_Pressing(VK_RIGHT);
	m_bMoveLeft = m_pKeyCom->Key_Pressing(VK_LEFT);
	m_bMoveUp = m_pKeyCom->Key_Pressing(VK_UP);
	m_bMoveDown = m_pKeyCom->Key_Pressing(VK_DOWN);

	if (m_bMoveUp) {
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

	if (m_bMoveDown) {
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

	if (m_bMoveLeft) {
		Set_Direction(DIR_LEFT);
		if (m_bCanMoveLeft)
			m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (m_bMoveRight) {
		Set_Direction(DIR_RIGHT);
		if (m_bCanMoveRight)
			m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (m_pKeyCom->Key_Pressing(VK_SHIFT))
		m_pTransformCom->Set_Speed(10.f);
	else
		m_pTransformCom->Set_Speed(5.f);

	if (m_pKeyCom->Key_Pressing('E'))
	{
		//m_PlayerCurState = STATE_SKILL;

		if (m_pKeyCom->Key_Pressing(VK_UP))
		{
			if (m_pKeyCom->Key_Pressing(VK_LEFT))
			{
				m_PlayerDir = DIR_LEFTUP;
				_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
				_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

				_float3 vDir = -vLook + vRight;
				D3DXVec3Normalize(&vDir, &vDir);

				m_SkillDir = -vDir;


			}

			else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
			{
				m_PlayerDir = DIR_RIGHTUP;
				_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
				_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

				_float3 vDir = -vLook - vRight;
				D3DXVec3Normalize(&vDir, &vDir);

				m_SkillDir = -vDir;
			}

			else
			{
				m_PlayerDir = (DIR_UP);
				_float3		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

				D3DXVec3Normalize(&vLook, &vLook);

				m_SkillDir = vLook;
		    }
		}

		else if (m_pKeyCom->Key_Pressing(VK_DOWN))
		{

			if (m_pKeyCom->Key_Pressing(VK_LEFT))
			{
				m_PlayerDir = (DIR_LEFTDOWN);
				_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
				_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

				_float3 vDir = -vLook - vRight;
				D3DXVec3Normalize(&vDir, &vDir);

				m_SkillDir = vDir;
			}


			else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
			{
				m_PlayerDir = (DIR_RIGHTDOWN);
				_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
				_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

				_float3 vDir = -vLook + vRight;
				D3DXVec3Normalize(&vDir, &vDir);

				m_SkillDir = vDir;
			}


			else
			{
				m_PlayerDir = (DIR_DOWN);
				_float3		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

				m_SkillDir = *D3DXVec3Normalize(&vLook, &vLook);

				m_SkillDir = -vLook;

			}

		}

		else if (m_pKeyCom->Key_Pressing(VK_LEFT))
		{
			m_PlayerDir = (DIR_LEFT);
			_float3		vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

			m_SkillDir = *D3DXVec3Normalize(&vRight, &vRight);

			m_SkillDir = -vRight;
		}


		else if (m_pKeyCom->Key_Pressing(VK_RIGHT))
		{
			m_PlayerDir = (DIR_RIGHT);
			_float3		vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

			m_SkillDir = *D3DXVec3Normalize(&vRight, &vRight);

			m_SkillDir = vRight;
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

	else if (m_pKeyCom->Key_Down('A'))
	{
		m_PlayerCurState = (STATE_ATTACK);
		Player_Attack(fTimeDelta);
	}

	return S_OK;
}

	
void CPlayer::Player_Attack(_float fTimeDelta)
{
	_float3		curScaled;

	// 평타
	// 플레이어 트랜스폼 가지고 와서 바라보는 방향으로 일정 거리 이내에 오브젝트가 있다면 삭제?
	// 시간 값 받아서 일정 시간 지나면 상태 기본으로 변경


	curScaled.x = m_forScaled.x + 10.f;
	curScaled.y = m_forScaled.y + 10.f;
	curScaled.z = m_forScaled.z + 10.f;

	m_pTransformCom->Set_Scaled(curScaled);

}

HRESULT CPlayer::Create_Skill()
{
	if (m_iCurrentSkillCount > 3)
		return S_OK;

	CSkill_Player::SKILL_PLAYER_DESC SkillPlayerDesc = {};

	SkillPlayerDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));
	SkillPlayerDesc.m_iCurrentSkillCount = m_iCurrentSkillCount;
	SkillPlayerDesc.m_SkillDir = m_SkillDir;

	if(FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Player"), TEXT("Layer_Player_Skill"), &SkillPlayerDesc)));
		return E_FAIL;

	return S_OK;
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
	__super::Free();

	Safe_Release(m_pTransformCom);

	Safe_Release(m_pVIBufferCom);

	Safe_Release(m_pTextureCom);

	Safe_Release(m_pCal_Timercom);

	Safe_Release(m_pColliderCom);

	Safe_Release(m_pKeyCom);

	//m_pGameInstance->Release_Collider(m_pColliderCom);
}
