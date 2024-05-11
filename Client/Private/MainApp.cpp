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
			if (ImGui::Button("-X")) position.x -= 0.1f;
			ImGui::SameLine();
			ImGui::SliderFloat("##PosX", &position.x, 0, rangeX, "%.3f", sliderPower);
			ImGui::SameLine();
			if (ImGui::Button("+X")) position.x += 0.1f;
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			ImGui::InputFloat("##PosXInput", &position.x);
			ImGui::PopItemWidth();
			ImGui::PopItemWidth();

			// Y 축 조정
			ImGui::PushItemWidth(sliderWidth);
			if (ImGui::Button("-Y")) position.y -= 0.1f;
			ImGui::SameLine();
			ImGui::SliderFloat("##PosY", &position.y, 0, rangeY, "%.3f", sliderPower);
			ImGui::SameLine();
			if (ImGui::Button("+Y")) position.y += 0.1f;
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			ImGui::InputFloat("##PosYInput", &position.y);
			ImGui::PopItemWidth();
			ImGui::PopItemWidth();

			// Z 축 조정
			ImGui::PushItemWidth(sliderWidth);
			if (ImGui::Button("-Z")) position.z -= 0.1f;
			ImGui::SameLine();
			ImGui::SliderFloat("##PosZ", &position.z, 0, rangeZ, "%.3f", sliderPower);
			ImGui::SameLine();
			if (ImGui::Button("+Z")) position.z += 0.1f;
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			ImGui::InputFloat("##PosZInput", &position.z);
			ImGui::PopItemWidth();
			ImGui::PopItemWidth();

			ImGui::PopButtonRepeat(); // 버튼 반복을 비활성화
			transform->Set_State(CTransform::STATE_POSITION, &position);

			// 스케일 조절 슬라이더
			_float3 scale = transform->Get_Scaled();
			ImGui::Text("Scale: ");
			ImGui::PushButtonRepeat(true); // 버튼 반복을 다시 활성화
			ImGui::PushItemWidth(sliderWidth);

			// 스케일 X 축 조정
			if (ImGui::Button("-SX")) scale.x -= 0.01f;
			ImGui::SameLine();
			ImGui::SliderFloat("##ScaleX", &scale.x, 0.1f, 10.0f, "%.3f", sliderPower);
			ImGui::SameLine();
			if (ImGui::Button("+SX")) scale.x += 0.01f;
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			ImGui::InputFloat("##ScaleXInput", &scale.x);
			ImGui::PopItemWidth();

			// 스케일 Y 축 조정
			if (ImGui::Button("-SY")) scale.y -= 0.01f;
			ImGui::SameLine();
			ImGui::SliderFloat("##ScaleY", &scale.y, 0.1f, 10.0f, "%.3f", sliderPower);
			ImGui::SameLine();
			if (ImGui::Button("+SY")) scale.y += 0.01f;
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			ImGui::InputFloat("##ScaleYInput", &scale.y);
			ImGui::PopItemWidth();

			// 스케일 Z 축 조정
			if (ImGui::Button("-SZ")) scale.z -= 0.01f;
			ImGui::SameLine();
			ImGui::SliderFloat("##ScaleZ", &scale.z, 0.1f, 10.0f, "%.3f", sliderPower);
			ImGui::SameLine();
			if (ImGui::Button("+SZ")) scale.z += 0.01f;
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			ImGui::InputFloat("##ScaleZInput", &scale.z);
			ImGui::PopItemWidth();

			ImGui::PopItemWidth();
			ImGui::PopButtonRepeat(); // 버튼 반복을 다시 비활성화
			ImGui::PopStyleVar(); // 스타일 변수 복원

			transform->Set_Scaled(scale); // 스케일 적용

			////회전 적용
			//static _float3 lastEulerRotation = { 0.0f, 0.0f, 0.0f };
			//_float3 eulerRotation = lastEulerRotation;
			//ImGui::Text("Rotation: ");
			//bool changedX = ImGui::SliderFloat("Rotate X", &eulerRotation.x, -3.14159f, 3.14159f);
			//ImGui::InputFloat("Rotate X Input", &eulerRotation.x);
			//bool changedY = ImGui::SliderFloat("Rotate Y", &eulerRotation.y, -3.14159f, 3.14159f);
			//ImGui::InputFloat("Rotate Y Input", &eulerRotation.y);
			//bool changedZ = ImGui::SliderFloat("Rotate Z", &eulerRotation.z, -3.14159f, 3.14159f);
			//ImGui::InputFloat("Rotate Z Input", &eulerRotation.z);

			//if (changedX) {
			//	_float3 deltaRotation = { _float3(1.0f, 0.0f, 0.0f) * (eulerRotation.x - lastEulerRotation.x) };
			//	transform->Turn(_float3(1.0f, 0.0f, 0.0f), deltaRotation.x);
			//}
			//if (changedY) {
			//	_float3 deltaRotation = { _float3(0.0f, 1.0f, 0.0f) * (eulerRotation.y - lastEulerRotation.y) };
			//	transform->Turn(_float3(0.0f, 1.0f, 0.0f), deltaRotation.y);
			//}
			//if (changedZ) {
			//	_float3 deltaRotation = { _float3(0.0f, 0.0f, 1.0f) * (eulerRotation.z - lastEulerRotation.z) };
			//	transform->Turn(_float3(0.0f, 0.0f, 1.0f), deltaRotation.z);
			//}

			//lastEulerRotation = eulerRotation;

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

HRESULT CMainApp::Save_Button_Pressed(bool* bShowSaveSuccessMessage, bool* bShowSaveFailMessage)
{
	//오브젝트마다의 정보
	vector<FILEDATA> vecFileData;

	//레벨 정보 
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	vector<pair < wstring, list<CGameObject*>>> objectLayersVector;
	m_pGameInstance->AddObjectLayersVector(currentLevel, &objectLayersVector);

	for (auto& object : objectLayersVector)
	{
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
				vecFileData.emplace_back<FILEDATA>({ newPrefix, object.first, currentLevel, transform->Get_State(CTransform::STATE_POSITION), transform->Get_Scaled() });
			}
		}
	}

	//결국에 넘겨줘야 하는건 뭐야?
	// '하나의 오브젝트'에 대해서
	//1. 레이어 이름
	//2. 프로토타입 태그
	//3. 레벨정보
	//4. 트랜스폼 정보 (위치, 회전, 스케일 FLOAT3)

	//이걸 레벨당 사본객체 리스트를 넘겨줘야 한다.
	HRESULT result = m_pGameInstance->SaveObjects(TEXT("../Bin/ObjectData.txt"), &vecFileData);
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
