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


	m_forScaled = m_pTransformCom->Get_Scaled();

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
}

void CPlayer::Update(_float fTimeDelta)
{
	//SetUp_OnTerrain(m_pTransformCom, 0.f);

	m_pGameInstance->Add_Timer(TEXT("Timer_60"));

	_float		fTimeAcc = { 0.0f };
	_float		fCountTime = { 0.0f };

	while (m_PlayerState == STATE_ATTACK)
	{
		fCountTime = m_pGameInstance->Compute_TimeDelta(TEXT("Timer_60"));

		fTimeAcc += fCountTime;

		if (fTimeAcc >= 1.f)
		{
			m_PlayerState = STATE_IDLE;
			m_pTransformCom->Set_Scaled(m_forScaled);

			fTimeAcc = 0.f;
			break;
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

	/* �簢������ �ø��� ���� �ױ��ĸ� �̸� ��ġ�� ���ε��Ѵ�.  */
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
}

void CPlayer::OnCollisionExit(CCollider* other)
{
	// �浹 ���� �� �ش� ���� �̵� �������� ����
	CGameObject* otherObject = other->m_MineGameObject;

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
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_EDIT, TEXT("Prototype_Component_Texture_Player"),
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

	//�ݶ��̴� �纻�� ���鶧 Cube ���� �߰������ ��.
	if (FAILED(__super::Add_Component(LEVEL_EDIT, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//�ݶ��̴�������Ʈ �߰�
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

	return S_OK;
}

void CPlayer::Player_Attack(_float fTimeDelta)
{
	_float3		curScaled;

	// ��Ÿ
	// �÷��̾� Ʈ������ ������ �ͼ� �ٶ󺸴� �������� ���� �Ÿ� �̳��� ������Ʈ�� �ִٸ� ����?
	// �ð� �� �޾Ƽ� ���� �ð� ������ ���� �⺻���� ����


	curScaled.x = m_forScaled.x + 10.f;
	curScaled.y = m_forScaled.y + 10.f;
	curScaled.z = m_forScaled.z + 10.f;

	m_pTransformCom->Set_Scaled(curScaled);

}

void CPlayer::Player_Skill(_float fTimeDelta)
{
	//ȸ���ؾ� �� y��, z��
	// transform ������Ʈ �ȿ� �� �Է��ϸ� 90���� ȸ���ϴ� �Լ� �����
	// �ð� �� ���� 3�ܰ�

	_float fSkillLevel = { 0.f };

	CSkill_Player::SKILL_PLAYER_DESC	SkillDesc{};
	SkillDesc.pTargetTransform = m_pTransformCom;
	m_pTransformCom->AddRef();

	_float vPositionX = SkillDesc.pTargetTransform->Get_State(CTransform::STATE_POSITION).x + (2.f * fSkillLevel);
	_float vPositionY = SkillDesc.pTargetTransform->Get_State(CTransform::STATE_POSITION).y;
	_float vPositionZ = SkillDesc.pTargetTransform->Get_State(CTransform::STATE_POSITION).z + (2.f * fSkillLevel);

	_float3 vPosition = { vPositionX , vPositionY, vPositionZ };



	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Player"), TEXT("Layer_Skill_Player"), &SkillDesc);

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

	Safe_Release(m_pColliderCom);

	Safe_Release(m_pKeyCom);
}
