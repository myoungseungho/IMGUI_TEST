#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CKeyState final:public CComponent
{
private:
	CKeyState(LPDIRECT3DDEVICE9 pGraphic_Device);
	CKeyState(const CKeyState& Prototype);
	virtual ~CKeyState() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	_bool 		Key_Pressing(_uint _iKey);
	_bool 			Key_Down(_uint _iKey);
	_bool 			Key_Up(_uint _iKey);

private:
	_bool m_bKeyState[VK_MAX];

public:
	static CKeyState* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END