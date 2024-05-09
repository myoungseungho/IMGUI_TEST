#pragma once
#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Calc_Timer.h"
#include "KeyState.h"

#include <codecvt>

bool bShowImGuiWindows = false;  // IMGUI 창 표시 여부를 제어하는 전역 변수

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

		ImGui::Begin("Main Tab", &bShowImGuiWindows); // 메인 창 시작
		if (ImGui::BeginTabBar("MyTabBar")) { // 탭 바 시작
			if (ImGui::BeginTabItem("Levels")) { // "Levels" 탭 아이템 시작
				//레벨 창
				bool bShowLevelWindow = true;
				if (bShowLevelWindow) {
					ShowLevels();
				}
				ImGui::EndTabItem(); // "Levels" 탭 아이템 종료
			}


			if (ImGui::BeginTabItem("Prototype_ObjectList")) { // "ObjectList" 탭 아이템 시작
				//원형 오브젝트 창
				bool bShowObjectWindow = true;
				if (bShowObjectWindow) {
					Show_PrototypeObjects();
				}
				ImGui::EndTabItem(); // "ObjectList" 탭 아이템 종료
			}

			if (ImGui::BeginTabItem("Layer_ObjectList")) { // "ObjectList" 탭 아이템 시작
				//원형 오브젝트 창
				bool bShowObjectWindow = true;
				if (bShowObjectWindow) {
					Show_LayerObjects();
				}
				ImGui::EndTabItem(); // "ObjectList" 탭 아이템 종료
			}

			ImGui::EndTabBar(); // 탭 바 종료
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

HRESULT CMainApp::Show_PrototypeObjects()
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

HRESULT CMainApp::Show_LayerObjects()
{
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();
	ImGui::Columns(2, NULL, true); // 두 개의 컬럼 생성

	static int selectedItem = -1; // 초기 선택 항목 없음
	static int selectedObject = -1;
	static CGameObject* pSelectedGameObject = nullptr; // 선택된 객체의 포인터

	vector<pair < string, list<CGameObject*>>> objectLayersVector;
	m_pGameInstance->AddObjectLayersVector(currentLevel, &objectLayersVector);

	ImGui::BeginChild("LeftPane", ImVec2(150, 0), true); // 왼쪽 창 생성
	for (int i = 0; i < objectLayersVector.size(); i++) {
		if (ImGui::Selectable(objectLayersVector[i].first.c_str(), selectedItem == i)) {
			selectedItem = i;
			selectedObject = -1; // 새 레이어를 선택하면 오른쪽 창의 선택 초기화
			pSelectedGameObject = nullptr;
		}
	}

	ImGui::EndChild();
	ImGui::NextColumn(); // 다음 컬럼으로 이동

	ImGui::BeginChild("RightPane", ImVec2(0, 0), true); // 오른쪽 창 생성
	if (selectedItem != -1) { // 유효한 레이어가 선택된 경우
		const string& layerName = objectLayersVector[selectedItem].first;
		const list<CGameObject*>& gameObjects = objectLayersVector[selectedItem].second;
		int index = 0;  // 게임 오브젝트의 번호를 표시하기 위한 인덱스
		for (CGameObject* gameObject : gameObjects) {
			// 각 게임 오브젝트를 클릭 가능하게 만듬
			if (ImGui::Selectable((layerName + " " + std::to_string(index)).c_str(), selectedObject == index)) {
				selectedObject = index; // 선택된 객체 인덱스 업데이트
				pSelectedGameObject = gameObject; // 선택된 객체 저장
			}
			index++;  // 다음 오브젝트에 대해 인덱스 증가
		}
	}

	if (pSelectedGameObject)
	{
		// 여기서 추가 동작을 수행할 수 있음 (예: 상세 정보 표시)
		CComponent* component = pSelectedGameObject->Get_Component(TEXT("Com_Transform"));
		if (component != nullptr)
		{
			component->AddRef();
			CTransform* transform = static_cast<CTransform*>(component);
			_float3 position = transform->Get_State(CTransform::STATE_POSITION);
			
			// 위치 정보 표시 및 슬라이더 조작
			ImGui::Text("Position: ");
			ImGui::SliderFloat("X", &position.x, -100.0f, 100.0f);
			ImGui::SliderFloat("Y", &position.y, -100.0f, 100.0f);
			ImGui::SliderFloat("Z", &position.z, -100.0f, 100.0f);

			transform->Set_State(CTransform::STATE_POSITION, &position);


			// 스케일 조절 슬라이더
			_float3 scale = transform->Get_Scaled();
			ImGui::Text("Scale: ");
			ImGui::SliderFloat("Scale X", &scale.x, 0.1f, 10.0f);
			ImGui::SliderFloat("Scale Y", &scale.y, 0.1f, 10.0f);
			ImGui::SliderFloat("Scale Z", &scale.z, 0.1f, 10.0f);
			transform->Set_Scaled(scale); // 스케일 적용

			//회전 적용
			static _float3 lastEulerRotation = { 0.0f, 0.0f, 0.0f };
			_float3 eulerRotation = lastEulerRotation;

			ImGui::Text("Rotation: ");
			bool changedX = ImGui::SliderFloat("Rotate X", &eulerRotation.x, -3.14159f, 3.14159f);
			bool changedY = ImGui::SliderFloat("Rotate Y", &eulerRotation.y, -3.14159f, 3.14159f);
			bool changedZ = ImGui::SliderFloat("Rotate Z", &eulerRotation.z, -3.14159f, 3.14159f);

			if (changedX) {
				_float3 deltaRotation = { _float3(1.0f, 0.0f, 0.0f) * (eulerRotation.x - lastEulerRotation.x) };
				transform->Turn(_float3(1.0f, 0.0f, 0.0f), deltaRotation.x);
			}
			if (changedY) {
				_float3 deltaRotation = { _float3(0.0f, 1.0f, 0.0f) * (eulerRotation.y - lastEulerRotation.y) };
				transform->Turn(_float3(0.0f, 1.0f, 0.0f), deltaRotation.y);
			}
			if (changedZ) {
				_float3 deltaRotation = { _float3(0.0f, 0.0f, 1.0f) * (eulerRotation.z - lastEulerRotation.z) };
				transform->Turn(_float3(0.0f, 0.0f, 1.0f), deltaRotation.z);
			}

			lastEulerRotation = eulerRotation;

			Safe_Release(component);
		}
	}
	ImGui::EndChild();

	ImGui::Columns(1); // 열 병합

	for (auto& iter : objectLayersVector)
	{
		for (auto& iter2 : iter.second)
			Safe_Release(iter2);
	}

	return S_OK;
}


HRESULT CMainApp::SpawnObjectAtZero(const std::string& type)
{
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	// 문자열에서 "Prototype_GameObject_" 접두어 제거
	string prefix = "Prototype_GameObject_";
	string suffix;

	if (type.substr(0, prefix.size()) == prefix) {
		suffix = type.substr(prefix.size());
	}
	else {
		suffix = type; // 접두어가 없다면 전체 타입을 사용
	}

	// Layer 이름 생성
	string layerName = "Layer_" + suffix;

	wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	wstring converted = converter.from_bytes(type);
	wstring wLayerName = converter.from_bytes(layerName);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, converted.c_str(), wLayerName)))
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

	/* For.Prototype_Component_Timer*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		CCalc_Timer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Key*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Key"),
		CKeyState::Create(m_pGraphic_Device))))
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
