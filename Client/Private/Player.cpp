#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"


#include "..\Public\Player.h"
#include "..\Public\Tree.h"
#include "Bush.h"
#include "GameInstance.h"
#include "Skill_Player.h"
#include "Monster.h"
#include "Boss_Bug.h"


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
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

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

	m_PlayerCurState = STATE_IDLE;
	m_PlayerDir = DIR_DOWN;

	m_forScaled = m_pTransformCom->Get_Scaled();

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
}

void CPlayer::Update(_float fTimeDelta)
{
	//BillBoarding();
	SetUp_OnTerrain(m_pTransformCom, 3.f);

	Key_Input(fTimeDelta);

	if (m_PlayerCurState == STATE_ATTACK)
	{
		if (m_pCal_Timercom->Time_Limit(fTimeDelta, 1.f))
		{
			m_PlayerCurState = STATE_IDLE;
			m_pTransformCom->Set_Scaled(m_forScaled);

		}
	}
	else if (m_PlayerCurState != STATE_ATTACK)
	{
		m_pTransformCom->Set_Scaled(m_forScaled);
	}



	if (m_PlayerCurState == STATE_SKILL)
	{
		if (m_pCal_Timercom->Time_Limit(fTimeDelta, 0.6f)) // E Ű�� ���� �ð� (1�ʸ���)
		{
			m_iCurrentSkillCount += 1;
			Player_Skill();

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
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	/* �簢������ �ø��� ���� �ױ��ĸ� �̸� ��ġ�� ���ε��Ѵ�.  */
	Player_AnimState(fTimeDelta);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CPlayer::OnCollisionEnter(CCollider* other)
{
	CGameObject* otherObject = other->m_MineGameObject;

	if (dynamic_cast<CBush*>(otherObject))
		return;


	// Transform ������Ʈ�� ������
	CComponent* other_component = otherObject->Get_Component(TEXT("Com_Transform"));
	CTransform* other_transform = static_cast<CTransform*>(other_component);

	// �÷��̾�� �ٸ� ��ü�� ��ġ�� ������
	_float3 playerPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 otherPosition = other_transform->Get_State(CTransform::STATE_POSITION);

	// �浹 ���� ���� �� �̵� ����
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
	CGameObject* otherObject = other->m_MineGameObject;
	
	if (m_PlayerCurState == STATE_ATTACK)
	{
		otherObject->Delete_Object();
		return;
	}

	if (dynamic_cast<CBoss_Bug*>(otherObject))
	{
		if (m_PlayerCurState == STATE_ATTACK)
		{
			CMonster* pDamagedObj = dynamic_cast<CMonster*>(otherObject);
			pDamagedObj->Damaged();

			if (pDamagedObj->m_Died)
			{
				pDamagedObj->Delete_Object();
			}
		}
		return;
	}
		
}

void CPlayer::OnCollisionExit(CCollider* other)
{
	if (other->m_Died)
		return;

	// �浹 ���� �� �ش� ���� �̵� �������� ����
	CGameObject* otherObject = other->m_MineGameObject;

	if (otherObject->m_Died)
	{
		m_bCanMoveRight = true;
		m_bCanMoveLeft = true;
		m_bCanMoveForward = true;
		m_bCanMoveBackward = true;
		return;
	}

	// Transform ������Ʈ�� ������

	CComponent* other_component = otherObject->Get_Component(TEXT("Com_Transform"));
	CTransform* other_transform = static_cast<CTransform*>(other_component);


	// �÷��̾�� �ٸ� ��ü�� ��ġ�� ������
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
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(35.f, 0.3f, 31.f));

	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 1.f;
	ColliderDesc.MineGameObject = this;

	//�ݶ��̴� �纻�� ���鶧 Cube ���� �߰������ ��.
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//�ݶ��̴�������Ʈ �߰�
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_PLAYER, this);

	return S_OK;
}

HRESULT CPlayer::Ready_Animation()
{
	// Idle
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_Up"), TEXT("Player_Idle_Up"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_Right"), TEXT("Player_Idle_Right"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_Down"), TEXT("Player_Idle_Down"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_Left"), TEXT("Player_Idle_Left"));

	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_LeftUp"), TEXT("Player_Idle_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_RightUp"), TEXT("Player_Idle_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_RightDown"), TEXT("Player_Idle_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_LeftDown"), TEXT("Player_Idle_LeftDown"));


	// Walk
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_Up"), TEXT("Player_Walk_Up"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_Right"), TEXT("Player_Walk_Right"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_Down"), TEXT("Player_Walk_Down"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_Left"), TEXT("Player_Walk_Left"));

	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_LeftUp"), TEXT("Player_Walk_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_RightUp"), TEXT("Player_Walk_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_RightDown"), TEXT("Player_Walk_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_LeftDown"), TEXT("Player_Walk_LeftDown"));

	// Attack
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_Up"), TEXT("Player_Attack_Up"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_Right"), TEXT("Player_Attack_Right"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_Down"), TEXT("Player_Attack_Down"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_Left"), TEXT("Player_Attack_Left"));

	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_LeftUp"), TEXT("Player_Attack_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_RightUp"), TEXT("Player_Attack_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_RightDown"), TEXT("Player_Attack_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_LeftDown"), TEXT("Player_Attack_LeftDown"));

	// Skill
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_Up"), TEXT("Player_Skill_Up"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_Right"), TEXT("Player_Skill_Right"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_Down"), TEXT("Player_Skill_Down"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_Left"), TEXT("Player_Skill_Left"));

	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_LeftUp"), TEXT("Player_Skill_LeftUp"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_RightUp"), TEXT("Player_Skill_RightUp"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_RightDown"), TEXT("Player_Skill_RightDown"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_LeftDown"), TEXT("Player_Skill_LeftDown"));

	// Push
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Push_Up"), TEXT("Player_Push_Up"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Push_Right"), TEXT("Player_Push_Right"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Push_Down"), TEXT("Player_Push_Down"));
	m_pAnimCom->Add_Animator(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Push_Left"), TEXT("Player_Push_Left"));

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

HRESULT CPlayer::Key_Input(_float fTimeDelta) {
	m_bMoveRight = m_pKeyCom->Key_Pressing(VK_RIGHT);
	m_bMoveLeft = m_pKeyCom->Key_Pressing(VK_LEFT);
	m_bMoveUp = m_pKeyCom->Key_Pressing(VK_UP);
	m_bMoveDown = m_pKeyCom->Key_Pressing(VK_DOWN);


	if (m_pKeyCom->Key_Pressing(VK_SHIFT))
		m_pTransformCom->Set_Speed(5.f);
	else
		m_pTransformCom->Set_Speed(3.f);

	if (m_pKeyCom->Key_Pressing('E'))
	{
		m_PlayerCurState = STATE_SKILL;

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

	else if (m_bMoveUp)
	{
		m_PlayerCurState = STATE_WALK;

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
		m_PlayerCurState = STATE_WALK;

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
		m_PlayerCurState = STATE_WALK;

		if (m_bCanMoveLeft)
			m_pTransformCom->Go_Left(fTimeDelta);
	}

	else if (m_bMoveRight)
	{
		Set_Direction(DIR_RIGHT);
		m_PlayerCurState = STATE_WALK;

		if (m_bCanMoveRight)
			m_pTransformCom->Go_Right(fTimeDelta);
	}

	else if (m_pKeyCom->Key_Down('A'))
	{
		m_PlayerCurState = (STATE_ATTACK);
		Player_Attack(fTimeDelta);
	}

	else if (m_pKeyCom->Key_Pressing('2'))
	{
		m_PlayerCurState = (STATE_PUSH);
		Player_Push(fTimeDelta);
	}


	//else
	//	m_PlayerCurState = (STATE_IDLE);

	return S_OK;
}


void CPlayer::Player_Attack(_float fTimeDelta)
{
	_float3		curScaled;

	// ��Ÿ
	// �÷��̾� Ʈ������ ������ �ͼ� �ٶ󺸴� �������� ���� �Ÿ� �̳��� ������Ʈ�� �ִٸ� ����?
	// �ð� �� �޾Ƽ� ���� �ð� ������ ���� �⺻���� ����


	curScaled.x = m_forScaled.x + 1.5f;
	curScaled.y = m_forScaled.y + 1.5f;
	curScaled.z = m_forScaled.z + 1.5f;

	m_pTransformCom->Set_Scaled(curScaled);

}

HRESULT CPlayer::Player_Skill()
{
	if (m_iCurrentSkillCount > 3)
		return S_OK;

	CSkill_Player::SKILL_PLAYER_DESC SkillPlayerDesc = {};

	SkillPlayerDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));
	SkillPlayerDesc.m_iCurrentSkillCount = m_iCurrentSkillCount;
	SkillPlayerDesc.m_SkillDir = m_SkillDir;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Player"), TEXT("Layer_Player_Skill"), &SkillPlayerDesc)));
	return E_FAIL;

	return S_OK;
}

void CPlayer::Player_Push(_float fTimeDelta)
{
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
	switch (m_PlayerCurState)
	{
	case STATE_IDLE:
		switch (m_PlayerDir)
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
		switch (m_PlayerDir)
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
		switch (m_PlayerDir)
		{
		case DIR_UP:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_Up"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_RIGHT:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_Right"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_DOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_Down"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_LEFT:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_Left"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_LEFTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_LeftUp"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_RIGHTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_RightUp"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_RIGHTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_RightDown"), 1.0f, _fTimeDelta, false);
			break;
		case DIR_LEFTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Attack_LeftDown"), 1.0f, _fTimeDelta, false);
			break;
		}
		break;
	case STATE_SKILL:
		switch (m_PlayerDir)
		{
		case DIR_UP:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_Up"), 1.5f, _fTimeDelta, false);
			break;
		case DIR_RIGHT:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_Right"), 1.5f, _fTimeDelta, false);
			break;
		case DIR_DOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_Down"), 1.5f, _fTimeDelta, false);
			break;
		case DIR_LEFT:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_Left"), 1.5f, _fTimeDelta, false);
			break;
		case DIR_LEFTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_LeftUp"), 1.5f, _fTimeDelta, false);
			break;
		case DIR_RIGHTUP:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_RightUp"), 1.5f, _fTimeDelta, false);
			break;
		case DIR_RIGHTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_RightDown"), 1.5f, _fTimeDelta, false);
			break;
		case DIR_LEFTDOWN:
			m_pAnimCom->Play_Animator(TEXT("Player_Skill_LeftDown"), 1.5f, _fTimeDelta, false);
			break;
		}
		break;
	case STATE_PUSH:
		switch (m_PlayerDir)
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
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pCal_Timercom);
	Safe_Release(m_pKeyCom);
	Safe_Release(m_pAnimCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

}
