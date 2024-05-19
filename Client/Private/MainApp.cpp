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

bool bShowImGuiWindows = true;  // IMGUI â ǥ�� ���θ� �����ϴ� ���� ����

CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	/* �̱��� ��ü�� �ּҴ� ��ü Ŭ���� ���� ��������� ���������� �� �ּҸ� CMainAppŬ������ ����������� �����Ͽ��⶧�� */
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

	/* �� ������ �����ϱ����� �ʿ��� ���� �ʱ�ȭ �۾��� �����Ѵ�. */
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

	// ��� ��ư
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Option")) {
			if (ImGui::MenuItem("IMGUI ON", NULL, &bShowImGuiWindows)) {
				// bShowImGuiWindows ���� Ŭ���� ������ ��۵˴ϴ�.
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (bShowImGuiWindows) {  // �� ������ ���� ��� ImGui â�� ǥ�� ���θ� ����

		ImGui::Begin("Main Tab", &bShowImGuiWindows); // ���� â ����
		if (ImGui::BeginTabBar("MyTabBar")) { // �� �� ����
			if (ImGui::BeginTabItem("Levels")) { // "Levels" �� ������ ����
				//���� â
				bool bShowLevelWindow = true;
				if (bShowLevelWindow) {
					ShowLevels();
				}
				ImGui::EndTabItem(); // "Levels" �� ������ ����
			}


			if (ImGui::BeginTabItem("Prototype_ObjectList")) { // "ObjectList" �� ������ ����
				//���� ������Ʈ â
				bool bShowObjectWindow = true;
				if (bShowObjectWindow) {
					Show_PrototypeObjects();
				}
				ImGui::EndTabItem(); // "ObjectList" �� ������ ����
			}

			if (ImGui::BeginTabItem("Layer_ObjectList")) { // "ObjectList" �� ������ ����
				//���� ������Ʈ â
				bool bShowObjectWindow = true;
				if (bShowObjectWindow) {
					Show_LayerObjects();
				}
				ImGui::EndTabItem(); // "ObjectList" �� ������ ����
			}

			ImGui::EndTabBar(); // �� �� ����
		}
		ImGui::End();


		bool bShowSettings = true;


		static bool bShowSaveSuccessMessage = false;
		static bool bShowSaveFailMessage = false;

		// ���ο� â �߰�
		ImGui::Begin("Settings", &bShowSettings); // "Settings" â ����
		if (ImGui::Button("Save")) { // "Save" ��ư
			Save_Button_Pressed(&bShowSaveSuccessMessage, &bShowSaveFailMessage);
		}

		if (bShowSaveSuccessMessage) {
			ImGui::Text("File saved successfully!");
		}

		if (bShowSaveFailMessage) {
			ImGui::Text("Failed to save file!");
		}

		if (ImGui::Button("Load")) { // "Load" ��ư
			//LoadSettings();
		}

		ImGui::End(); // "Settings" â ����


		// ���ο� Collider â �߰�
		static bool bColliderToggle = false;
		ImGui::Begin("Collider", &bShowSettings);
		if (ImGui::Checkbox("Toggle Collider", &bColliderToggle)) {
			Click_Collider_Toggle(bColliderToggle);
		}

		// CollisionCheckInterval ���� ����
		static float CollisionCheckInterval = 0.f;
		// CollisionCheckInterval �Է¶� �߰�
		ImGui::Text("CollisionInterval:");
		ImGui::SameLine(); // ���� ���ο� ��ġ
		ImGui::PushItemWidth(50); // �Է� �ʵ� �ʺ� ����
		if (ImGui::InputFloat("##CollisionCheckInterval", &CollisionCheckInterval)) {
			// ����ڰ� ���ڸ� �������� �� ȣ��� �Լ�
			OnCollisionCheckIntervalChanged(CollisionCheckInterval);
		}
		ImGui::PopItemWidth();

		// WASDQE Ȱ��ȭ ��� �߰�
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
	// ����: ���� �̸��� ID�� ���� �迭 �Ǵ� ����Ʈ
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
	int selectedItem = -1; // �ʱ� ���� �׸� ����

	for (int i = 0; i < objectPrototypes.size(); i++) {
		if (ImGui::Selectable(objectPrototypes[i].c_str(), selectedItem == i)) {
			selectedItem = i;
			// ������Ʈ ���� ����
			SpawnObjectAtZero(objectPrototypes[i]);
		}
	}

	return S_OK;

}

HRESULT CMainApp::Show_LayerObjects()
{
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();
	ImGui::Columns(2, NULL, true); // �� ���� �÷� ����

	static int selectedItem = -1; // �ʱ� ���� �׸� ����
	static vector<int> selectedObjects; // ���õ� ��ü���� �ε��� ����
	static vector<CGameObject*> selectedGameObjects; // ���õ� ��ü���� ������ ����

	vector<pair<string, list<CGameObject*>>> objectLayersVector;
	m_pGameInstance->AddObjectLayersVector(currentLevel, &objectLayersVector);

	ImGui::BeginChild("LeftPane", ImVec2(150, 0), true); // ���� â ����
	for (int i = 0; i < objectLayersVector.size(); i++) {
		if (ImGui::Selectable(objectLayersVector[i].first.c_str(), selectedItem == i)) {
			selectedItem = i;
			selectedObjects.clear(); // �� ���̾ �����ϸ� ������ â�� ���� �ʱ�ȭ
			selectedGameObjects.clear(); // ���õ� ��ü �ʱ�ȭ
		}
	}

	ImGui::EndChild();
	ImGui::NextColumn(); // ���� �÷����� �̵�

	ImGui::BeginChild("RightPane", ImVec2(0, 0), true); // ������ â ����
	if (selectedItem != -1) { // ��ȿ�� ���̾ ���õ� ���
		const string& layerName = objectLayersVector[selectedItem].first;
		list<CGameObject*>& gameObjects = objectLayersVector[selectedItem].second;
		int index = 0;  // ���� ������Ʈ�� ��ȣ�� ǥ���ϱ� ���� �ε���
		for (auto it = gameObjects.begin(); it != gameObjects.end(); /* �� ���� */) {
			CGameObject* gameObject = *it;
			bool isSelected = std::find(selectedObjects.begin(), selectedObjects.end(), index) != selectedObjects.end();

			// ���� ������Ʈ�� Ŭ���Ǿ����� Ȯ���ϰ�, Ŭ���� ��� IMGUI�� ���� ���¸� ������Ʈ�մϴ�.
			if (gameObject->GetIsPicking()) {
				// ���� ���� ���¸� �ʱ�ȭ
				selectedObjects.clear();
				selectedGameObjects.clear();

				// ���ο� ���� ���¸� ����
				selectedObjects.push_back(index);
				selectedGameObjects.push_back(gameObject);
				gameObject->SetPicking(false); // Ŭ�� ���� �ʱ�ȭ
			}


			if (ImGui::Selectable((layerName + " " + std::to_string(index)).c_str(), isSelected)) {
				if (ImGui::GetIO().KeyCtrl) { // Ctrl Ű�� �����ִ� ���¿����� ���� ����
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

			++it; // �������� ������ ���� ������Ʈ�� �̵�
			index++;  // ���� ������Ʈ�� ���� �ε��� ����
		}
	}

	if (!selectedGameObjects.empty())
	{
		// Ű �Է��� ó���Ͽ� ������ �̵�
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

			// ������Ʈ ������ ������Ʈ
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

		// ���⼭ �߰� ������ ������ �� ���� (��: �� ���� ǥ��)
		_float3 averagePosition = { 0.0f, 0.0f, 0.0f };
		for (CGameObject* gameObject : selectedGameObjects) {
			CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
			if (component != nullptr) {
				CTransform* transform = static_cast<CTransform*>(component);
				averagePosition += transform->Get_State(CTransform::STATE_POSITION);
			}
		}
		averagePosition /= static_cast<float>(selectedGameObjects.size());

		// ImGui ��Ÿ�� �� �ʺ� ����
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4)); // ��� �� ���� ����

		float sliderWidth = 150.0f; // �����̴��� �ʺ�
		float inputWidth = 50.0f;   // �Է� �ʵ��� �ʺ�
		float sliderPower = 0.01f;  // �����̴� ���� �ΰ���

		// �����̴� ���� ����
		float rangeX = 50.0f;
		float rangeY = 50.0f;
		float rangeZ = 50.0f;

		ImGui::Text("Position: ");
		ImGui::PushButtonRepeat(true); // ��ư �ݺ��� Ȱ��ȭ

		// X �� ����
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

		// Y �� ����
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

		// Z �� ����
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

		ImGui::PopButtonRepeat(); // ��ư �ݺ��� ��Ȱ��ȭ

		for (CGameObject* gameObject : selectedGameObjects) {
			CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
			if (component != nullptr) {
				CTransform* transform = static_cast<CTransform*>(component);
				transform->Set_State(CTransform::STATE_POSITION, &averagePosition);
			}
		}

		// ������ ���� �����̴�
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
		ImGui::PushButtonRepeat(true); // ��ư �ݺ��� �ٽ� Ȱ��ȭ
		ImGui::PushItemWidth(sliderWidth);

		// ������ X �� ����
		if (ImGui::Button("-SX")) averageScale.x -= 0.01f;
		ImGui::SameLine();
		ImGui::SliderFloat("##ScaleX", &averageScale.x, 0.1f, 10.0f, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+SX")) averageScale.x += 0.01f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##ScaleXInput", &averageScale.x);
		ImGui::PopItemWidth();

		// ������ Y �� ����
		if (ImGui::Button("-SY")) averageScale.y -= 0.01f;
		ImGui::SameLine();
		ImGui::SliderFloat("##ScaleY", &averageScale.y, 0.1f, 10.0f, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+SY")) averageScale.y += 0.01f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##ScaleYInput", &averageScale.y);
		ImGui::PopItemWidth();

		// ������ Z �� ����
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
		ImGui::PopButtonRepeat(); // ��ư �ݺ��� �ٽ� ��Ȱ��ȭ
		ImGui::PopStyleVar(); // ��Ÿ�� ���� ����

		for (CGameObject* gameObject : selectedGameObjects) {
			CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
			if (component != nullptr) {
				CTransform* transform = static_cast<CTransform*>(component);
				transform->Set_Scaled(averageScale); // ������ ����
			}
		}

		// ���� ��ư �߰�
		ImGui::Separator(); // ���м� �߰�
		if (ImGui::Button("Delete Selected Object")) {
			Click_Button_Release(selectedGameObjects);
		}
	}
	ImGui::EndChild();

	ImGui::Columns(1); // �� ����

	for (auto& iter : objectLayersVector)
	{
		for (auto& iter2 : iter.second)
			Safe_Release(iter2);
	}

	return S_OK;
}

HRESULT CMainApp::Save_Button_Pressed(bool* bShowSaveSuccessMessage, bool* bShowSaveFailMessage)
{
	// ������Ʈ������ ����
	vector<FILEDATA> vecFileData;

	// ���� ���� 
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	vector<pair<wstring, list<CGameObject*>>> objectLayersVector;
	m_pGameInstance->AddObjectLayersVector(currentLevel, &objectLayersVector);

	// ���⿡ ��ŵ�� ���̾� �̸��� ����
	unordered_set<wstring> skipLayers =
	{ L"Layer_BackGround", L"Layer_Camera", L"Layer_Player" };

	for (auto& object : objectLayersVector)
	{
		// ���� ���̾ ��ŵ�� ����Ʈ�� �ִ��� �˻�
		if (skipLayers.find(object.first) != skipLayers.end()) {
			continue;  // ��ŵ�� ���̾�� ���� �ݺ����� �Ѿ
		}

		for (auto& iter : object.second)
		{
			CTransform* transform = dynamic_cast<CTransform*>(iter->Get_Component(TEXT("Com_Transform")));

			// Layer_ ���� ���ڿ� ����
			wstring layerName = object.first;
			size_t pos = layerName.find(L"Layer_");
			if (pos != wstring::npos) {
				// "Layer_" ���� ���ڿ� ���� ��ġ�� pos + 6
				wstring suffix = layerName.substr(pos + 6);

				// ���ο� prefix ����
				wstring newPrefix = L"Prototype_GameObject_" + suffix;

				// �� ���ο� ���ڿ��� vecFileData�� �߰�
				vecFileData.emplace_back<FILEDATA>({ newPrefix, object.first, currentLevel, transform->Get_State(CTransform::STATE_POSITION), transform->Get_Scaled(), true });
			}
		}
	}

	// currentLevel�� ���� ���� �̸� ����
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
		filePath = L"../Bin/DefaultLevelObjects.txt"; // default �̸�
		break;
	}


	// �̰� ������ �纻 ��ü ����Ʈ�� �Ѱ���� �Ѵ�.
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
	for (auto it = selectedGameObjects.begin(); it != selectedGameObjects.end(); /* �� ���� */) {
		CGameObject* gameObject = *it;
		// ���� ���� ȣ��
		Safe_Release(gameObject); // ������Ʈ ������
		it = selectedGameObjects.erase(it); // ����Ʈ���� ������Ʈ ����
	}
	return S_OK;
}

HRESULT CMainApp::SpawnObjectAtZero(const std::string& type)
{
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	// ���ڿ����� "Prototype_GameObject_" ���ξ� ����
	string prefix = "Prototype_GameObject_";
	string suffix;

	if (type.substr(0, prefix.size()) == prefix) {
		suffix = type.substr(prefix.size());
	}
	else {
		suffix = type; // ���ξ ���ٸ� ��ü Ÿ���� ���
	}

	// Layer �̸� ����
	string layerName = "Layer_" + suffix;

	wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	wstring converted = converter.from_bytes(type);
	wstring wLayerName = converter.from_bytes(layerName);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, converted.c_str(), wLayerName)))
		return E_FAIL;

	return S_OK;
}

/* �� ���ӿ� ���ʰ��Ǵ� ����̽� ����(��������, ���ø���Ŀ����� ����)�� �����Ѵ�. */
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
