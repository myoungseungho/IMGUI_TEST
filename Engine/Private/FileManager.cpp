#include "..\Public\FileManager.h"
#include <sstream>

CFile_Manager::CFile_Manager()
{
}

HRESULT CFile_Manager::Initialize()
{
	return S_OK;
}

HRESULT CFile_Manager::SaveObjects(const wstring& filename, void* pArg)
{
	wofstream file(filename);
	if (!file.is_open()) {
		throw runtime_error("Cannot open file for writing.");
		return E_FAIL;
	}

	vector<FILEDATA>* pvecFileData = reinterpret_cast<vector<FILEDATA>*>(pArg);

	for (const auto& data : *pvecFileData) {
		file << L"LayerName: " << data.layerName << L"\n";
		file << L"PrototypeTag: " << data.prototypeTag << L"\n";
		file << L"LevelIndex: " << data.levelIndex << L"\n";
		file << L"Position: " << data.position.x << L" " << data.position.y << L" " << data.position.z << L"\n";
		file << L"Scale: " << data.scale.x << L" " << data.scale.y << L" " << data.scale.z << L"\n";
		file << L"IsParsing: " << data.isParsing << L"\n\n";
	}

	file.close();

	return S_OK;
}

void* CFile_Manager::LoadObjects(const wstring& filename)
{
	wifstream file(filename);
	if (!file.is_open()) {
		throw runtime_error("Cannot open file for reading.");
		return nullptr;
	}

	wstring line;
	FILEDATA data;

	// ������ ���������� �д´�.
	while (getline(file, line)) {
		// �� ������ �ƴ϶��
		if (!line.empty()) {
			// �� ���� �Ľ��ϰ� �Ľ� �����ϸ� ���� ����
			if (ParseLine(line, data) == E_FAIL) {
				file.close();
				return nullptr;
			}
		}
	}

	file.close();
	return &m_vecpFileData;
}

HRESULT CFile_Manager::ParseLine(const wstring& line, FILEDATA& obj) {
	wistringstream iss(line);
	wstring key;
	// ':' �ݷ��� �������� Ű�� ������ ���ڿ� �и�
	if (getline(iss, key, L':')) {
		wstring value;
		getline(iss, value);
		// �¿� ���� ���� (C++11 �̻�)
		value.erase(0, value.find_first_not_of(L' ')); // ���� ���� ����
		value.erase(value.find_last_not_of(L' ') + 1); // ������ ���� ����

		if (key == L"LayerName") {
			obj.layerName = value;
		}
		else if (key == L"PrototypeTag") {
			obj.prototypeTag = value;
		}
		else if (key == L"LevelIndex") {
			obj.levelIndex = stoi(value);
		}
		else if (key == L"Position") {
			wistringstream vals(value);
			float a, b, c;
			vals >> a >> b >> c;
			obj.position.x = a;
			obj.position.y = b;
			obj.position.z = c;
		}
		else if (key == L"Scale") {
			wistringstream vals(value);
			float a, b, c;
			vals >> a >> b >> c;
			obj.scale.x = a;
			obj.scale.y = b;
			obj.scale.z = c;
		}
		else if (key == L"IsParsing") {
			obj.isParsing = (value == L"true" || value == L"1");
			// ��� ���� ���� �� ��ü�� ����
			m_vecpFileData.push_back(obj);
			obj = FILEDATA(); // ��ü �ʱ�ȭ
		}
		return S_OK;
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
