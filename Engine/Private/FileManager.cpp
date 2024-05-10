#include "..\Public\FileManager.h"
#include <sstream>
CFile_Manager::CFile_Manager()
{
}

HRESULT CFile_Manager::Initialize()
{
	return S_OK;
}

HRESULT CFile_Manager::SaveObjects(const wstring& filename)
{
	wofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Cannot open file for writing.");
		return E_FAIL;
	}

	for (const auto& data : m_vecFileData) {
		file << L"LayerName: " << data.layerName << L"\n";
		file << L"PrototypeTag: " << data.prototypeTag << L"\n";
		file << L"LevelIndex: " << data.levelIndex << L"\n";
		file << L"Position: " << data.position.x << L" " << data.position.y << L" " << data.position.z << L"\n";
		file << L"Rotation: " << data.rotation.x << L" " << data.rotation.y << L" " << data.rotation.z << L"\n";
		file << L"Scale: " << data.scale.x << L" " << data.scale.y << L" " << data.scale.z << L"\n\n";
	}

	file.close();

	return S_OK;
}

HRESULT CFile_Manager::LoadObjects(const wstring& filename)
{
	std::wifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Cannot open file for reading.");
		return E_FAIL;
	}

	wstring line;
	FileData data;

	while (getline(file, line)) {
		ParseLine(line, data);
		if (line.empty()) {
			m_vecFileData.push_back(data);  // Assuming a blank line indicates the end of one object's data
		}
	}

	file.close();

	return S_OK;
}

HRESULT CFile_Manager::ParseLine(const wstring& line, FileData& obj)
{
	wistringstream iss(line);
	wstring key;
	if (getline(iss, key, L':')) {
		wstring value;
		getline(iss, value);
		if (key == L"LayerName") {
			obj.layerName = value;
		}
		else if (key == L"PrototypeTag") {
			obj.prototypeTag = value;
		}
		else if (key == L"LevelIndex") {
			obj.levelIndex = std::stoi(value);
		}
		else if (key == L"Position" || key == L"Rotation" || key == L"Scale") {
			std::wistringstream vals(value);
			float a, b, c;
			vals >> a >> b >> c;
			if (key == L"Position") {
				obj.position.x = a;
				obj.position.y = b;
				obj.position.z = c;
			}
			else if (key == L"Rotation") {
				obj.rotation.x = a;
				obj.rotation.y = b;
				obj.rotation.z = c;
			}
			else if (key == L"Scale") {
				obj.scale.x = a;
				obj.scale.y = b;
				obj.scale.z = c;
			}
		}
	}
	else
		return E_FAIL;
}

CFile_Manager* CFile_Manager::Create()
{
	CFile_Manager* pInstance = new CFile_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CFileManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFile_Manager::Free()
{
	__super::Free();
}
