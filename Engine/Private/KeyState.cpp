#include "KeyState.h"

CKeyState::CKeyState(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent { pGraphic_Device }
{
}

CKeyState::CKeyState(const CKeyState& Prototype)
	: CComponent{Prototype}
{
}

HRESULT CKeyState::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKeyState::Initialize(void* pArg)
{
	return S_OK;
}

_bool CKeyState::Key_Pressing(_uint _iKey)
{
	if (GetAsyncKeyState(_iKey) & 0x8000)
		return true;

	return false;
}

_bool CKeyState::Key_Down(_uint _iKey)
{
	if ((!m_bKeyState[_iKey]) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}

	return false;
}

_bool CKeyState::Key_Up(_uint _iKey)
{
	if ((m_bKeyState[_iKey]) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}

	return false;
}

CKeyState* CKeyState::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CKeyState* pInstance = new CKeyState(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : KeyState "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CKeyState::Clone(void* pArg)
{
	CKeyState* pInstance = new CKeyState(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : KeyState "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKeyState::Free()
{
	__super::Free();
}
