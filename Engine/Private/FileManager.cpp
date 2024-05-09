#include "..\Public\FileManager.h"

CFileManager::CFileManager()
{
}

HRESULT CFileManager::Initialize()
{
	return S_OK;
}

CFileManager* CFileManager::Create()
{
	CFileManager* pInstance = new CFileManager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CFileManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFileManager::Free()
{
	__super::Free();
}
