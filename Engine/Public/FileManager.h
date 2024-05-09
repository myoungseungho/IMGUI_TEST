#pragma once

#include "Base.h"

/* ������ ��ü���� �����Ѵ�. */
/* Prototype ������� ��ü�� �߰��Ѵ�. */


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