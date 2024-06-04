#include "stdafx.h"
#include "..\Public\UI_HP_Green_Enemy.h"
#include "GameInstance.h"

CUI_HP_Green_Enemy::CUI_HP_Green_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_HP_Green_Enemy::CUI_HP_Green_Enemy(const CUI_HP_Green_Enemy& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_HP_Green_Enemy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_HP_Green_Enemy::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 410.f;
	m_fSizeY = 35.f;
	m_fX = -20.f;
	m_fY = 320.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));

	m_fAlpha = 255.f;

	LEVELID currentLevel = (LEVELID)m_pGameInstance->GetLoadingLevelIndex();
	CGameObject* gameobject = nullptr;

	switch (currentLevel)
	{
	case Client::LEVEL_KOOFU:
		gameobject = m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_Boss_Koofu"));
		break;
	case Client::LEVEL_BUG:
		gameobject = m_pGameInstance->Get_GameObject(currentLevel, TEXT("Layer_Boss_Bug"));
		break;
	}
	m_pEnemyCopy = static_cast<CMonster*>(gameobject);
	Safe_AddRef(m_pEnemyCopy);
	m_iEnemyMaxHp = m_pEnemyCopy->m_tMonsterDesc.iMaxHp;

	return S_OK;
}

void CUI_HP_Green_Enemy::Priority_Update(_float fTimeDelta)
{
}

void CUI_HP_Green_Enemy::Update(_float fTimeDelta)
{
	m_iEnemyCurrentHp = m_pEnemyCopy->m_tMonsterDesc.iCurrentHp;

	/*if (GetAsyncKeyState('F') & 0x8000) {
		offsetX -= 5.f;
	}
	if (GetAsyncKeyState('H') & 0x8000) {
		offsetX += 5.f;
	}
	if (GetAsyncKeyState('T') & 0x8000) {
		offsetY += 5.f;
	}
	if (GetAsyncKeyState('G') & 0x8000) {
		offsetY -= 5.f;
	}
	if (GetAsyncKeyState('R') & 0x8000) {
		offsetZ -= 0.01f;
	}
	if (GetAsyncKeyState('Y') & 0x8000) {
		offsetZ += 0.01f;
	}
	if (GetAsyncKeyState('J') & 0x8000) {
		offsetXScale -= 5.f;
	}
	if (GetAsyncKeyState('K') & 0x8000) {
		offsetXScale += 5.f;
	}
	if (GetAsyncKeyState('N') & 0x8000) {
		offsetYScale -= 5.f;
	}
	if (GetAsyncKeyState('M') & 0x8000) {
		offsetYScale += 5.f;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		m_fAlpha += 1.f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		m_fAlpha -= 1.f;
	}*/

}

void CUI_HP_Green_Enemy::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	_float healthRatio = static_cast<_float>(m_iEnemyCurrentHp) / static_cast<_float>(m_iEnemyMaxHp);

	_float scaleX = 425.f;

	m_fSizeX = scaleX * healthRatio;

	if (m_fSizeX < 1.0f) m_fSizeX = 1.0f;

	_float leftFixedX = m_fX - scaleX * 0.5f;
	_float adjustedX = leftFixedX + m_fSizeX * 0.5f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(adjustedX, m_fY, 0.f));

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX + offsetX, m_fY + offsetY, 0.f));

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_HP_Green_Enemy::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

HRESULT CUI_HP_Green_Enemy::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_HP_GuageBar"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CUI_HP_Green_Enemy* CUI_HP_Green_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_HP_Green_Enemy* pInstance = new CUI_HP_Green_Enemy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_HP_Green_Enemy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_HP_Green_Enemy::Clone(void* pArg)
{
	CUI_HP_Green_Enemy* pInstance = new CUI_HP_Green_Enemy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_HP_Green_Enemy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_HP_Green_Enemy::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pEnemyCopy);

	__super::Free();
}
