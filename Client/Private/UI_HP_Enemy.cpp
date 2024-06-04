#include "stdafx.h"
#include "..\Public\UI_HP_Enemy.h"
#include "GameInstance.h"
#include "Monster.h"

CUI_HP_Enemy::CUI_HP_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_HP_Enemy::CUI_HP_Enemy(const CUI_HP_Enemy& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_HP_Enemy::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_HP_Enemy::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_fSizeX = 435.f;
    m_fSizeY = 35.f;
    m_fX = -25.f;
    m_fY = 320.f;

    m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));

    m_fAlpha = 255.f;

    D3DXFONT_DESCW tFontInfo;
    ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

    tFontInfo.Height = 25;
    tFontInfo.Width = 15;
    tFontInfo.Weight = FW_HEAVY;
    tFontInfo.CharSet = HANGEUL_CHARSET;

    wcscpy_s(tFontInfo.FaceName, LF_FACESIZE, TEXT("Cafe24 Ssurround air OTF Light"));

    if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pFont)))
    {
        MSG_BOX(L"CreateFontIndirect Failed");
        return E_FAIL;
    }

    CGameObject* gameobject = m_pGameInstance->Get_GameObject(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Boss_Koofu"));
    m_pEnemyCopy = static_cast<CMonster*>(gameobject);
    Safe_AddRef(m_pEnemyCopy);
    m_iEnemyMaxHp = m_pEnemyCopy->m_tMonsterDesc.iMaxHp;

    return S_OK;
}

void CUI_HP_Enemy::Priority_Update(_float fTimeDelta)
{
}

void CUI_HP_Enemy::Update(_float fTimeDelta)
{
    m_iEnemyCurrentHp = m_pEnemyCopy->m_tMonsterDesc.iCurrentHp;

  
}

void CUI_HP_Enemy::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_HP_Enemy::Render(_float fTimeDelta)
{
    __super::Begin_RenderState();

    if (FAILED(m_pTextureCom->Bind_Texture(0)))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    wchar_t text[256];
    swprintf_s(text, L"%d / %d", m_iEnemyCurrentHp, m_iEnemyMaxHp);

    RECT rect;
    SetRect(&rect, static_cast<int>(155.f), static_cast<int>(43.f), 0, 0);
    m_pFont->DrawText(
        NULL,
        text,
        -1,
        &rect,
        DT_NOCLIP,
        D3DCOLOR_ARGB(255, 255, 255, 255)
    );

    __super::End_RenderState();

    return S_OK;
}

HRESULT CUI_HP_Enemy::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_HP_Frame"),
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

CUI_HP_Enemy* CUI_HP_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUI_HP_Enemy* pInstance = new CUI_HP_Enemy(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CUI_HP_Enemy"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_HP_Enemy::Clone(void* pArg)
{
    CUI_HP_Enemy* pInstance = new CUI_HP_Enemy(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CUI_HP_Enemy"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_HP_Enemy::Free()
{
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pFont);
    Safe_Release(m_pEnemyCopy);

    __super::Free();
}