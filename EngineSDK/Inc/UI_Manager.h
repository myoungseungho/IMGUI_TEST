#pragma once

#include "Base.h"

/* ������ ��ü���� �����Ѵ�. */
/* Prototype ������� ��ü�� �߰��Ѵ�. */


BEGIN(Engine)

class CUI_Manager final : public CBase
{
private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex);
	
public:
	HRESULT Initialize();
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject_ToLayer(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strLayerTag, void* pArg = nullptr);
	HRESULT AddObjectPrototypesVector(vector<string>*);
	HRESULT AddObjectLayersVector(_uint, vector<pair < string, list<CGameObject*>>>* pVector);
	HRESULT AddObjectLayersVector(_uint, vector<pair < wstring, list<CGameObject*>>>* pVector);


	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	HRESULT Clear_Resources(_uint iLevelIndex);
private:
	map<const wstring, class CGameObject*>		m_Prototypes;

private:
	_uint										m_iNumLevels = { 0 };
	map<const wstring, class CLayer*>* m_pLayers = { nullptr };

private:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);

public:
	static CUI_Manager* Create();
	virtual void Free() override;
};

END