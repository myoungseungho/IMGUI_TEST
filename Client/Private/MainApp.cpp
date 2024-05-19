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

#include <codecvt>

bool bShowImGuiWindows = true;  // IMGUI 창 표시 여부를 제어하는 전역 변수

CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	/* 싱글턴 객체의 주소는 자체 클래스 안의 멤버변수에 보관하지만 그 주소를 CMainApp클래스에 멤버변수에도 공유하였기때문 */
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	srand((_uint)time(NULL));

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

	if (FAILED(Open_Level(LEVEL_EDIT)))
		return E_FAIL;


	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);
}
HRESULT CMainApp::Render(_float fTimeDelta)
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


		bool bShowSettings = true;


		static bool bShowSaveSuccessMessage = false;
		static bool bShowSaveFailMessage = false;

		// 새로운 창 추가
		ImGui::Begin("Settings", &bShowSettings); // "Settings" 창 시작
		if (ImGui::Button("Save")) { // "Save" 버튼
			Save_Button_Pressed(&bShowSaveSuccessMessage, &bShowSaveFailMessage);
		}

		if (bShowSaveSuccessMessage) {
			ImGui::Text("File saved successfully!");
		}

		if (bShowSaveFailMessage) {
			ImGui::Text("Failed to save file!");
		}

		if (ImGui::Button("Load")) { // "Load" 버튼
			//LoadSettings();
		}

		ImGui::End(); // "Settings" 창 종료


		// 새로운 Collider 창 추가
		static bool bColliderToggle = false;
		ImGui::Begin("Collider", &bShowSettings);
		if (ImGui::Checkbox("Toggle Collider", &bColliderToggle)) {
			Click_Collider_Toggle(bColliderToggle);
		}

		// CollisionCheckInterval 변수 선언
		static float CollisionCheckInterval = 0.f;
		// CollisionCheckInterval 입력란 추가
		ImGui::Text("CollisionInterval:");
		ImGui::SameLine(); // 같은 라인에 배치
		ImGui::PushItemWidth(50); // 입력 필드 너비 조정
		if (ImGui::InputFloat("##CollisionCheckInterval", &CollisionCheckInterval)) {
			// 사용자가 숫자를 변경했을 때 호출될 함수
			OnCollisionCheckIntervalChanged(CollisionCheckInterval);
		}
		ImGui::PopItemWidth();

		// WASDQE 활성화 토글 추가
		ImGui::Checkbox("Enable WASDQE Movement", &bMovementEnabled);

		ImGui::End();

	}
#pragma endregion
	m_pGameInstance->Render_Begin();
	m_pGameInstance->Render_Engine(fTimeDelta);
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
		{"Edit", LEVEL_EDIT},
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
	static vector<int> selectedObjects; // 선택된 객체들의 인덱스 벡터
	static vector<CGameObject*> selectedGameObjects; // 선택된 객체들의 포인터 벡터

	vector<pair<string, list<CGameObject*>>> objectLayersVector;
	m_pGameInstance->AddObjectLayersVector(currentLevel, &objectLayersVector);

	ImGui::BeginChild("LeftPane", ImVec2(150, 0), true); // 왼쪽 창 생성
	for (int i = 0; i < objectLayersVector.size(); i++) {
		if (ImGui::Selectable(objectLayersVector[i].first.c_str(), selectedItem == i)) {
			selectedItem = i;
			selectedObjects.clear(); // 새 레이어를 선택하면 오른쪽 창의 선택 초기화
			selectedGameObjects.clear(); // 선택된 객체 초기화
		}
	}

	ImGui::EndChild();
	ImGui::NextColumn(); // 다음 컬럼으로 이동

	ImGui::BeginChild("RightPane", ImVec2(0, 0), true); // 오른쪽 창 생성
	if (selectedItem != -1) { // 유효한 레이어가 선택된 경우
		const string& layerName = objectLayersVector[selectedItem].first;
		list<CGameObject*>& gameObjects = objectLayersVector[selectedItem].second;
		int index = 0;  // 게임 오브젝트의 번호를 표시하기 위한 인덱스
		for (auto it = gameObjects.begin(); it != gameObjects.end(); /* 빈 상태 */) {
			CGameObject* gameObject = *it;
			bool isSelected = std::find(selectedObjects.begin(), selectedObjects.end(), index) != selectedObjects.end();

			// 게임 오브젝트가 클릭되었는지 확인하고, 클릭된 경우 IMGUI의 선택 상태를 업데이트합니다.
			if (gameObject->GetIsPicking()) {
				// 기존 선택 상태를 초기화
				selectedObjects.clear();
				selectedGameObjects.clear();

				// 새로운 선택 상태를 설정
				selectedObjects.push_back(index);
				selectedGameObjects.push_back(gameObject);
				gameObject->SetPicking(false); // 클릭 상태 초기화
			}


			if (ImGui::Selectable((layerName + " " + std::to_string(index)).c_str(), isSelected)) {
				if (ImGui::GetIO().KeyCtrl) { // Ctrl 키가 눌려있는 상태에서만 다중 선택
					if (isSelected) {
						selectedObjects.erase(std::remove(selectedObjects.begin(), selectedObjects.end(), index), selectedObjects.end());
						selectedGameObjects.erase(std::remove(selectedGameObjects.begin(), selectedGameObjects.end(), gameObject), selectedGameObjects.end());
					}
					else {
						selectedObjects.push_back(index);
						selectedGameObjects.push_back(gameObject);
					}
				}
				else {
					selectedObjects.clear();
					selectedGameObjects.clear();
					selectedObjects.push_back(index);
					selectedGameObjects.push_back(gameObject);
				}
			}

			++it; // 삭제하지 않으면 다음 오브젝트로 이동
			index++;  // 다음 오브젝트에 대해 인덱스 증가
		}
	}

	if (!selectedGameObjects.empty())
	{
		// 키 입력을 처리하여 포지션 이동
		_float3 deltaPosition = { 0.0f, 0.0f, 0.0f };
		if (bMovementEnabled) {
			_float offset = 0.1f;
			if (ImGui::IsKeyDown(ImGuiKey_W)) {
				deltaPosition.z += offset;
			}
			if (ImGui::IsKeyDown(ImGuiKey_S)) {
				deltaPosition.z -= offset;
			}
			if (ImGui::IsKeyDown(ImGuiKey_A)) {
				deltaPosition.x -= offset;
			}
			if (ImGui::IsKeyDown(ImGuiKey_D)) {
				deltaPosition.x += offset;
			}
			if (ImGui::IsKeyDown(ImGuiKey_Q)) {
				deltaPosition.y -= offset;
			}
			if (ImGui::IsKeyDown(ImGuiKey_E)) {
				deltaPosition.y += offset;
			}

			// 오브젝트 포지션 업데이트
			for (CGameObject* gameObject : selectedGameObjects) {
				CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
				if (component != nullptr) {
					CTransform* transform = static_cast<CTransform*>(component);
					_float3 currentPosition = transform->Get_State(CTransform::STATE_POSITION);
					currentPosition += deltaPosition;
					transform->Set_State(CTransform::STATE_POSITION, &currentPosition);
				}
			}
		}

		// 여기서 추가 동작을 수행할 수 있음 (예: 상세 정보 표시)
		_float3 averagePosition = { 0.0f, 0.0f, 0.0f };
		for (CGameObject* gameObject : selectedGameObjects) {
			CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
			if (component != nullptr) {
				CTransform* transform = static_cast<CTransform*>(component);
				averagePosition += transform->Get_State(CTransform::STATE_POSITION);
			}
		}
		averagePosition /= static_cast<float>(selectedGameObjects.size());

		// ImGui 스타일 및 너비 설정
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4)); // 요소 간 간격 조정

		float sliderWidth = 150.0f; // 슬라이더의 너비
		float inputWidth = 50.0f;   // 입력 필드의 너비
		float sliderPower = 0.01f;  // 슬라이더 조정 민감도

		// 슬라이더 범위 설정
		float rangeX = 50.0f;
		float rangeY = 50.0f;
		float rangeZ = 50.0f;

		ImGui::Text("Position: ");
		ImGui::PushButtonRepeat(true); // 버튼 반복을 활성화

		// X 축 조정
		ImGui::PushItemWidth(sliderWidth);
		if (ImGui::Button("-X")) averagePosition.x -= 0.1f;
		ImGui::SameLine();
		ImGui::SliderFloat("##PosX", &averagePosition.x, 0, rangeX, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+X")) averagePosition.x += 0.1f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##PosXInput", &averagePosition.x);
		ImGui::PopItemWidth();
		ImGui::PopItemWidth();

		// Y 축 조정
		ImGui::PushItemWidth(sliderWidth);
		if (ImGui::Button("-Y")) averagePosition.y -= 0.1f;
		ImGui::SameLine();
		ImGui::SliderFloat("##PosY", &averagePosition.y, 0, rangeY, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+Y")) averagePosition.y += 0.1f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##PosYInput", &averagePosition.y);
		ImGui::PopItemWidth();
		ImGui::PopItemWidth();

		// Z 축 조정
		ImGui::PushItemWidth(sliderWidth);
		if (ImGui::Button("-Z")) averagePosition.z -= 0.1f;
		ImGui::SameLine();
		ImGui::SliderFloat("##PosZ", &averagePosition.z, 0, rangeZ, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+Z")) averagePosition.z += 0.1f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##PosZInput", &averagePosition.z);
		ImGui::PopItemWidth();
		ImGui::PopItemWidth();

		ImGui::PopButtonRepeat(); // 버튼 반복을 비활성화

		for (CGameObject* gameObject : selectedGameObjects) {
			CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
			if (component != nullptr) {
				CTransform* transform = static_cast<CTransform*>(component);
				transform->Set_State(CTransform::STATE_POSITION, &averagePosition);
			}
		}

		// 스케일 조절 슬라이더
		_float3 averageScale = { 0.0f, 0.0f, 0.0f };
		for (CGameObject* gameObject : selectedGameObjects) {
			CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
			if (component != nullptr) {
				CTransform* transform = static_cast<CTransform*>(component);
				averageScale += transform->Get_Scaled();
			}
		}
		averageScale /= static_cast<float>(selectedGameObjects.size());

		ImGui::Text("Scale: ");
		ImGui::PushButtonRepeat(true); // 버튼 반복을 다시 활성화
		ImGui::PushItemWidth(sliderWidth);

		// 스케일 X 축 조정
		if (ImGui::Button("-SX")) averageScale.x -= 0.01f;
		ImGui::SameLine();
		ImGui::SliderFloat("##ScaleX", &averageScale.x, 0.1f, 10.0f, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+SX")) averageScale.x += 0.01f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##ScaleXInput", &averageScale.x);
		ImGui::PopItemWidth();

		// 스케일 Y 축 조정
		if (ImGui::Button("-SY")) averageScale.y -= 0.01f;
		ImGui::SameLine();
		ImGui::SliderFloat("##ScaleY", &averageScale.y, 0.1f, 10.0f, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+SY")) averageScale.y += 0.01f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##ScaleYInput", &averageScale.y);
		ImGui::PopItemWidth();

		// 스케일 Z 축 조정
		if (ImGui::Button("-SZ")) averageScale.z -= 0.01f;
		ImGui::SameLine();
		ImGui::SliderFloat("##ScaleZ", &averageScale.z, 0.1f, 10.0f, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+SZ")) averageScale.z += 0.01f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##ScaleZInput", &averageScale.z);
		ImGui::PopItemWidth();

		ImGui::PopItemWidth();
		ImGui::PopButtonRepeat(); // 버튼 반복을 다시 비활성화
		ImGui::PopStyleVar(); // 스타일 변수 복원

		for (CGameObject* gameObject : selectedGameObjects) {
			CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
			if (component != nullptr) {
				CTransform* transform = static_cast<CTransform*>(component);
				transform->Set_Scaled(averageScale); // 스케일 적용
			}
		}

		// 삭제 버튼 추가
		ImGui::Separator(); // 구분선 추가
		if (ImGui::Button("Delete Selected Object")) {
			Click_Button_Release(selectedGameObjects);
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

HRESULT CMainApp::Save_Button_Pressed(bool* bShowSaveSuccessMessage, bool* bShowSaveFailMessage)
{
	// 오브젝트마다의 정보
	vector<FILEDATA> vecFileData;

	// 레벨 정보 
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	vector<pair<wstring, list<CGameObject*>>> objectLayersVector;
	m_pGameInstance->AddObjectLayersVector(currentLevel, &objectLayersVector);

	// 여기에 스킵할 레이어 이름을 정의
	unordered_set<wstring> skipLayers =
	{ L"Layer_BackGround", L"Layer_Camera", L"Layer_Player" };

	for (auto& object : objectLayersVector)
	{
		// 현재 레이어가 스킵할 리스트에 있는지 검사
		if (skipLayers.find(object.first) != skipLayers.end()) {
			continue;  // 스킵할 레이어면 다음 반복으로 넘어감
		}

		for (auto& iter : object.second)
		{
			CTransform* transform = dynamic_cast<CTransform*>(iter->Get_Component(TEXT("Com_Transform")));

			// Layer_ 뒤의 문자열 추출
			wstring layerName = object.first;
			size_t pos = layerName.find(L"Layer_");
			if (pos != wstring::npos) {
				// "Layer_" 다음 문자열 시작 위치는 pos + 6
				wstring suffix = layerName.substr(pos + 6);

				// 새로운 prefix 생성
				wstring newPrefix = L"Prototype_GameObject_" + suffix;

				// 이 새로운 문자열을 vecFileData에 추가
				vecFileData.emplace_back<FILEDATA>({ newPrefix, object.first, currentLevel, transform->Get_State(CTransform::STATE_POSITION), transform->Get_Scaled(), true });
			}
		}
	}

	// currentLevel에 따라 파일 이름 지정
	wstring filePath;
	switch (currentLevel)
	{
	case LEVEL_STATIC:
		filePath = L"../Bin/LevelStaticObjects.txt";
		break;
	case LEVEL_LOADING:
		filePath = L"../Bin/LevelLoadingObjects.txt";
		break;
	case LEVEL_LOGO:
		filePath = L"../Bin/LevelLogoObjects.txt";
		break;
	case LEVEL_GAMEPLAY:
		filePath = L"../Bin/LevelGameplayObjects.txt";
		break;
	case LEVEL_EDIT:
		filePath = L"../Bin/LevelEditObjects.txt";
		break;
	default:
		filePath = L"../Bin/DefaultLevelObjects.txt"; // default 이름
		break;
	}


	// 이걸 레벨당 사본 객체 리스트를 넘겨줘야 한다.
	HRESULT result = m_pGameInstance->SaveObjects(filePath.c_str(), &vecFileData);
	if (result == S_OK) {
		*bShowSaveSuccessMessage = true;
		*bShowSaveFailMessage = false;

		return S_OK;
	}
	else {
		*bShowSaveFailMessage = true;
		*bShowSaveSuccessMessage = false;

		return E_FAIL;
	}
}


HRESULT CMainApp::Load_Button_Pressed()
{
	return E_NOTIMPL;
}

HRESULT CMainApp::Click_Collider_Toggle(bool isChecked)
{
	m_pGameInstance->Show_Collider(isChecked);
	return S_OK;
}

HRESULT CMainApp::OnCollisionCheckIntervalChanged(float _fCollisionCheckInterval)
{
	m_pGameInstance->OnCollisionCheckIntervalChanged(_fCollisionCheckInterval);
	return S_OK;
}

HRESULT CMainApp::Click_Button_Release(std::vector<CGameObject*>& selectedGameObjects)
{
	for (auto it = selectedGameObjects.begin(); it != selectedGameObjects.end(); /* 빈 상태 */) {
		CGameObject* gameObject = *it;
		// 삭제 로직 호출
		Safe_Release(gameObject); // 오브젝트 릴리즈
		it = selectedGameObjects.erase(it); // 리스트에서 오브젝트 제거
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

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		CAnimator::Create(m_pGraphic_Device))))
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
