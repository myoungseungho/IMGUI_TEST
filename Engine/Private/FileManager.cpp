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
		file << L"Scale: " << data.scale.x << L" " << data.scale.y << L" " << data.scale.z << L"\n\n";
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

	//라인을 순차적으로 읽는다.
	while (getline(file, line)) {
		//빈 라인이 아니라면
		if (!line.empty()) {
			//각 라인 파싱하고 파싱 실패하면 실패 리턴
			if (ParseLine(line, data) == E_FAIL) {
				return nullptr;
			}
			//라인이 비면, 새로운 블록시 시작된다고 간주하고
			// 지금까지 파싱한 data 객체를 objects 벡터에 저장한다. 그 후 data 객체 초기화
			if (line == L"") {
				m_vecpFileData.push_back(data);
				data = FILEDATA();
			}
		}
	}

	file.close();
	return &m_vecpFileData;
}

HRESULT CFile_Manager::ParseLine(const wstring& line, FILEDATA& obj) {
	wistringstream iss(line);
	wstring key;
	// : 라는 콜론을 기준으로 키와 값으로 문자열 분리
	if (getline(iss, key, L':')) {
		wstring value;
		getline(iss, value);
		// 좌우 공백 제거 (C++11 이상)
		value.erase(0, value.find_first_not_of(L' ')); // 왼쪽 공백 제거
		value.erase(value.find_last_not_of(L' ') + 1); // 오른쪽 공백 제거

		if (key == L"LayerName") {
			obj.layerName = value;
		}
		else if (key == L"PrototypeTag") {
			obj.prototypeTag = value;
		}
		else if (key == L"LevelIndex") {
			obj.levelIndex = stoi(value);
		}
		else if (key == L"Position" || key == L"Scale") {
			wistringstream vals(value);
			float a, b, c;
			vals >> a >> b >> c;
			if (key == L"Position") {
				obj.position.x = a;
				obj.position.y = b;
				obj.position.z = c;
			}
			else if (key == L"Scale") {
				obj.scale.x = a;
				obj.scale.y = b;
				obj.scale.z = c;
				// Scale을 처리한 후 객체를 저장해야 함
				m_vecpFileData.push_back(obj); // 데이터 블록 완료
				obj = FILEDATA(); // 객체 초기화
			}
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
