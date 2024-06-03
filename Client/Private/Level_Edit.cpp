#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\Level_Edit.h"

#include "GameInstance.h"
#include "LandObject.h"
#include <Camera.h>
#include "TachoShop_Tile.h"
#include "Bush.h"
#include "UI_Npc_Talk.h"
#include "UI_FadeInOut.h"
CLevel_Edit::CLevel_Edit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel_UI{ pGraphic_Device }
{
}

HRESULT CLevel_Edit::Initialize()
{
	m_iLevelIndex = LEVEL_EDIT;


	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	__super::Initialize();

	//int horizontalTiles = 14; // ���÷� ���� 13 Ÿ��
	//int verticalTiles = 2; // ���÷� ���� 5 Ÿ��

	//if (FAILED(Ready_Layer_Tile(TEXT("Layer_TachoGround_Tile"), horizontalTiles, verticalTiles)))
	//	return E_FAIL;

	if (FAILED(ParseInitialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Edit::Update(_float fTimeDelta)
{
	if (m_bIsEnd)
		return;
	__super::Update(fTimeDelta);

	m_fElapsedTime += fTimeDelta; // ��� �ð� ����

	// 2�ʰ� ������, Level_Edit_Start1 �Լ��� ���� ȣ����� �ʾҴٸ� ȣ��
	if (m_fElapsedTime >= 2.0f && !m_bStart1Called)
	{
		Level_Edit_Start1();
		m_bStart1Called = true;
		m_fElapsedTime = 0; // ���� �� ��� �ð� �ʱ�ȭ
	}

	if (m_bStart1Called)
	{
		_uint level = m_pGameInstance->GetLoadingLevelIndex();

		CGameObject* gameObjectTalk = m_pGameInstance->Get_GameObject(level, TEXT("Layer_UI_Npc_Talk"));
		CUI_Npc_Talk* npcTalkUI = static_cast<CUI_Npc_Talk*>(gameObjectTalk);

		// npcTalkUI ���� �� ��� �ð� �ʱ�ȭ
		if (m_fTalkElapsedTime < 0)
		{
			m_fTalkElapsedTime = 0;
		}
		else
		{
			m_fTalkElapsedTime += fTimeDelta;
		}

		// npcTalkUI�� ��ȭ ���� �ƴϰ� 3�ʰ� �������� Ȯ��
		if (!npcTalkUI->m_bIsNpcTalkOn && m_fTalkElapsedTime > 3.0f)
		{
			// 3�ʰ� ������, ī�޶� ����ũ�� ���� ȣ����� �ʾҴٸ� ȣ��
			if (!m_bShakeCalled)
			{
				CGameObject* cameraObject = m_pGameInstance->Get_GameObject(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Camera"));
				static_cast<CCamera*>(cameraObject)->ShakeCamera(5.f, 0.5f, 0.1f);

				CGameObject* fadeInOutObject = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_UI_FadeInOut"));
				static_cast<CUI_FadeInOut*>(fadeInOutObject)->StartFading(0.3f, 0.f, 255.f, true, 5.f);

				m_bShakeCalled = true;
				m_fFadeElapsedTime = 0; // ���̵� �ƿ� ��� �ð� �ʱ�ȭ
			}
		}

		// ���̵� �ƿ� ���� �� ��� �ð� ����
		if (m_bShakeCalled && !m_bStart2Called)
		{
			m_fFadeElapsedTime += fTimeDelta;

			// ���̵� �ƿ��� ī�޶� ����ũ�� �Ϸ�ǰ� 1�ʰ� ���� �� Level_Edit_Start2 ȣ��
			if (m_fFadeElapsedTime > 6.0f) // 5�� ���̵� �ƿ� + 1�� ���
			{
				Level_Edit_Start2();
				m_bStart2Called = true; // Level_Edit_Start2�� ȣ��Ǿ����� ǥ��
				m_fPostStart2ElapsedTime = 0; // Level_Edit_Start2 ���� ��� �ð� �ʱ�ȭ
			}
		}

		// Level_Edit_Start2 ȣ�� ���� �߰� �۾�
		if (m_bStart2Called && !m_bIsWaitingForFade)
		{
			m_fPostStart2ElapsedTime += fTimeDelta;

			if (!npcTalkUI->m_bIsNpcTalkOn && m_fPostStart2ElapsedTime > 2.0f)
			{
				CGameObject* fadeInOutObject = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_UI_FadeInOut"));
				static_cast<CUI_FadeInOut*>(fadeInOutObject)->m_iTextureNum = 1;
				static_cast<CUI_FadeInOut*>(fadeInOutObject)->StartFadingSingleDirection(5.f, 0.f, 255.f);

				m_bIsWaitingForFade = true; // 10�� ��� ���� ����
				m_fWaitingElapsedTime = 0;  // ��� �ð� �ʱ�ȭ
			}
		}

		// 10�� ��� �ð� ��� �� Level_Edit_Start3 ȣ��
		if (m_bIsWaitingForFade && !m_bStart3Called)
		{
			m_fWaitingElapsedTime += fTimeDelta;

			if (m_fWaitingElapsedTime > 5.f)
			{
				Level_Edit_Start3();
				m_bStart3Called = true; // Level_Edit_Start3�� ȣ��Ǿ����� ǥ��
				m_fPostStart3ElapsedTime = 0; // Level_Edit_Start3 ���� ��� �ð� �ʱ�ȭ
			}
		}

		// Level_Edit_Start3 ȣ�� ���� �߰� �۾�
		if (m_bStart3Called)
		{
			m_fPostStart3ElapsedTime += fTimeDelta;

			if (!npcTalkUI->m_bIsNpcTalkOn && m_fPostStart3ElapsedTime > 3.0f)
			{
				// ���⿡ 3�� �Ŀ� �� �۾� �߰�
				// �۾��� �Ϸ�Ǿ����� ǥ��
				m_bStart3Called = false; // �ʿ信 ���� �߰� ������ �����ϰų�, ���� �ܰ�� �̵�
				m_fPostStart3ElapsedTime = 0;

				CGameObject* fadeInOutObject = m_pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_UI_FadeInOut"));
				static_cast<CUI_FadeInOut*>(fadeInOutObject)->m_iTextureNum = 1;
				static_cast<CUI_FadeInOut*>(fadeInOutObject)->StartFadingSingleDirection(5.f, 255.f, 0.f);

				// 5�� �� ������Ʈ ����
				m_bIsEnd = true;
				m_fEndingElapsedTime = 0; // ���� ��� �ð� �ʱ�ȭ
			}
		}

		// 5�� �� ������Ʈ ����
		if (m_bIsEnd)
		{
			m_fEndingElapsedTime += fTimeDelta;

			if (m_fEndingElapsedTime > 5.0f)
			{
				m_bIsEnd = true; // ������Ʈ ����
			}
		}
	}
}

HRESULT CLevel_Edit::Render()
{
	SetWindowText(g_hWnd, TEXT("�����÷��̷���"));

	return S_OK;
}

HRESULT CLevel_Edit::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_EDIT, TEXT("Prototype_GameObject_Terrain"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Edit::Ready_Layer_Sky(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"), strLayerTag)))
		return E_FAIL;
}

HRESULT CLevel_Edit::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::CAMERA_DESC			CameraDesc{};

	CameraDesc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_EDIT, TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_EDIT, TEXT("Prototype_GameObject_Camera"), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Edit::Ready_Layer_Tile(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles)
{
	CTachoShop_Tile::TILEDESC tileDesc{};

	float startX = 17.64f;
	float startY = 0.006f;
	float startZ = 29.510f;
	float spacing = 1.0f; // �� Ÿ�� ������ ����

	for (int z = 0; z < verticalTiles; ++z)
	{
		for (int x = 0; x < horizontalTiles; ++x)
		{
			tileDesc.startPosition = _float3(startX + x * spacing, startY, startZ + z * spacing);
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_EDIT, TEXT("Prototype_GameObject_TachoGround_Tile"), strLayerTag, &tileDesc)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CLevel_Edit::Ready_Layer_Bush(const _wstring& strLayerTag, int horizontalTiles, int verticalTiles)
{
	CBush::BUSHDESC bushDecs{};

	float startX = 23.944f;
	float startY = 0.600f;
	float startZ = 25.468f;
	float spacing = 0.8f; // �� Ÿ�� ������ ����

	for (int z = 0; z < verticalTiles; ++z)
	{
		for (int x = 0; x < horizontalTiles; ++x)
		{
			bushDecs.startPosition = _float3(startX + x * spacing, startY, startZ + z * spacing);
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_EDIT, TEXT("Prototype_GameObject_Bush"), strLayerTag, &bushDecs)))
				return E_FAIL;
		}
	}

	return S_OK;
}


HRESULT CLevel_Edit::Ready_LandObjects()
{
	/*CLandObject::LANDOBJECT_DESC	Desc{};

	Desc.m_pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"), 0));
	Desc.m_pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_JUNGLE, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0));

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"), Desc)))
		return E_FAIL;*/

	return S_OK;
}

void CLevel_Edit::Level_Edit_Start1()
{
	_uint level = m_pGameInstance->GetLoadingLevelIndex();

	CGameObject* gameObjectTalk = m_pGameInstance->Get_GameObject(level, TEXT("Layer_UI_Npc_Talk"));
	CUI_Npc_Talk* npcTalkUI = static_cast<CUI_Npc_Talk*>(gameObjectTalk);

	if (npcTalkUI)
	{
		npcTalkUI->SetIsNpcTalkOn(true);
		vector<pair<wstring, wstring>> messages = {
			{TEXT("?"), TEXT("���Ⱑ �����?")},
		};
		npcTalkUI->SetNpcTalkMessages(messages);
	}
}


void CLevel_Edit::Level_Edit_Start2()
{
	_uint level = m_pGameInstance->GetLoadingLevelIndex();

	CGameObject* gameObjectTalk = m_pGameInstance->Get_GameObject(level, TEXT("Layer_UI_Npc_Talk"));
	CUI_Npc_Talk* npcTalkUI = static_cast<CUI_Npc_Talk*>(gameObjectTalk);

	if (npcTalkUI)
	{
		npcTalkUI->SetIsNpcTalkOn(true);
		vector<pair<wstring, wstring>> messages = {
			{TEXT("?"), TEXT("��.. ���� ����..")},
		};
		npcTalkUI->SetNpcTalkMessages(messages);
	}
}

void CLevel_Edit::Level_Edit_Start3()
{
	_uint level = m_pGameInstance->GetLoadingLevelIndex();

	CGameObject* gameObjectTalk = m_pGameInstance->Get_GameObject(level, TEXT("Layer_UI_Npc_Talk"));
	CUI_Npc_Talk* npcTalkUI = static_cast<CUI_Npc_Talk*>(gameObjectTalk);

	if (npcTalkUI)
	{
		npcTalkUI->SetIsNpcTalkOn(true);
		vector<pair<wstring, wstring>> messages = {
			{TEXT("144�� �ϵ�"), TEXT("��..")},
		};
		npcTalkUI->SetNpcTalkMessages(messages);
	}
}

HRESULT CLevel_Edit::Ready_Layer_Player(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_EDIT, TEXT("Prototype_GameObject_Player"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Edit::ParseInitialize()
{
	if (FAILED(__super::ParseInitialize()))
		return E_FAIL;
}


CLevel_Edit* CLevel_Edit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Edit* pInstance = new CLevel_Edit(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Edit"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Edit::Free()
{
	__super::Free();
}
