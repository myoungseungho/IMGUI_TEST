#pragma once

#include "Base.h"
/* 클라이언트 개발자가 제작한 객체들의 부모 클래스가 된다. */

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct
	{
		bool isPasingObject;
	}GAMEOBJECT_DESC;

	//enum class ObjectType {
	//	Player,
	//	Enemy,
	//	NPC,
	//	// 필요한 다른 타입들 추가
	//};

	//virtual ObjectType GetType() const = 0;

protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_Component(const _wstring& strComponentTag) {
		auto	iter = m_Components.find(strComponentTag);
		if (iter == m_Components.end())
			return nullptr;
		return iter->second;
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

public:
	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other);
	virtual void OnCollisionExit(class CCollider* other);

public:
	virtual void	Delete_Object();

public:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, class CComponent** ppOut, void* pArg = nullptr);

public:
	void SetPicking(_bool _picking) { m_bIsPicking = _picking; };
	_bool GetIsPicking() { return m_bIsPicking; };

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };

	_uint						m_iGameObjectData = { 0 };
	_bool						m_bIsPasingObject = { false };
	_bool						m_bIsPicking = { false };
protected:
	map<const _wstring, CComponent*>		m_Components;

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END