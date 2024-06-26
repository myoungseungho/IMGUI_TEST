#include "..\Public\GameInstance.h"

#include "Component_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "FileManager.h"
#include "Collider_Manager.h"
#include "Picking.h"
#include "Sound_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{

}

HRESULT CGameInstance::Initialize_Engine(HWND hWnd, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, LPDIRECT3DDEVICE9* ppOut)
{
	m_pGraphic_Device = CGraphic_Device::Create(hWnd, iWinSizeX, iWinSizeY, ppOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pFileManager = CFile_Manager::Create();
	if (nullptr == m_pFileManager)
		return E_FAIL;

	m_pColliderManager = CCollider_Manager::Create();
	if (nullptr == m_pColliderManager)
		return E_FAIL;

	m_pPicking = CPicking::Create(*ppOut, hWnd);
	if (nullptr == m_pPicking)
		return E_FAIL;

	m_pSoundManager = CSound_Manager::Create(iNumLevels);
	if (nullptr == m_pSoundManager)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pPicking->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);

	m_pObject_Manager->Update(fTimeDelta);

	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pLevel_Manager->Update(fTimeDelta);

	m_pColliderManager->Update(fTimeDelta);

	m_pSoundManager->Update(fTimeDelta);
}

HRESULT CGameInstance::Render_Engine(_float deltaTime)
{
	/* 엔진에서 관리하는 객체들 중, 반복적인 렌더가 필요한 객체들이 있다면. */
	/* 여기에서 렌더를 수행해준다. */

	if (FAILED(m_pRenderer->Draw(deltaTime)))
		return E_FAIL;

	if (FAILED(m_pLevel_Manager->Render()))
		return E_FAIL;

	if (FAILED(m_pColliderManager->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Clear_LevelResources(_uint iLevelIndex)
{
	if (FAILED(m_pObject_Manager->Clear_Resources(iLevelIndex)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Clear_Resources(iLevelIndex)))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Render_Begin()
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_Begin();
}

void CGameInstance::Render_End(HWND hWnd)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_End(hWnd);
}


HRESULT CGameInstance::Change_Level(CLevel* pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Change_Level(pNewLevel);
}

_uint CGameInstance::GetCurrentLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->GetCurrentLevelIndex();
}

_uint CGameInstance::GetLoadingLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->GetLoadingLevelIndex();
}

HRESULT CGameInstance::SetLoadingLevelIndex(_uint _level)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->SetLoadingLevelIndex(_level);
}

CLevel* CGameInstance::GetCurrentLevel()
{
	if (nullptr == m_pLevel_Manager)
		return nullptr;

	return m_pLevel_Manager->GetCurrentLevel();
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

_float CGameInstance::Compute_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Prototype(const wstring& strPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strLayerTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject_ToLayer(iLevelIndex, strPrototypeTag, strLayerTag, pArg);
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return	m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);
}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return	m_pObject_Manager->Get_GameObject(iLevelIndex, strLayerTag, iIndex);
}

HRESULT CGameInstance::AddObjectPrototypesVector(vector<string>* pVector)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return	m_pObject_Manager->AddObjectPrototypesVector(pVector);
}

HRESULT CGameInstance::AddObjectLayersVector(_uint iLevelIndex, vector<pair < string, list<CGameObject*>>>* pVector)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return	m_pObject_Manager->AddObjectLayersVector(iLevelIndex, pVector);
}

HRESULT CGameInstance::AddObjectLayersVector(_uint iLevelIndex, vector<pair < wstring, list<CGameObject*>>>* pVector)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return	m_pObject_Manager->AddObjectLayersVector(iLevelIndex, pVector);
}

CGameObject* CGameInstance::GetObjectByIndex(_uint iLevelIndex, const wstring& strLayerTag, _uint index)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return	m_pObject_Manager->GetObjectByIndex(iLevelIndex, strLayerTag, index);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, CComponent* pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CComponent* CGameInstance::Clone_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderObject(eRenderGroup, pRenderObject);
}

HRESULT CGameInstance::SaveObjects(const wstring& filename, void* pArg)
{
	if (nullptr == m_pFileManager)
		return E_FAIL;

	return m_pFileManager->SaveObjects(filename, pArg);
}

void* CGameInstance::LoadObjects(const wstring& filename)
{
	if (nullptr == m_pFileManager)
		return nullptr;

	return m_pFileManager->LoadObjects(filename);
}

HRESULT CGameInstance::Add_ColliderObject(CCollider_Manager::COLLIDERGROUP eColliderGroup, CGameObject* pColliderObject)
{
	if (nullptr == m_pColliderManager)
		return E_FAIL;

	return m_pColliderManager->Add_ColliderObject(eColliderGroup, pColliderObject);
}

HRESULT CGameInstance::Show_Collider(bool _isOn)
{
	if (nullptr == m_pColliderManager)
		return E_FAIL;

	return m_pColliderManager->Show_Collider(_isOn);
}

HRESULT CGameInstance::OnCollisionCheckIntervalChanged(float _fCollisionCheckInterval)
{
	if (nullptr == m_pColliderManager)
		return E_FAIL;

	return m_pColliderManager->OnCollisionCheckIntervalChanged(_fCollisionCheckInterval);
}

HRESULT CGameInstance::Release_Collider(const CCollider* targetCollider)
{
	if (nullptr == m_pColliderManager)
		return E_FAIL;

	m_pColliderManager->Release_Collider(targetCollider);
}

void CGameInstance::Transform_ForPicking_ToLocalSpace(const _float4x4* pWorldMatrix)
{
	return m_pPicking->Transform_ToLocalSpace(pWorldMatrix);
}

_bool CGameInstance::Picked_InLocalSpace(const _float3* pPointA, const _float3* pPointB, const _float3* pPointC, _float3* pPickPos)
{
	return m_pPicking->Picked_InLocalSpace(pPointA, pPointB, pPointC, pPickPos);
}

_bool CGameInstance::Picked_InWorldSpace(const _float3* pPointA, const _float3* pPointB, const _float3* pPointC, _float3* pPickPos)
{
	return m_pPicking->Picked_InWorldSpace(pPointA, pPointB, pPointC, pPickPos);
}


void CGameInstance::Register_Sound(const std::wstring& filePath, const std::wstring& alias, _uint levelId, _uint soundType)
{
	if (m_pSoundManager == nullptr)
		return;

	return m_pSoundManager->Register_Sound(filePath, alias, levelId, soundType);
}

void CGameInstance::Play_Sound(const std::wstring& alias, _uint levelId, bool loop)
{
	if (m_pSoundManager == nullptr)
		return;

	return m_pSoundManager->Play_Sound(alias, levelId, loop);
}

void CGameInstance::Stop_Sound(const wstring& filePath, _uint levelId)
{
	if (m_pSoundManager == nullptr)
		return;

	return m_pSoundManager->Stop_Sound(filePath, levelId);

}

void CGameInstance::Set_Volume(const wstring& alias, _uint levelId, float volume)
{
	if (m_pSoundManager == nullptr)
		return;

	return m_pSoundManager->Set_Volume(alias, levelId, volume);

}

void CGameInstance::Stop_All_Sounds(_uint levelId)
{
	if (m_pSoundManager == nullptr)
		return;

	return m_pSoundManager->Stop_All_Sounds(levelId);
}

void CGameInstance::Release_Engine()
{
	Safe_Release(m_pSoundManager);
	Safe_Release(m_pPicking);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pFileManager);
	Safe_Release(m_pColliderManager);
	Safe_Release(m_pGraphic_Device);
	CGameInstance::Get_Instance()->Destroy_Instance();
}

void CGameInstance::Free()
{
	__super::Free();
}
