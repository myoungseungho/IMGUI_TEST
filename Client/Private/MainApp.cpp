#pragma once
#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\Public\MainApp.h"

#include "Sound.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "GameObject.h"
#include "Component.h"
#include "EnviormentObject.h"
#include "UI_Heart_Player.h"
#include "UI_Icon_KeyBoard.h"
#include "UI_HP_Player.h"
#include "UI_HP_Green_Player.h"
#include "UI_HP_GlueEffect_Player.h"
#include "UI_HP_BloodEffect.h"
#include "UI_FadeInOut.h"
#include "UI_Cursor.h"
#include "UI_Inventory_BackGround.h"
#include "UI_Inventory_DotLine.h"
#include "UI_Inventory_Slot.h"
#include "UI_Inventory_VerticalDotLine.h"
#include "UI_ItemTabIcon_Caution.h"
#include "UI_ItemTabIcon_Food.h"
#include "UI_ItemTabIcon_Hat.h"
#include "UI_ItemTabIcon_Leaf.h"
#include "UI_Shop_ItemInfo_TextBox.h"
#include "UI_Shop_PlayerCoin.h"
#include "UI_Shop_PriceTag.h"
#include "UI_Inventory.h"
#include "UI_Inventory_SlotBeigeBackground.h"
#include "UI_Inventory_Button.h"
#include "UI_PlayerEquipSlot.h"
#include "UI_PlayerEquipSlot_BackGround.h"
#include "UI_Hat.h" 
#include "UI_Item.h"
#include "UI_Loading_Background.h"
#include "UI_Loading_Orgu_Run.h"
#include "Bush.h"
#include "Sky.h"
#include "Tree.h"
#include "Player.h"
#include "Camera.h"
#include "Terrain.h"
#include "RockBreakable.h"
#include "Block.h"
#include "Hole.h"
#include "Push_Stone.h"
#include "Camera.h"
#include "UI_Npc_Talk.h"
#include "UI_Npc_Question_Effect.h"
#include "UI_MapGuide.h"
#include "TravelNpc.h"
#include <codecvt>
#include "Effect_Item.h"
#include "Effect_Player.h"
#include "Hat.h"
#include "Effect_Player_Stun.h"
#include "Effect_Bush_1.h"
#include "Effect_Player_Heal.h"

bool bShowImGuiWindows = false;  // IMGUI 창 표시 여부를 제어하는 전역 변수

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

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	//Ready_Sound();

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphic_Device);

	if (FAILED(Open_Level(LEVEL_SNOW)))
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
		{"Tacho", LEVEL_TACHO},
		{"Jungle", LEVEL_JUNGLE},
		{"Snow", LEVEL_SNOW},
		{"Koofu", LEVEL_KOOFU},
		{"Bug", LEVEL_BUG},
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

	string layerName; // layerName 변수를 스코프 바깥으로 이동

	ImGui::BeginChild("RightPane", ImVec2(0, 0), true); // 오른쪽 창 생성
	if (selectedItem != -1) { // 유효한 레이어가 선택된 경우
		layerName = objectLayersVector[selectedItem].first;
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

		// 복사 버튼 추가
		ImGui::Separator(); // 구분선 추가
		if (ImGui::Button("Copy Selected Object")) {
			for (CGameObject* gameObject : selectedGameObjects) {
				// 레이어 이름과 함께 객체를 복사합니다.
				wstring_convert<std::codecvt_utf8<wchar_t>> converter;
				std::wstring wLayerName = converter.from_bytes(layerName);
				CopySelectedObject(wLayerName, gameObject);
			}
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
	{ L"Layer_BackGround", L"Layer_Camera", L"Layer_Player", L"Layer_Skill_Player", L"Layer_End_Orb", L"Layer_Rotation_Orb" , L"Layer_UnRotation_Orb",
		L"Layer_Small_Orb", L"Layer_Laser",L"Layer_Shop" };

	// "Layer_UI"로 시작하는 모든 레이어를 스킵할 레이어에 추가
	for (const auto& object : objectLayersVector)
	{
		if (object.first.find(L"Layer_UI") == 0 ||
			object.first.find(L"Layer_ZUI") == 0 ||
			object.first.find(L"Layer_XUI") == 0 ||
			object.first.find(L"Layer_Npc") == 0)
		{
			skipLayers.insert(object.first);
		}
	}

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
	case LEVEL_TACHO:
		filePath = L"../Bin/LevelTachoObjects.txt";
		break;
	case LEVEL_JUNGLE:
		filePath = L"../Bin/LevelJungleObjects.txt";
		break;
	case LEVEL_SNOW:
		filePath = L"../Bin/LevelSnowObjects.txt";
		break;
	case LEVEL_KOOFU:
		filePath = L"../Bin/LevelKoofuObjects.txt";
		break;
	case LEVEL_BUG:
		filePath = L"../Bin/LevelBugObjects.txt";
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

HRESULT CMainApp::CopySelectedObject(const std::wstring& layerName, CGameObject* sourceObject)
{
	// Get the current level index
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	// Extract the prototype suffix from the layer name
	std::wstring prototypeSuffix = layerName.substr(6); // "Layer_" 이후의 문자열 추출
	std::wstring prototypeTag = L"Prototype_GameObject_" + prototypeSuffix;

	// Copy the position and scale from the source object
	CComponent* sourceComponent = sourceObject->Get_Component(TEXT("Com_Transform"));
	CTransform* sourceTransform = static_cast<CTransform*>(sourceComponent);

	FILEDATA copy_DESC{};
	copy_DESC.prototypeTag = prototypeTag;
	copy_DESC.layerName = layerName;
	copy_DESC.position = sourceTransform->Get_State(CTransform::STATE_POSITION);
	copy_DESC.scale = sourceTransform->Get_Scaled();
	copy_DESC.isParsing = false;

	// Create a new game object using the prototype
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, prototypeTag.c_str(), layerName.c_str(), &copy_DESC)))
		return E_FAIL;

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

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 100, 100))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURECUBE, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/SkyBox/SkyBox_%d.dds"), 4))))
		return E_FAIL;

	//콜라이더 컴포넌트
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		CCollider::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma region AnimTexture_Player_Idle

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/Up/Player_Idle_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/Right/Player_Idle_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/Down/Player_Idle_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/Left/Player_Idle_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/LeftUp/Player_Idle_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/RIghtUp/Player_Idle_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/RightDown/Player_Idle_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Idle_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/LeftDown/Player_Idle_%d.png"), 7))))
		return E_FAIL;
#pragma endregion

#pragma region Animation_Player_Walk

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Up/Player_Walk_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Right/Player_Walk_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Down/Player_Walk_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Left/Player_Walk_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/LeftUp/Player_Walk_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/RIghtUp/Player_Walk_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/RightDown/Player_Walk_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Walk_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/LeftDown/Player_Walk_%d.png"), 10))))
		return E_FAIL;
#pragma endregion

#pragma region Animation_Player_Skill

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/Up/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/Right/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/Down/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/Left/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/LeftUp/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/RightUp/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/RightDown/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Skill_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/LeftDown/Player_Smash_%d.png"), 8))))
		return E_FAIL;

#pragma endregion

#pragma region Animation_Player_Attack

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/Up/Player_Swing_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/Right/Player_Swing_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/Down/Player_Swing_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/Left/Player_Swing_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/LeftUp/Player_Swing_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/RightUp/Player_Swing_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/RightDown/Player_Swing_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Attack_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/LeftDown/Player_Swing_%d.png"), 6))))
		return E_FAIL;
#pragma endregion

#pragma region Animation_Player_Push

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Push_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Push/Up/Player_Push_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Push_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Push/Right/Player_Push_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Push_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Push/Down/Player_Push_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Push_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Push/Left/Player_Push_%d.png"), 10))))
		return E_FAIL;
#pragma endregion

#pragma region Animation_Player_Travel

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Ballon_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Npc/Travel/Sprite_BabyOgu_BalloonFly_%d.png"), 19))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Ballon_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Npc/Travel/Sprite_BabyOgu_BalloonFly_Down_%d.png"), 11))))
		return E_FAIL;

#pragma endregion

#pragma region Texture_Player_Skill

	/* For.Prototype_Component_Texture_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Skill"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Effect/Smash/ForSkill/Sprite_OguSmashCrackBlur.png"), 1))))
		return E_FAIL;

#pragma endregion

#pragma region Texture_Player_Hit
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Hit/Up/Player_Hit_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Hit/Right/Player_Hit_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Hit/Down/Player_Hit_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Hit/Left/Player_Hit_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Hit/LeftUp/Player_Hit_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Hit/RightUp/Player_Hit_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Hit/RightDown/Player_Hit_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Hit_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Hit/LeftDown/Player_Hit_%d.png"), 3))))
		return E_FAIL;

#pragma endregion

#pragma region AnimTexture_Player_Get

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Get_Item"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_GetITem/Player_GetItem_%d.png"), 6))))
		return E_FAIL;

#pragma endregion

#pragma region  AnimTexture_Player_Died
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Down/Player_Down_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Player_Live"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Live/Player_Live_%d.png"), 10))))
		return E_FAIL;
#pragma endregion

#pragma region AnimTexture_Effect_Player
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Effect_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing_Effect/Sprite_OguSwing_Effect_%d.png"), 8))))
		return E_FAIL;
#pragma endregion

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Hole_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_Hole_Idle.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Sprite_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_Hole_%d.png"), 4))))
		return E_FAIL;

	/* 애니메이션 텍스쳐를 로드한다*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Sprite_Bush_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_LootNaturalObjects_ColorBush0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Sprite_Bush_Move"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_LootNaturalObjects_ColorBush%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Block_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block_Blocking/Sprite_StoneBlockBlocking.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Block_UnIdle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block_Idle/Sprite_StoneBlockNotBlocking.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_UnBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/UnBlock/Sprite_StoneUnBlock%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Block"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block/Sprite_StoneBlock%d.png"), 9))))
		return E_FAIL;

#pragma region AnimTexture_RockBreakable
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_RockBreakable_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_RockBreakable0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_RockBreakable_Die"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_RockBreakable%d.png"), 8))))
		return E_FAIL;
#pragma endregion


#pragma region UI
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Icon_HeartSymbol"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Icon/UI_Icon_HeartSymbol.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Icon_KeyBoard"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Icon/UI_Icon_Keyboard.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_HP_Frame"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Icon/UI_HP_Frame.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_HP_Green"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/UI_HP_GaugeBar.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_HP_GlueEffect_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/HPGaugeBar.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_HP_BloodEffect"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Sprite_UI_HPBarFrameLine.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_FadeInOut"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Kakao_%d.png"), 2))))
		return E_FAIL;

	//인벤토리
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Sprite_UI_Cursor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/UI_Cursor_Test_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Inventory_BackGround"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/UI_Shop_Test.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Inventory_DotLine"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/UI_Shop_DotLineDetail.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Inventory_Slot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/UI_Shop_Test_AddSlot.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Inventory_VerticalDotLine"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/UI_Shop_VerticalDotLine.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_ItemTabIcon_Caution"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/UI_ItemTabIcon_3.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_ItemTabIcon_Food"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/UI_ItemTabIcon_1.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_ItemTabIcon_Hat"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/UI_ItemTabIcon_0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_ItemTabIcon_Leaf"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/UI_ItemTabIcon_2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Shop_ItemInfo_TextBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/UI_Shop_ItemInfo_TextBox.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Shop_PlayerCoin"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/UI_Shop_PlayerCoin.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Shop_PriceTag"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/UI_Shop_PriceTag.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Inventory_SlotBeigeBackground"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/Sprite_RestaurantTycoon_PageCover.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Inventory_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/Sprite_UI_MainComponents_InventoryButton.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_PlayerEquipSlot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/Sprite_UI_MainComponents_SlotFrame.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_PlayerEquipSlot_BackGround"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Shop/Sprite_UISystem_CursorBack.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_Item"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Item/ItemSprite/Sprite_Item_%d.png"), 16))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_Hat"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Item/ItemSprite/Sprite_ItemThumbnails_Equipment_%d.png"), 64))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_Loading_BackGround"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Loading/BackGround/BackGround.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_Sprite_Loading_Ogu"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Loading/Ogu/Loading_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_MainComponents_ArrowDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Sprite_UI_MainComponents_ArrowDown.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_BoxShadow"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Sprite_BoxShadow.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_HP_GuageBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/UI_Boss_HP_GuageBar.png"), 1))))
		return E_FAIL;

#pragma endregion

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Item"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/GetItem/GetItem_0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tree"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tree_69.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Terrain/Orgu_Terrain_%d.png"), 5))))
		return E_FAIL;

#pragma region NPC
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Test_UITextBox_TextBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Basic/Test_UITextBox_TextBox.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_UI_Quest_QuestionMark"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Quest/QuestionMark/UI_Quest_QuestionMark_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_UI_Quest_QuestionMarkBlur"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/UI/Quest/QuestionMark/UI_Quest_QuestionMark_Blur_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AnimTexture_BalloonMonkey"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/NPC/Travel/Sprite_BalloonMonkey_%d.png"), 14))))
		return E_FAIL;

#pragma endregion

#pragma region Texture_Player_Hat_Towel

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hat_Towel"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Hat/Sprite_HatGot.png"), 1))))
		return E_FAIL;

#pragma endregion

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Player_Stun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Effect/OneTexture/Sprite_ThornHatRoll_Spikes.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Bush_1"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/Leaf/Leaf_Fixed/Leaf_Fixed_0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Player_Heal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Effect/OneTexture/Sprite_LensFlare_Source.png"), 1))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Player_Heal"),
		CEffect_Player_Heal::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Bush_1"),
		CEffect_Bush_1::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hat_Towel"),
		CHat::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Player_Stun"),
		CEffect_Player_Stun::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Player"),
		CEffect_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Item"),
		CEffect_Item::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"),
		CCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RockBreakable"),
		CRockBreakable::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Block"),
		CBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StonePushable"),
		CPush_Stone::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma region UIPrototype
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Heart_Player"),
		CUI_Heart_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Icon_KeyBoard"),
		CUI_Icon_KeyBoard::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HP_Player"),
		CUI_HP_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HP_Green_Player"),
		CUI_HP_Green_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HP_GlueEffect_Player"),
		CUI_HP_GlueEffect_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HP_BloodEffect"),
		CUI_HP_BloodEffect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_FadeInOut"),
		CUI_FadeInOut::Create(m_pGraphic_Device))))
		return E_FAIL;

	///////// 인벤토리

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory"),
		CInventory::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Cursor"),
		CUI_Cursor::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_BackGround"),
		CUI_Inventory_BackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_DotLine"),
		CUI_Inventory_DotLine::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot"),
		CUI_Inventory_Slot::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_VerticalDotLine"),
		CUI_Inventory_VerticalDotLine::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ItemTabIcon_Caution"),
		CUI_ItemTabIcon_Caution::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ItemTabIcon_Food"),
		CUI_ItemTabIcon_Food::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ItemTabIcon_Hat"),
		CUI_ItemTabIcon_Hat::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ItemTabIcon_Leaf"),
		CUI_ItemTabIcon_Leaf::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Shop_ItemInfo_TextBox"),
		CUI_Shop_ItemInfo_TextBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Shop_PlayerCoin"),
		CUI_Shop_PlayerCoin::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Shop_PriceTag"),
		CUI_Shop_PriceTag::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_SlotBeigeBackground"),
		CUI_Inventory_SlotBeigeBackground::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Button"),
		CUI_Inventory_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_PlayerEquipSlot"),
		CUI_PlayerEquipSlot::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_PlayerEquipSlot_BackGround"),
		CUI_PlayerEquipSlot_BackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Hat"),
		CUI_Hat::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Item"),
		CUI_Item::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_Background"),
		CUI_Loading_Background::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_Orgu_Run"),
		CUI_Loading_Orgu_Run::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bush"),
		CBush::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Npc_Talk"),
		CUI_Npc_Talk::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Npc_Question_Effect"),
		CUI_Npc_Question_Effect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MapGuide"),
		CUI_MapGuide::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TravelNpc"),
		CTravelNpc::Create(m_pGraphic_Device))))
		return E_FAIL;
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
