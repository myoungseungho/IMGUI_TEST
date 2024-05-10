#pragma once

#include "Base.h"

/* 생성한 객체들을 보관한다. */
/* Prototype 방식으로 객체를 추가한다. */


BEGIN(Engine)

#pragma once
class CFile_Manager final : public CBase
{
	struct FileData
	{
		wstring prototypeTag;
		wstring layerName;
		_uint levelIndex;
		_float3 position;
		_float3 scale;
	};
private:
	CFile_Manager();
	virtual ~CFile_Manager() = default;
public:
	HRESULT Initialize();
public:
	HRESULT SaveObjects(const wstring& filename, void* pArg);
	HRESULT LoadObjects(const wstring& filename);
	HRESULT ParseLine(const wstring& line, FileData& obj);

public:
	static CFile_Manager* Create();
	virtual void Free() override;
};

END