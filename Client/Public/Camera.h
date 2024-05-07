#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera final : public CGameObject
{	
private:
	CCamera(LPDIRECT3DDEVICE9 pGraphic_Device); /* 원형생성 시 */
	CCamera(const CCamera& Prototype); /* 사본생성 시 */
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:		
	/* 뷰변환행렬 = 월드행렬의 역행렬 */
	CTransform*			m_pTransformCom = { nullptr };
	_float				m_fFovy = {};
	_float				m_fAspect = {};
	_float				m_fNear = {};
	_float				m_fFar = {};

private:
	HRESULT Ready_Components();

private:
	/* 투영행렬 : 투영변환을 할때 생길 수 있는 문제(fov가 90도로 고정된다 + z값이 모두 1이 된다. )들을 사전에 처리한다? */
	/* 투영변환 : / w */
	/* 렌더링 파이프라인에 필요한 뷰변환행렬 + 투영행렬 */
	HRESULT Bind_PipeLines();

public:
	/* 원형객체를 생성한다. */
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* 원형객체를 복제한 사본객체를 생성한다.(내 게임내에서 실제 동작하기위한 객체들) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END