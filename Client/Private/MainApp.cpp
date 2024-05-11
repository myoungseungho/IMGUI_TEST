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

bool bShowImGuiWindows = true;  // IMGUI â ǥ�� ���θ� �����ϴ� ���� ����

CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	/* �̱��� ��ü�� �ּҴ� ��ü Ŭ���� ���� ��������� ���������� �� �ּҸ� CMainAppŬ������ ����������� �����Ͽ��⶧�� */
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
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
	// ����: ���� �̸��� ID�� ���� �迭 �Ǵ� ����Ʈ
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
	static int selectedObject = -1;
	static CGameObject* pSelectedGameObject = nullptr; // ���õ� ��ü�� ������

	vector<pair < string, list<CGameObject*>>> objectLayersVector;
	m_pGameInstance->AddObjectLayersVector(currentLevel, &objectLayersVector);

	ImGui::BeginChild("LeftPane", ImVec2(150, 0), true); // ���� â ����
	for (int i = 0; i < objectLayersVector.size(); i++) {
		if (ImGui::Selectable(objectLayersVector[i].first.c_str(), selectedItem == i)) {
			selectedItem = i;
			selectedObject = -1; // �� ���̾ �����ϸ� ������ â�� ���� �ʱ�ȭ
			pSelectedGameObject = nullptr;
		}
	}

	ImGui::EndChild();
	ImGui::NextColumn(); // ���� �÷����� �̵�

	ImGui::BeginChild("RightPane", ImVec2(0, 0), true); // ������ â ����
	if (selectedItem != -1) { // ��ȿ�� ���̾ ���õ� ���
		const string& layerName = objectLayersVector[selectedItem].first;
		const list<CGameObject*>& gameObjects = objectLayersVector[selectedItem].second;
		int index = 0;  // ���� ������Ʈ�� ��ȣ�� ǥ���ϱ� ���� �ε���
		for (CGameObject* gameObject : gameObjects) {
			// �� ���� ������Ʈ�� Ŭ�� �����ϰ� ����
			if (ImGui::Selectable((layerName + " " + std::to_string(index)).c_str(), selectedObject == index)) {
				selectedObject = index; // ���õ� ��ü �ε��� ������Ʈ
				pSelectedGameObject = gameObject; // ���õ� ��ü ����
			}
			index++;  // ���� ������Ʈ�� ���� �ε��� ����
		}
	}

	if (pSelectedGameObject)
	{
		// ���⼭ �߰� ������ ������ �� ���� (��: �� ���� ǥ��)
		CComponent* component = pSelectedGameObject->Get_Component(TEXT("Com_Transform"));
		if (component != nullptr)
		{
			component->AddRef();
			CTransform* transform = static_cast<CTransform*>(component);
			_float3 position = transform->Get_State(CTransform::STATE_POSITION);

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

			// Y �� ����
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

			// Z �� ����
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

			ImGui::PopButtonRepeat(); // ��ư �ݺ��� ��Ȱ��ȭ
			transform->Set_State(CTransform::STATE_POSITION, &position);

			// ������ ���� �����̴�
			_float3 scale = transform->Get_Scaled();
			ImGui::Text("Scale: ");
			ImGui::PushButtonRepeat(true); // ��ư �ݺ��� �ٽ� Ȱ��ȭ
			ImGui::PushItemWidth(sliderWidth);

			// ������ X �� ����
			if (ImGui::Button("-SX")) scale.x -= 0.01f;
			ImGui::SameLine();
			ImGui::SliderFloat("##ScaleX", &scale.x, 0.1f, 10.0f, "%.3f", sliderPower);
			ImGui::SameLine();
			if (ImGui::Button("+SX")) scale.x += 0.01f;
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			ImGui::InputFloat("##ScaleXInput", &scale.x);
			ImGui::PopItemWidth();

			// ������ Y �� ����
			if (ImGui::Button("-SY")) scale.y -= 0.01f;
			ImGui::SameLine();
			ImGui::SliderFloat("##ScaleY", &scale.y, 0.1f, 10.0f, "%.3f", sliderPower);
			ImGui::SameLine();
			if (ImGui::Button("+SY")) scale.y += 0.01f;
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			ImGui::InputFloat("##ScaleYInput", &scale.y);
			ImGui::PopItemWidth();

			// ������ Z �� ����
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
			ImGui::PopButtonRepeat(); // ��ư �ݺ��� �ٽ� ��Ȱ��ȭ
			ImGui::PopStyleVar(); // ��Ÿ�� ���� ����

			transform->Set_Scaled(scale); // ������ ����

			////ȸ�� ����
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
	//������Ʈ������ ����
	vector<FILEDATA> vecFileData;

	//���� ���� 
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	vector<pair < wstring, list<CGameObject*>>> objectLayersVector;
	m_pGameInstance->AddObjectLayersVector(currentLevel, &objectLayersVector);

	for (auto& object : objectLayersVector)
	{
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
				vecFileData.emplace_back<FILEDATA>({ newPrefix, object.first, currentLevel, transform->Get_State(CTransform::STATE_POSITION), transform->Get_Scaled() });
			}
		}
	}

	//�ᱹ�� �Ѱ���� �ϴ°� ����?
	// '�ϳ��� ������Ʈ'�� ���ؼ�
	//1. ���̾� �̸�
	//2. ������Ÿ�� �±�
	//3. ��������
	//4. Ʈ������ ���� (��ġ, ȸ��, ������ FLOAT3)

	//�̰� ������ �纻��ü ����Ʈ�� �Ѱ���� �Ѵ�.
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
