#pragma once

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"
#include "Collider.h"
#include "Calc_Timer.h"
#include "KeyState.h"
#include "Animator.h"

/* 1. 원형객첼르 레벨별로 보관한다. */
/* 2. 지정한 원형객체를 복제하여 리턴한다. */

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	HRESULT Clear_Resources(_uint iLevelIndex);
private:
	_uint										m_iNumLevels = { 0 };
	map<const _wstring, class CComponent*>*		m_pPrototypes = { nullptr };

private:
	class CComponent* Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag);

public:
	static CComponent_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END