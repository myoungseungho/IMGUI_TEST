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

HRESULT CTexture::Bind_Anim(_uint fFrame)
{
	//어떤 하나의 애니메이션의 텍스쳐
	//애니메이션 텍스쳐 전체 장보다 현재 재생되고 있는
	//프레임이 더 크면, 현재 재생하고 있는 프레임 단계를
	//0으로 초기화.
	
	if (fFrame >= m_iNumTextures)
		fFrame = 0;
	
	HRESULT hr = m_pGraphic_Device->SetTexture(0, m_Textures[fFrame]);
	//현재 재생하고 있는 프레임의 텍스쳐를 장치에 바인딩
	return hr;
}


void CTexture::ChangeTextureColor(D3DCOLOR originalColor, D3DCOLOR newColor)
{
	if (m_Textures.empty())
		return;

	IDirect3DTexture9* pTexture = static_cast<IDirect3DTexture9*>(m_Textures[0]);
	if (!pTexture)
		return;

	D3DLOCKED_RECT lockedRect;
	if (FAILED(pTexture->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD)))
		return;

	BYTE* pTexels = (BYTE*)lockedRect.pBits;
	D3DSURFACE_DESC desc;
	pTexture->GetLevelDesc(0, &desc);

	for (UINT y = 0; y < desc.Height; ++y)
	{
		for (UINT x = 0; x < desc.Width; ++x)
		{
			BYTE* pPixel = pTexels + y * lockedRect.Pitch + x * 4;
			D3DCOLOR color = D3DCOLOR_ARGB(pPixel[3], pPixel[2], pPixel[1], pPixel[0]);

			if (color == originalColor)
			{
				pPixel[2] = (newColor >> 16) & 0xFF; // R
				pPixel[1] = (newColor >> 8) & 0xFF;  // G
				pPixel[0] = newColor & 0xFF;         // B
				pPixel[3] = (newColor >> 24) & 0xFF; // A
			}
			else if (color == D3DCOLOR_XRGB(0, 0, 0)) // 검정색 부분을 투명화
			{
				pPixel[3] = 0; // A 값을 0으로 만들어 투명하게 만듦
			}
		}
	}

	pTexture->UnlockRect(0);
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
