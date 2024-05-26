#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)

class CMon_Copy_Koofu  final :public CMonster
{

private:
	enum class ANIM_STATE {IDLE,  THROW , ANIM_END};
	enum class MON_STATE {IDLE , THROW , STATE_END};

public:
	typedef struct :public MONSTER_DESC
	{
	
	}MON_COPY_KOOFU_DESC;

private:
	CMon_Copy_Koofu(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMon_Copy_Koofu(const CMon_Copy_Koofu& Prototype);
	virtual ~CMon_Copy_Koofu() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	void Move_Dir();

	void Mon_State(_float fTimeDelta);
	void Anim_State(_float fTimeDelta);

	void State_Idle(_float fTimeDelta);
	void State_Throw(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Animation();

	virtual HRESULT Begin_RenderState();
	virtual HRESULT End_RenderState();

public:
	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other, _float fTimeDelta);
	virtual void OnCollisionExit(class CCollider* other);

private:
	void Warf(_int iPosX, _int iPosZ, _float fDistance);
	void Destory();

	HRESULT FuitCreate();

private:
	CTransform* m_pTargetTransform = { nullptr };

	MON_STATE m_eMon_State = {};
	ANIM_STATE m_eAnim_State = {};

public:
	static CMon_Copy_Koofu* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
