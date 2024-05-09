#pragma once

#include "Base.h"

/* 생성한 객체들을 보관한다. */
/* Prototype 방식으로 객체를 추가한다. */


BEGIN(Engine)

#pragma once
class CFileManager final : public CBase
{
private:
	CFileManager();
	virtual ~CFileManager() = default;
public:
	HRESULT Initialize();
public:
	static CFileManager* Create();
	virtual void Free() override;
};

END