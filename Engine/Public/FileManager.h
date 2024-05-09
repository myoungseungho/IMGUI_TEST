#pragma once

#include "Base.h"

/* ������ ��ü���� �����Ѵ�. */
/* Prototype ������� ��ü�� �߰��Ѵ�. */


BEGIN(Engine)

#pragma once
class CFileManager final : public CBase
{
	struct FileData
	{
		wstring layerName;
		wstring prototypeTag;
		_uint levelIndex;
		_float3 position;
		_float3 rotation;
		_float3 scale;
	};
private:
	CFileManager();
	virtual ~CFileManager() = default;
public:
	HRESULT Initialize();
public:
	HRESULT SaveObjects(const wstring& filename);
	HRESULT LoadObjects(const wstring& filename);
	HRESULT ParseLine(const wstring& line, FileData& obj);
private:
	vector<FileData> m_vecFileData;
public:
	static CFileManager* Create();
	virtual void Free() override;
};

END