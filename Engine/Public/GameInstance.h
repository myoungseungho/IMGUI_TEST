#pragma once

#include "Renderer.h"
#include "Component_Manager.h"
#include "Collider_Manager.h"
BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	/* 엔진을 초기화한다. */
	HRESULT Initialize_Engine(HWND hWnd, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, LPDIRECT3DDEVICE9* ppOut);
	void Update_Engine(_float fTimeDelta);
	HRESULT Render_Engine(_float deltaTime);
	HRESULT Clear_LevelResources(_uint iLevelIndex);

public: /* For.Graphic_Device */
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);


public: /* For.Level_Manager */
	HRESULT Change_Level(class CLevel* pNewLevel);
	_uint	GetCurrentLevelIndex();
	_uint GetLoadingLevelIndex();
	HRESULT SetLoadingLevelIndex(_uint);
public: /* For.Timer_Manager */
	HRESULT Add_Timer(const _wstring& strTimerTag);
	_float Compute_TimeDelta(const _wstring& strTimerTag);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject_ToLayer(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strLayerTag, void* pArg = nullptr);
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex = 0);
	HRESULT AddObjectPrototypesVector(vector<string>*);
	HRESULT AddObjectLayersVector(_uint iLevelIndex, vector<pair < string, list<CGameObject*>>>*);
	HRESULT AddObjectLayersVector(_uint iLevelIndex, vector<pair < wstring, list<CGameObject*>>>*);


public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.Renderer */
	HRESULT Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);

public: /* For.FileManager */
	HRESULT SaveObjects(const wstring& filename, void* pArg);
	void* LoadObjects(const wstring& filename);
public: /* For.ColliderManager */
	HRESULT Add_ColliderObject(CCollider_Manager::COLLIDERGROUP eColliderGroup, class CGameObject* pColliderObject);
	HRESULT Show_Collider(bool);
	HRESULT OnCollisionCheckIntervalChanged(float);
	HRESULT Release_Collider(const CCollider*);

public: /* For.Picking */
	void Transform_ForPicking_ToLocalSpace(const _float4x4* pWorldMatrix);
	_bool Picked_InLocalSpace(const _float3* pPointA, const _float3* pPointB, const _float3* pPointC, _float3* pPickPos);
	_bool Picked_InWorldSpace(const _float3* pPointA, const _float3* pPointB, const _float3* pPointC, _float3* pPickPos);

private:
	class CGraphic_Device* m_pGraphic_Device = { nullptr };
	class CLevel_Manager* m_pLevel_Manager = { nullptr };
	class CTimer_Manager* m_pTimer_Manager = { nullptr };
	class CObject_Manager* m_pObject_Manager = { nullptr };
	class CComponent_Manager* m_pComponent_Manager = { nullptr };
	class CRenderer* m_pRenderer = { nullptr };
	class CFile_Manager* m_pFileManager = { nullptr };
	class CCollider_Manager* m_pColliderManager = { nullptr };
	class CPicking* m_pPicking = { nullptr };

public:
	void Release_Engine();

	virtual void Free() override;

};

END

