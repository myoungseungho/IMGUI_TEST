#pragma once

#include "Base.h"

/* ������ ��ü���� �����Ѵ�. */
/* Prototype ������� ��ü�� �߰��Ѵ�. */


BEGIN(Engine)

#pragma once
class CFile_Manager final : public CBase
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
	CFile_Manager();
	virtual ~CFile_Manager() = default;
public:
	HRESULT Initialize();
public:
	HRESULT SaveObjects(const wstring& filename);
	HRESULT LoadObjects(const wstring& filename);
	HRESULT ParseLine(const wstring& line, FileData& obj);
private:
	vector<FileData> m_vecFileData;
public:
	static CFile_Manager* Create();
	virtual void Free() override;
};

END