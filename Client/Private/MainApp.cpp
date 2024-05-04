#pragma once
#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"

bool bShowImGuiWindows = true;  // IMGUI 창 표시 여부를 제어하는 전역 변수

CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	/* 싱글턴 객체의 주소는 자체 클래스 안의 멤버변수에 보관하지만 그 주소를 CMainApp클래스에 멤버변수에도 공유하였기때문 */
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	/* 내 게임을 진행하기위해 필요한 여러 초기화 작업을 수행한다. */
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hWnd, LEVEL_END, g_iWinSizeX, g_iWinSizeY, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(SetUp_DefaultState()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Components()))
		return E_FAIL;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphic_Device);

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;


	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);
}
HRESULT CMainApp::Render()
{
#pragma region IMGUI
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 토글 버튼
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Option")) {
			if (ImGui::MenuItem("IMGUI ON", NULL, &bShowImGuiWindows)) {
				// bShowImGuiWindows 값이 클릭할 때마다 토글됩니다.
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (bShowImGuiWindows) {  // 이 조건을 통해 모든 ImGui 창의 표시 여부를 제어
		bool bShowLevelWindow = true;
		ImGui::Begin("Levels", &bShowLevelWindow, ImGuiWindowFlags_NoCollapse);
		if (bShowLevelWindow) {
			ShowLevels();
		}
		ImGui::End();


		bool bShowObjectWindow = true;
		ImGui::Begin("ObjectList", &bShowObjectWindow, ImGuiWindowFlags_NoCollapse);
		if (bShowObjectWindow) {
			ShowObjects();
		}
		ImGui::End();
	}

#pragma endregion
	m_pGameInstance->Render_Begin();
	m_pGameInstance->Render_Engine();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	m_pGameInstance->Render_End();

	return S_OK;
}
HRESULT CMainApp::ShowLevels()
{
	// 가정: 레벨 이름과 ID를 가진 배열 또는 리스트
	vector<pair<string, LEVELID>> levels = {
		{"Logo", LEVEL_LOGO},
		{"Game", LEVEL_GAMEPLAY},
	};

	for (auto& level : levels) {
		if (ImGui::Button(level.first.c_str())) {
			return m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pGraphic_Device, level.second));
		}
	}

	return S_OK;
}

HRESULT CMainApp::ShowObjects()
{
	vector<string> objectPrototypes;
	m_pGameInstance->AddObjectPrototypesVector(&objectPrototypes);
	int selectedItem = -1; // 초기 선택 항목 없음

	for (int i = 0; i < objectPrototypes.size(); i++) {
		if (ImGui::Selectable(objectPrototypes[i].c_str(), selectedItem == i)) {
			selectedItem = i;
			// 오브젝트 생성 로직
			SpawnObjectAtZero(objectPrototypes[i]);
		}
	}

	return S_OK;

}


HRESULT CMainApp::SpawnObjectAtZero(const std::string& type)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"), TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

/* 내 게임에 기초가되는 디바이스 상태(렌더상태, 샘플링방식에대한 설정)를 설정한다. */
HRESULT CMainApp::SetUp_DefaultState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID eLevelID)
{
	return m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pGraphic_Device, eLevelID));
}

HRESULT CMainApp::Ready_Prototype_Components()
{
	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;

}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	__super::Free();

	Safe_Release(m_pGraphic_Device);

	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);
}
