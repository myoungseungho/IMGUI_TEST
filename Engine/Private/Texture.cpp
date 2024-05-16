#include "..\Public\Texture.h"


CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{

}

CTexture::CTexture(const CTexture & rhs)
	: CComponent{ rhs }
	, m_iNumTextures{ rhs.m_iNumTextures }
	, m_Textures{ rhs.m_Textures }	
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);
}

HRESULT CTexture::Initialize_Prototype(TYPE eType, const _wstring & strTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;


	/* "../Bin/Resources/Textures/Explosion/Explosion%d.png" */
	/* "../Bin/Resources/Textures/Snow/Snow.png" */

	/* 텍스쳐를 로드하여 m_Textures에 보관한다. */
	for (size_t i = 0; i < iNumTextures; i++)
	{
		LPDIRECT3DBASETEXTURE9			pTexture = { nullptr };

		_tchar			szTextureFilePath[MAX_PATH] = TEXT("");

		wsprintf(szTextureFilePath, strTextureFilePath.c_str(), i);		

		HRESULT hr = TYPE_TEXTURE2D == eType ? D3DXCreateTextureFromFile(m_pGraphic_Device, szTextureFilePath, (LPDIRECT3DTEXTURE9*)&pTexture) : D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szTextureFilePath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
		if (FAILED(hr))
			return E_FAIL;

		m_Textures.emplace_back(pTexture);		
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_Texture(_uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return m_pGraphic_Device->SetTexture(0, m_Textures[iTextureIndex]);	
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _wstring & strTextureFilePath, _uint iNumTextures)
{
	CTexture*		pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eType, strTextureFilePath, iNumTextures)))
	{
		MSG_BOX(TEXT("Failed to Created : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CTexture::Clone(void * pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

	m_Textures.clear();
}
