#pragma once

#include "Base.h"

BEGIN(Client)

class CSound_Manager : public CBase
{
private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg);



private:
	map<const wstring, class CSound*>		m_Prototypes;

public:
	static CSound_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END

